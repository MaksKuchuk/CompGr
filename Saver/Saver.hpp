#ifndef ASDASD_SAVER_H
#define ASDASD_SAVER_H

#include "../Parser/ParseData.hpp"
#include "../Saver/SaveHandler.hpp"
#include <QString>

class Saver {
public:
    static void TxtSaver(ParseData *data, const QString& path_to_file);
    static void TxtSaver(SaveHandler *data, const QString& path_to_file);
};

#endif //ASDASD_SAVER_H
