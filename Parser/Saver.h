#ifndef ASDASD_SAVER_H
#define ASDASD_SAVER_H

#include "ParseData.hpp"
#include <string>

class Saver {
public:
    static void TxtSaver(ParseData* data, const std::string& path_to_file);
};

#endif //ASDASD_SAVER_H
