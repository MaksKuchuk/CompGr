#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <QWidget>

namespace Ui {
class GraphTemplate;
}

class GraphTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit GraphTemplate(QString name ,QWidget *parent = nullptr);
    ~GraphTemplate();

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
