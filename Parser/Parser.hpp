#ifndef PARSER_H
#define PARSER_H

#include "ParseDataTxt.hpp"
#include "ParseDataWav.hpp"
#include <QString>
#include <QFile>
#include <filesystem>

#include <chrono>
#include <QDebug>

class Parser {
    static ParseData* parseFile(const QString& path_to_file_str) {
        std::filesystem::path path_to_file(path_to_file_str.toStdWString());

        if (!QFile::exists(path_to_file)) {
            throw std::runtime_error("file not opened: " + path_to_file_str.toStdString());
        }

        if (path_to_file_str.endsWith("txt")) {
            auto pd = new ParseDataTxt();
            pd->parse(path_to_file);
            return static_cast<ParseData*>(pd);
        }
        else if (path_to_file_str.endsWith("wav")) {
            auto pd = new ParseDataWav();
            pd->parse(path_to_file);
            return static_cast<ParseData*>(pd);
        }
        return nullptr;
    }
public:
    static ParseData *parse(const QString& path_to_file) {
        auto s = std::chrono::high_resolution_clock().now();
        auto pd = parseFile(path_to_file);
        if (pd == nullptr)
            return nullptr;

        auto namePos = path_to_file.lastIndexOf('/');
        auto name = path_to_file.sliced(namePos+1);
        pd->setName(name);

        auto e = std::chrono::high_resolution_clock().now();
        qDebug() << name << duration_cast<std::chrono::milliseconds>(e - s).count() << " ms";

        return pd;
    }
};

#endif
