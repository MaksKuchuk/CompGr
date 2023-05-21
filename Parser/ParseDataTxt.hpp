#ifndef PARSEDATATXT_H
#define PARSEDATATXT_H

#include "ParseData.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <thread>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QDateTime>

#include <QDebug>

class ParseDataTxt : public ParseData {
protected:
    void setChannelsNames(const QString &string_names);

    static void getData(QTextStream &file, auto &val);

    static void getData(QTextStream &file, QString &val);

    static void getVal(unsigned long long &val_to, const QString &str_from);

    static void getVal(double &val_to, const QString &str_from);

    static void setChannels(ParseDataTxt *data, const QList<QByteArray>& list,
                            qint64 first, qint64 last, QList<std::pair<double, double>>* sub_extr);

    void threadsHandle(const QList<QByteArray>& list);

    qint64 parseGeneral(QTextStream &file_to_parse);

public:
    void parse(const std::filesystem::path &path_to_file) override;
};

#endif
