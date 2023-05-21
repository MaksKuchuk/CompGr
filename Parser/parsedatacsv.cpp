#include "parsedatacsv.h"
#include <QList>
#include <QByteArray>

ParseDataCsv::ParseDataCsv() {
    delimiter = ';';
}

void ParseDataCsv::parseHeader(QList<QByteArray>& list) {
    amountOfChannels = list[0].count(delimiter) + 1;
    amountOfSamples = list.size() - 1;

    channels_names.resize(amountOfChannels);
    setChannelsNames(list[0]);
    list.pop_front();

    channels.resize(amountOfChannels);
    for (int i = 0; i < amountOfChannels; ++i)
        channels[i].resize(amountOfSamples);

    extremums.resize(amountOfChannels, {std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()});
}

void ParseDataCsv::parseTime() {
    totalSeconds = channels[0].back();
    startTime = "01-01-2000 00:00:00.000";

    size_t n = amountOfSamples < 10 ? amountOfSamples : 10;
    QList<double> diffs;
    for (size_t i = 0; i < n - 1; ++i) {
        diffs.push_back(channels[0][i+1] - channels[0][i]);
    }

    std::nth_element(diffs.begin(), diffs.begin() + diffs.size()/2, diffs.end());
    Hz = diffs[diffs.size()/2];

    setDuration(totalSeconds);
    setStopTime();
}

void ParseDataCsv::parse(const std::filesystem::path &path_to_file) {

    auto s = std::chrono::high_resolution_clock().now();

    QFile file_to_parse(path_to_file);
    file_to_parse.open(QFile::Text | QFile::ReadOnly);
    QTextStream stream (&file_to_parse);
    stream.setEncoding(QStringConverter::System);

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms";

    auto list1 = file_to_parse.readAll();

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms";

    auto list = list1.split('\n');

    while (list.back().isEmpty()) {
        qDebug() << "last line is empty: " + list.back();
        list.pop_back();
    }

    parseHeader(list);

    if (list.size() != amountOfSamples) {
        qDebug() << QString::fromStdString( "amountOfSamples != amount of samples in file " +
                                 std::to_string(amountOfSamples) + " " + std::to_string(list.size()));
        amountOfSamples = list.size();
    }

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms";

    threadsHandle(list);

    parseTime();

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms";
}
