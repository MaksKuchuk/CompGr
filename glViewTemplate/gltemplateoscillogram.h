#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>

namespace Ui {
class glTemplateOscillogram;
}

class glTemplateOscillogram : public QWidget
{
    Q_OBJECT

public:
    explicit glTemplateOscillogram(QWidget *parent = nullptr);
    ~glTemplateOscillogram();

private:
    Ui::glTemplateOscillogram *ui;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
