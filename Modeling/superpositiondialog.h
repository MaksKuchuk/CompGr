#ifndef SUPERPOSITIONDIALOG_H
#define SUPERPOSITIONDIALOG_H

#include <QObject>
#include <QWidget>
#include "../GraphGlData/Graph2DData.hpp"
#include "../GraphGlData/generaldata.h"

class SuperpositionDialog : public QWidget
{
    Q_OBJECT
public:
    SuperpositionDialog() = default;

    static void LinSupDialog(std::shared_ptr<GeneralData> data);
    static void MultSupDialog(std::shared_ptr<GeneralData> data);

signals:

};

#endif // SUPERPOSITIONDIALOG_H
