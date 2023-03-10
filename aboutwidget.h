#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>

namespace Ui {
class AboutWidget;
}

class AboutWidget : public QWidget
{
    Q_OBJECT

public:
    std::string nm = "AboutWidget";

    explicit AboutWidget(QWidget *parent = nullptr);
    ~AboutWidget();

private:
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
