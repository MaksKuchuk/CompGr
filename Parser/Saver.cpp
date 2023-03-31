#include "Saver.h"
#include <string>
#include <fstream>

void Saver::TxtSaver(ParseData *data, const std::string &path_to_file) {
    std::ofstream file_out(path_to_file);

    size_t space_place = data->getStartTime().find(' ');
    file_out << data->getAmountOfChannels() << '\n'
        << data->getAmountOfSamples() << '\n'
        << data->getHz() << '\n'
        << data->getStartTime().replace(space_place, 1, "\n") << '\n';

    for (size_t i = 0; i < data->getAmountOfChannels(); ++i) {
        file_out << data->getChannelName(i) << ";";
    }
    file_out << '\n';

    for (size_t i = 0; i < data->getAmountOfSamples(); ++i) {
        for (size_t j = 0; j < data->getAmountOfChannels(); ++j) {
            file_out << data->getChannel(j)[i] << ' ';
        }
        file_out << '\n';
    }
}