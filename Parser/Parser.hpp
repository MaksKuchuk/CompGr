#include "ParseDataTxt.hpp"
#include "ParseDataWav.hpp"

#ifndef PARSER_H
#define PARSER_H

class Parser {
    static ParseData* parseFile(const std::string& path_to_file) {
        std::ifstream file_to_parse(path_to_file);

        unsigned long long dotPos = path_to_file.rfind('.');
        std::string extension = path_to_file.substr(dotPos + 1, path_to_file.size() - 1);

        if (extension == "txt") {
            auto pd = new ParseDataTxt();
            pd->parse(file_to_parse);
            return static_cast<ParseData*>(pd);
        }
        else if (extension == "wav") {
            auto pd = new ParseDataWav();
            pd->parse(file_to_parse);
            return static_cast<ParseData*>(pd);
        }
        return nullptr;
    }
public:
    static ParseData *parse(const std::string &path_to_file) {
        auto pd = parseFile(path_to_file);
        if (pd == nullptr)
            return nullptr;

        unsigned long long namePos = path_to_file.rfind('/');
        std::string fileName = path_to_file.substr(namePos + 1, path_to_file.size() - 1);

        pd->setName(fileName);
        return pd;
    }
};

#endif