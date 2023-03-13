#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>

namespace Ui {
class AnalyzeWidget;
}

class AnalyzeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzeWidget(QWidget *parent = nullptr);
    ~AnalyzeWidget();

    void closeEvent(QCloseEvent *event) override;

private slots:


private:
    Ui::AnalyzeWidget *ui;
};

#endif // ANALYZEWIDGET_H
