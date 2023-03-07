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
    double maxVal;
    double minVal;

public:
    explicit GraphTemplate(QWidget *parent = nullptr,
                           std::string name = "",
                           long long parNum = 0,
                           double* data = nullptr,
                           double maxVal = 0,
                           double minVal = 0
                           );
    ~GraphTemplate();

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
