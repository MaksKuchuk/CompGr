#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QKeyEvent>

namespace Ui {
class AnalyzeWidget;
}

class AnalyzeWidget : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout *layout;

    explicit AnalyzeWidget(QWidget *parent = nullptr);
    ~AnalyzeWidget();

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::AnalyzeWidget *ui;

    friend class AnalysisWindowHandler;
};

#endif // ANALYZEWIDGET_H
