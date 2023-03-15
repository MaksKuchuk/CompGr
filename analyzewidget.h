#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class AnalyzeWidget;
}

class AnalyzeWidget : public QWidget
{
    Q_OBJECT
    QVBoxLayout *layout;

public:
    explicit AnalyzeWidget(QWidget *parent = nullptr);
    ~AnalyzeWidget();

    void closeEvent(QCloseEvent *event) override;

private slots:


private:
    Ui::AnalyzeWidget *ui;

    friend class AnalysisWindowHandler;
};

#endif // ANALYZEWIDGET_H
