#include "ParseDataTxt.hpp"

#ifndef PARSER_H
#define PARSER_H

class Parser {
public:
    static ParseData *parse(const std::string &path_to_file) {
        std::ifstream file_to_parse(path_to_file);
 
        unsigned long long dotPos = path_to_file.rfind('.');
        std::string extension = path_to_file.substr(dotPos + 1, path_to_file.size() - 1);
 
        if (extension == "txt") {
            auto pd = new ParseDataTxt();
            pd->parse(file_to_parse);
            return static_cast<ParseData *>(pd);
        }
        return nullptr;
    }
};

#endif
