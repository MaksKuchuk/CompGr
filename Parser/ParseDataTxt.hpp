#ifndef PARSEDATATXT_H
#define PARSEDATATXT_H

#include "ParseData.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <thread>
#include <QFile>
#include <QTextStream>
#include <QByteArray>

class ParseDataTxt : public ParseData {
    void setChannelsNames(const QString &string_names) {
        QString cur;
        int i{};
        for (QChar c: string_names) {
            if (c != ';')
                cur += c;
            else {
                channels_names[i] = cur;
                i++;
                cur.clear();
            }
        }
        if (!cur.toStdWString().empty())
            channels_names[i] = cur;
    }

    static void getData(QTextStream &file, auto &val) {
        QString in_str;

        do {
            in_str = file.readLine();
        } while (in_str.isEmpty() || in_str[0] == '#');

        getVal(val, in_str);
    }

    static void getData(QTextStream &file, QString &val) {
        QString in_str;
        do {
            in_str = file.readLine();
        } while (in_str.isEmpty() || in_str[0] == '#');

        val = in_str;
    }

    static void getVal(unsigned long long &val_to, const QString &str_from) {
        val_to = str_from.toULongLong();
    }

    static void getVal(double &val_to, const QString &str_from) {
        val_to = str_from.toDouble();
    }

    static void setChannels(ParseDataTxt *data, size_t start_line,
                            const QString& buf, size_t start_buf, size_t end) {
        QString cur_num{};
        size_t i{}, j = start_line;
        for (size_t p = start_buf; p <= end; ++p) {
            if ((buf[p] == ' ' || buf[p] == '\n') && !cur_num.isEmpty()) {
                data->channels[i][j] = cur_num.toDouble();
                cur_num.clear();
                ++i;
                if (i >= data->amountOfChannels) {
                    i = 0;
                    ++j;
                }
            } else {
                if (buf[p] != ' ' && buf[p] != '\n' && buf[p] != '\r')
                    cur_num += buf[p];
            }
        }
    }

    void threadsHandle(const QString& buf) {
        size_t number_of_threads = std::thread::hardware_concurrency();
        size_t buf_length = buf.size() - 1;

        if (number_of_threads * 4 > amountOfSamples) {
            setChannels(this, 0, buf, 0, buf_length);
            return;
        }

        auto *position_end_line = new size_t[number_of_threads]();
        auto *position_end_buf = new size_t[number_of_threads]();

        for (size_t y = 0; y < number_of_threads; ++y) {
            position_end_line[y] = amountOfSamples * (y + 1) / number_of_threads;
        }

        size_t ii{}, z{}, count{};
        while (ii < buf_length && z + 1 < number_of_threads) {
            if (buf[ii] == '\n') {
                ++count;
                if (count == position_end_line[z]) {
                    position_end_buf[z] = ii;
                    ++z;
                }
            }
            ++ii;
        }
        position_end_buf[number_of_threads - 1] = buf_length;

        size_t s{}, e{}, el{};
        auto **threads = new std::thread *[number_of_threads];
        for (size_t y = 0; y < number_of_threads; ++y) {
            if (y == 0) {
                s = 0;
            } else {
                s = position_end_buf[y - 1] + 1;
                el = position_end_line[y - 1];
            }
            e = position_end_buf[y];
            threads[y] = new std::thread(setChannels, this, el, buf, s, e);
        }

        for (size_t y = 0; y < number_of_threads; ++y) {
            threads[y]->join();
        }
        for (size_t y = 0; y < number_of_threads; ++y) {
            delete threads[y];
        }

        delete[] position_end_buf;
        delete[] position_end_line;
        delete[] threads;
    }

    qint64 parseGeneral(QTextStream &file_to_parse) {
        QString in_str;

        QString startDate_, startTime_;
        getData(file_to_parse, amountOfChannels);
        getData(file_to_parse, amountOfSamples);
        getData(file_to_parse, Hz);
        getData(file_to_parse, startDate_);
        getData(file_to_parse, startTime_);

        startTime = startDate_ + " " + startTime_;

        totalSeconds = (amountOfSamples - 1) / Hz;
        setDuration(totalSeconds);

        std::tm t{};
        std::istringstream ss(startDate_.toStdString() + " " + startTime_.toStdString().substr(0, 8));
        //ss.imbue(std::locale(""));
        ss >> std::get_time(&t, "%d-%m-%Y %H:%M:%S");
        auto dotPos = startTime_.toStdString().find('.');
        int milliseconds = 0;
        if (dotPos != std::string::npos)
            milliseconds = std::stoi(startTime_.toStdString().substr(dotPos + 1, startTime_.size() - 1));

        t.tm_sec += totalSeconds + milliseconds / 1000.0;
        mktime(&t);

        int millAdd = (totalSeconds - floor(totalSeconds)) * 1000 + milliseconds;
        milliseconds = millAdd >= 1000 ? millAdd % 1000 : millAdd;

        std::ostringstream oss;
        oss << std::put_time(&t, "%d-%m-%Y %H:%M:%S");
        if (milliseconds > 0)
            oss << '.' << milliseconds;

        stopTime = QString( oss.str().c_str() );

        channels_names = new QString[amountOfChannels]();
        getData(file_to_parse, in_str);

        setChannelsNames(in_str);

        channels = new double *[amountOfChannels]();
        for (int i = 0; i < amountOfChannels; ++i)
            channels[i] = new double[amountOfSamples];

        return file_to_parse.pos();
    }

    void findExtrs() {
        extremums = new std::pair<double, double>[amountOfChannels];
        for (int i = 0; i < amountOfChannels; ++i) {
            extremums[i].first = extremums[i].second = channels[i][0];
        }
        for (int k = 1; k < amountOfSamples; ++k) {
            for (int i = 0; i < amountOfChannels; ++i) {
                extremums[i].second = std::max(extremums[i].second, channels[i][k]);
                extremums[i].first = std::min(extremums[i].first, channels[i][k]);
            }
        }
    }

public:
    void parse2(const QString &path_to_file)  {
        QFile file_to_parse(path_to_file);

        file_to_parse.open(QFile::Text | QFile::ReadOnly);

        QTextStream stream (&file_to_parse);
        stream.setEncoding(QStringConverter::System);

        parseGeneral(stream);

        for (size_t i = 0; i < amountOfSamples; ++i) {
            for (size_t j = 0; j < amountOfChannels; ++j) {
                stream >> channels[j][i];
            }
        }

        findExtrs();
    }

    void parse(const QString &path_to_file) override {
        QFile file_to_parse(path_to_file);
        file_to_parse.open(QFile::Text | QFile::ReadOnly);
        QTextStream stream (&file_to_parse);
        stream.setEncoding(QStringConverter::System);

        auto pos = parseGeneral(stream);

        file_to_parse.seek(pos);
        auto buf = file_to_parse.readAll();
        buf.push_back('\n');
        threadsHandle(buf);

        findExtrs();
    }
};

#endif
