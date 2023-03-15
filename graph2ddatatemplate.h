#ifndef GRAPH2DDATATEMPLATE_H
#define GRAPH2DDATATEMPLATE_H

#include <QWidget>

namespace Ui {
class Graph2DDataTemplate;
}

class Graph2DDataTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit Graph2DDataTemplate(QWidget *parent = nullptr);
    ~Graph2DDataTemplate();

private:
    Ui::Graph2DDataTemplate *ui;
};

#endif // GRAPH2DDATATEMPLATE_H
