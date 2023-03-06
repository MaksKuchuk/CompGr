#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <QWidget>

namespace Ui {
class GraphTemplate;
}

class GraphTemplate : public QWidget
{
    Q_OBJECT
    std::string name;
    long long parNum;
    double* data;
    long long maxMod;

public:
    explicit GraphTemplate(QWidget *parent = nullptr,
                           std::string name = "",
                           long long parNum = 0,
                           double* data = nullptr,
                           long long maxMod = 0
                           );
    ~GraphTemplate();

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
