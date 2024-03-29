#include "ParseDataTxt.hpp"
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
#include <QDateTime>

void ParseDataTxt::setChannelsNames(const QString &string_names) {

    auto list_names = string_names.split(delimiter_names);

    for (int i = 0; i < amountOfChannels; ++i) {
        channels_names[i] = list_names[i];
    }
}

void ParseDataTxt::getData(QTextStream &file, auto &val) {
    QString in_str;

    do {
        in_str = file.readLine();
    } while (in_str.isEmpty() || in_str[0] == '#');

    getVal(val, in_str);
}

void ParseDataTxt::getData(QTextStream &file, QString &val) {
    QString in_str;
    do {
        in_str = file.readLine();
    } while (in_str.isEmpty() || in_str[0] == '#');

    val = in_str;
}

void ParseDataTxt::getVal(unsigned long long &val_to, const QString &str_from) {
    val_to = str_from.toULongLong();
}

void ParseDataTxt::getVal(double &val_to, const QString &str_from) {
    val_to = str_from.toDouble();
}

void ParseDataTxt::setChannels(ParseDataTxt *data, const QList<QByteArray>& list,
                        qint64 first, qint64 last, QList<std::pair<double, double>>* sub_extr) {
    auto delim = data->delimiter_nums;
    for (qint64 p = first; p < last; ++p) {
        auto nums = list[p].split(delim);
        for (qint64 i = 0; i < data->amountOfChannels; ++i) {
            data->channels[i][p] = nums[i].toDouble();

            if (sub_extr->at(i).first > data->channels[i][p]) sub_extr->operator[](i).first = data->channels[i][p];
            if (sub_extr->at(i).second < data->channels[i][p]) sub_extr->operator[](i).second = data->channels[i][p];
        }
    }
}

void ParseDataTxt::threadsHandle(const QList<QByteArray>& list) {
    size_t number_of_threads = std::thread::hardware_concurrency();

    if (number_of_threads * 4 > amountOfSamples) {
        setChannels(this, list, 0, amountOfSamples, &extremums);
        return;
    }


    QList<std::pair<double, double>> se(amountOfChannels,
                                              {std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()});
    QList<QList<std::pair<double, double>>> sub_extrs(number_of_threads, se);

    std::vector<std::unique_ptr<std::thread>> threads;
    for (size_t y = 0; y < number_of_threads; ++y) {
        threads.emplace_back(std::make_unique<std::thread>(setChannels, this, list,
                                     amountOfSamples * y / number_of_threads,
                                     amountOfSamples * (y+1) / number_of_threads,
                                     &sub_extrs[y]));
    }

    for (size_t y = 0; y < number_of_threads; ++y) {
        threads[y]->join();
        for (size_t i = 0; i < amountOfChannels; ++i) {
            extremums[i].first = std::min(extremums[i].first, sub_extrs[y][i].first);
            extremums[i].second = std::max(extremums[i].second, sub_extrs[y][i].second);
        }
    }
}

qint64 ParseDataTxt::parseGeneral(QTextStream &file_to_parse) {
    QString in_str;

    QString startDate_, startTime_;
    getData(file_to_parse, amountOfChannels);
    getData(file_to_parse, amountOfSamples);
    rcur = amountOfSamples - 1;
    getData(file_to_parse, Hz);
    getData(file_to_parse, startDate_);
    getData(file_to_parse, startTime_);

    startTime = startDate_ + " " + startTime_;

    totalSeconds = (amountOfSamples - 1) / Hz;
    setDuration(totalSeconds);
    setStopTime();

    channels_names.resize(amountOfChannels);
    getData(file_to_parse, in_str);
    setChannelsNames(in_str);

    channels.resize(amountOfChannels);
    for (int i = 0; i < amountOfChannels; ++i)
        channels[i].resize(amountOfSamples);

    extremums.resize(amountOfChannels, {std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()});

    return file_to_parse.pos();
}

void ParseDataTxt::parse(const std::filesystem::path &path_to_file) {

    auto s = std::chrono::high_resolution_clock().now();

    QFile file_to_parse(path_to_file);
    file_to_parse.open(QFile::Text | QFile::ReadOnly);
    QTextStream stream (&file_to_parse);
    stream.setEncoding(QStringConverter::System);

    auto pos = parseGeneral(stream);

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms: Header";

    file_to_parse.seek(pos);
    auto list1 = file_to_parse.readAll();

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms: Data read";

    auto list = list1.split('\n');

    while (list.back().isEmpty()) {
        qDebug() << "last line is empty";
        list.pop_back();
    }

    if (list.size() != amountOfSamples) {
        qDebug() << QString::fromStdString( "amountOfSamples != amount of samples in file " +
                                 std::to_string(amountOfSamples) + " " + std::to_string(list.size()));
        amountOfSamples = list.size();
    }

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms: Data splitted";

    threadsHandle(list);

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms: Data parsed";
}
