#ifndef PARSEDATACSV_H
#define PARSEDATACSV_H

#include "ParseDataTxt.hpp"
#include <vector>

class ParseDataCsv : public ParseDataTxt
{
    std::vector<char> delimiters = {'\t', ';', ','};
    void setDelimiters(char);
public:
    ParseDataCsv();

    void parseHeader(QList<QByteArray>& str);
    void parseTime();

    void parse(const std::filesystem::path &path_to_file) override;
};

#endif // PARSEDATACSV_H
