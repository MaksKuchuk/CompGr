#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <tuple>
#include <QString>
#include <filesystem>
#include "../GraphGlData/generaldata.h"

class ParseData : public GeneralData {
public:
    char delimiter = ' ';

    virtual ~ParseData() = default;

    virtual void parse(const std::filesystem::path &path_to_file) {};
};

#endif
