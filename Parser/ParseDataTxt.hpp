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

#include <QDebug>

class ParseDataTxt : public ParseData {
    void setChannelsNames(const QString &string_names) {

        auto list_names = string_names.split(';');

        for (int i = 0; i < amountOfChannels; ++i) {
            channels_names[i] = list_names[i];
        }
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

    static void setChannels(ParseDataTxt *data, const QList<QByteArray>& list,
                            qint64 first, qint64 last) {
        for (qint64 p = first; p < last; ++p) {
            auto nums = list[p].split(' ');
            for (qint64 i = 0; i < data->amountOfChannels; ++i) {
                data->channels[i][p] = nums[i].toDouble();
            }
        }
    }

    void threadsHandle(const QList<QByteArray>& list) {
        size_t number_of_threads = std::thread::hardware_concurrency();

        if (number_of_threads * 4 > amountOfSamples) {
            setChannels(this, list, 0, amountOfSamples);
            return;
        }

        auto **threads = new std::thread *[number_of_threads];
        for (size_t y = 0; y < number_of_threads; ++y) {
            threads[y] = new std::thread(setChannels, this, list,
                                         amountOfSamples * y / number_of_threads, amountOfSamples * (y+1) / number_of_threads);
        }

        for (size_t y = 0; y < number_of_threads; ++y) {
            threads[y]->join();
        }
        for (size_t y = 0; y < number_of_threads; ++y) {
            delete threads[y];
        }
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
        std::istringstream ss((startDate_ + " " + startTime.sliced(0, 8)).toStdString());
        //ss.imbue(std::locale(""));
        ss >> std::get_time(&t, "%d-%m-%Y %H:%M:%S");
        auto dotPos = startTime_.indexOf('.');
        int milliseconds = 0;
        if (dotPos != std::string::npos)
            milliseconds = startTime_.sliced(dotPos+1).toInt();

        t.tm_sec += totalSeconds + milliseconds / 1000.0;
        mktime(&t);

        int millAdd = (totalSeconds - floor(totalSeconds)) * 1000 + milliseconds;
        milliseconds = millAdd >= 1000 ? millAdd % 1000 : millAdd;

        std::ostringstream oss;
        oss << std::put_time(&t, "%d-%m-%Y %H:%M:%S");
        if (milliseconds > 0)
            oss << '.' << milliseconds;

        stopTime = QString::fromStdString( oss.str() );

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
    void parse(const std::filesystem::path &path_to_file) override {
        QFile file_to_parse(path_to_file);
        file_to_parse.open(QFile::Text | QFile::ReadOnly);
        QTextStream stream (&file_to_parse);
        stream.setEncoding(QStringConverter::System);

        auto pos = parseGeneral(stream);

        file_to_parse.seek(pos);
        auto list = file_to_parse.readAll().split('\n');

        while (list.back().isEmpty()) {
            qDebug() << "last line is empty: " + list.back();
            list.pop_back();
        }

        if (list.size() != amountOfSamples) {
            qDebug() << QString::fromStdString( "amountOfSamples != amount of samples in file " +
                                     std::to_string(amountOfSamples) + " " + std::to_string(list.size()));
            amountOfSamples = list.size();
        }


        threadsHandle(list);

        findExtrs();
    }
};

#endif
