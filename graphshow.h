#ifndef GRAPHSHOW_H
#define GRAPHSHOW_H

#include <QWidget>

namespace Ui {
class GraphShow;
}

class GraphShow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphShow(QWidget *parent = nullptr);
    ~GraphShow();

private:
    Ui::GraphShow *ui;
};

#endif // GRAPHSHOW_H
