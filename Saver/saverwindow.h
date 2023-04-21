#ifndef SAVERWINDOW_H
#define SAVERWINDOW_H

#include "../GraphGlData/generaldata.h"

#include <QWidget>
#include <QString>
#include <QList>

struct SaverDataHandler {
    size_t first_sample, last_sample;
    QList<size_t> channels;
    bool success = false;
};

class SaverWindow : public QWidget
{
    Q_OBJECT
public:
    SaverWindow() = default;
    ~SaverWindow() = default;

    static SaverDataHandler openWindow(std::shared_ptr<GeneralData> data);
    static SaverDataHandler openWindow(std::shared_ptr<GeneralData> data, const size_t first_sample, const size_t last_sample);
private:
    static SaverDataHandler openWindow(const QList<QString>& names, const size_t names_count, const size_t first_sample, const size_t last_sample);
};

#endif // SAVERWINDOW_H
