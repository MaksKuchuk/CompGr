#ifndef ASDASD_SAVER_H
#define ASDASD_SAVER_H

#include "../Parser/ParseData.hpp"
#include "../Saver/SaveHandler.hpp"
#include <QString>
#include <vector>

class Saver {
public:
    static void TxtSaver(ParseData *data, const QString& path_to_file);
    static void TxtSaver(ParseData *data, const size_t first, const size_t last, const std::vector<size_t>& channels, const QString& path_to_file);
    static void TxtSaver(SaveHandler *data, const QString& path_to_file);
    static void TxtSaver(SaveHandler *data, const size_t first, const size_t last, const std::vector<size_t>& channels, const QString& path_to_file);
};

#endif //ASDASD_SAVER_H
