#include "utility.h"

QList<double> Utility::StringToDList(const QString& str, const QString& sep) {
    QList<double> dList;
    foreach(QString d, str.split(sep)) {
        dList.push_back(d.toDouble());
    }
    return dList;
}
