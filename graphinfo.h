#ifndef GRAPHINFO_H
#define GRAPHINFO_H

#include <QWidget>

namespace Ui {
class GraphInfo;
}

class GraphInfo : public QWidget
{
    Q_OBJECT

public:
    explicit GraphInfo(QWidget *parent = nullptr);
    ~GraphInfo();

private:
    Ui::GraphInfo *ui;
};

#endif // GRAPHINFO_H
