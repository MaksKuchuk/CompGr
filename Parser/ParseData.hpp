#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <tuple>
#include <QString>
#include <filesystem>
#include "../GraphGlData/generaldata.h"

class ParseData : public GeneralData {
protected:
    void setDuration(double totalSeconds_) {
        days = floor(totalSeconds_ / (60 * 60 * 24));
        totalSeconds_ -= days * (60 * 60 * 24);
 
        hours = floor(totalSeconds_ / (60 * 60));
        totalSeconds_ -= hours * (60 * 60);
 
        minutes = floor(totalSeconds_ / 60);
        totalSeconds_ -= minutes * 60;
 
        seconds = totalSeconds_;
    }

public:
    virtual ~ParseData() = default;

    virtual void parse(const std::filesystem::path &path_to_file) {};
};

#endif
