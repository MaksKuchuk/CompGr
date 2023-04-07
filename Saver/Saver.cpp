#include "Saver.hpp"
#include <string>
#include <fstream>
#include <QString>
#include <QFile>
#include <QTextStream>

void Saver::TxtSaver(ParseData *data, const QString& path_to_file) {
    QFile file_out(path_to_file);
    file_out.open(QFile::Text | QFile::WriteOnly);
    QTextStream stream (&file_out);

    auto time = data->getStartTime().split(' ');
    stream << data->getAmountOfChannels() << '\n'
        << data->getAmountOfSamples() << '\n'
        << data->getHz() << '\n'
        << time[0] << '\n'
        << time[1] << '\n';

    for (size_t i = 0; i < data->getAmountOfChannels(); ++i) {
        stream << data->getChannelName(i) << ";";
    }
    stream << '\n';

    for (size_t i = 0; i < data->getAmountOfSamples(); ++i) {
        for (size_t j = 0; j < data->getAmountOfChannels(); ++j) {
            stream << data->getChannel(j)[i] << ' ';
        }
        stream << '\n';
    }
}


void Saver::TxtSaver(SaveHandler *data, const QString& path_to_file) {
    QFile file_out(path_to_file);
    file_out.open(QFile::Text | QFile::WriteOnly);
    QTextStream stream (&file_out);

    stream << data->amountOfChannels << '\n'
        << data->amountOfSamples << '\n'
        << data->Hz << '\n'
        << data->startDate << '\n'
        << data->startTime << '\n';

    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        stream << data->channels_name[i] << ";";
    }
    stream << '\n';

    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        for (size_t j = 0; j < data->amountOfChannels; ++j) {
            stream << data->channels[j][i] << ' ';
        }
        stream << '\n';
    }
}
