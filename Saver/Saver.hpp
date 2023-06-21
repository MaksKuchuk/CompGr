#ifndef ASDASD_SAVER_H
#define ASDASD_SAVER_H

#include "../GraphGlData/generaldata.h"
#include <QString>
#include <QList>
#include <vector>

class Saver {
public:
    static void TxtSaver(std::shared_ptr<GeneralData> data, const QString& path_to_file);
    static void TxtSaver(std::shared_ptr<GeneralData> data, const size_t first,
                         const size_t last, const QList<size_t>& channels, const QString& path_to_file);
};

#endif //ASDASD_SAVER_H
