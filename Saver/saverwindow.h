#ifndef SAVERWINDOW_H
#define SAVERWINDOW_H

#include "../Parser/ParseData.hpp"

#include <QWidget>
#include <QString>
#include <vector>

struct SaverDataHandler {
    size_t first_sample, last_sample;
    std::vector<size_t> channels;
};

class SaverWindow : public QWidget
{
    Q_OBJECT
public:
    SaverWindow();

    static SaverDataHandler openWindow(const ParseData* data);
    static SaverDataHandler openWindow(const ParseData* data, const size_t first_sample, const size_t last_sample);
private:
    static SaverDataHandler openWindow(const QString* names, const size_t names_count, const size_t first_sample, const size_t last_sample);
};

#endif // SAVERWINDOW_H
