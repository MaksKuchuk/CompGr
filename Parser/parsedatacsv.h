#ifndef PARSEDATACSV_H
#define PARSEDATACSV_H

#include "ParseDataTxt.hpp"

class ParseDataCsv : public ParseDataTxt
{
public:
    ParseDataCsv();

    void parseHeader(QList<QByteArray>& str);
    void parseTime();

    void parse(const std::filesystem::path &path_to_file) override;
};

#endif // PARSEDATACSV_H
