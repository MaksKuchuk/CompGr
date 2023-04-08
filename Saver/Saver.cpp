#include "Saver.hpp"
#include <string>
#include <fstream>
#include <QString>
#include <QFile>
#include <QTextStream>

void Saver::TxtSaver(ParseData *data, const QString& path_to_file) {
    std::vector<size_t> channels(data->getAmountOfChannels());
    for (size_t i = 0; i<channels.size(); ++i) {
        channels[i] = i;
    }
    TxtSaver(data, 0, data->getAmountOfSamples(), channels, path_to_file);
}


/*
 * first - first sample
 *
 * last - last sample (not including)
 *
 * channels - vector of channel's indices to save
 */
void Saver::TxtSaver(ParseData *data, const size_t first, const size_t last, const std::vector<size_t>& channels, const QString& path_to_file) {
    QFile file_out(path_to_file);
    file_out.open(QFile::Text | QFile::WriteOnly);
    QTextStream stream (&file_out);

    auto time = data->getStartTime().split(' ');
    stream << channels.size() << '\n'
        << last-first << '\n'
        << data->getHz() << '\n'
        << time[0] << '\n'
        << time[1] << '\n';

    for (auto i : channels) {
        stream << data->getChannelName(i) << ";";
    }
    stream << '\n';

    for (size_t i = first; i < last; ++i) {
        for (auto j : channels) {
            stream << data->getChannel(j)[i] << ' ';
        }
        stream << '\n';
    }
}


void Saver::TxtSaver(SaveHandler *data, const QString& path_to_file) {
    std::vector<size_t> channels(data->amountOfChannels);
    for (size_t i = 0; i<channels.size(); ++i) {
        channels[i] = i;
    }
    TxtSaver(data, 0, data->amountOfSamples, channels, path_to_file);
}

/*
 * first - first sample
 *
 * last - last sample (not including)
 *
 * channels - vector of channel's indices to save
 */
void Saver::TxtSaver(SaveHandler *data, const size_t first, const size_t last, const std::vector<size_t>& channels, const QString& path_to_file) {
    QFile file_out(path_to_file);
    file_out.open(QFile::Text | QFile::WriteOnly);
    QTextStream stream (&file_out);

    stream << channels.size() << '\n'
        << last-first << '\n'
        << data->Hz << '\n'
        << data->startDate << '\n'
        << data->startTime << '\n';

    for (auto i : channels) {
        stream << data->channels_name[i] << ";";
    }
    stream << '\n';

    for (size_t i = first; i < last; ++i) {
        for (auto j : channels) {
            stream << data->channels[j][i] << ' ';
        }
        stream << '\n';
    }
}
