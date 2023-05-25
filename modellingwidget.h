#ifndef MODELLINGWIDGET_H
#define MODELLINGWIDGET_H

#include <QWidget>
#include "GraphGlData/Graph2DData.hpp"
#include "GraphGlData/generaldata.h"
#include "glview.h"
#include "ui_modellingwidget.h"
#include "Modeling/modeling.h"
#include "graphtemplate.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QCloseEvent>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
class ModellingWidget;
}

class ModellingWidget : public QWidget
{
    Q_OBJECT
    class InputLines
    {
    public:
        InputLines(QWidget* parent, std::shared_ptr<GeneralData> data, Modeling::Type type);

        QPointer<QLineEdit> _amountOfSamples;
        QPointer<QLineEdit> _timeStep;
        QPointer<QLineEdit> _timeFreq;
        QPointer<QLineEdit> _delay;
        QPointer<QLineEdit> _scale1;
        QPointer<QLineEdit> _scale2;
        QPointer<QLineEdit> _phase;
        QPointer<QLineEdit> _freq1;
        QPointer<QLineEdit> _freq2;
        QPointer<QLineEdit> _line1;
        QPointer<QLineEdit> _line2;
        QPointer<QPushButton> _ARMA_button;

        QList<QString> _ARMA_inputs = {
            "0.68, 0.088",
            "-1.656, 0.888",
            "-1.944, 0.976",
            "0.744, 0.96",
            "1.613, 0.787",

            "-2.34, 2.733, -2.148, 0.863",
            "-1.12, 0.592",

            "-4.167, 7.940, -9.397, 7.515, -3.752, 0.862",
            "-2.28, 1.77, -0.472"
        };

        void Hide();

        size_t samples();
        double timeStep();
        double timeFreq();
        size_t delay();
        double scale1();
        double scale2();
        double phase();
        double freq1();
        double freq2();
        QString line1();
        QString line2();
    };

    size_t amoutOfSamples = 100;
    double Hz = 1;
public:
    const std::string nm = "ModellingWidget";

    std::shared_ptr<Graph2DData> data = nullptr;

    ModellingWidget(QWidget* parent, std::shared_ptr<GeneralData> generalData = nullptr, Modeling::Type type = Modeling::Type::SingleImpulse);
    ~ModellingWidget() = default;

    QPointer<glView> gv = nullptr;
    QPointer<GraphTemplate> graphTemplate = nullptr;
    QPointer<QFormLayout> form = nullptr;
    QPointer<QComboBox> dropDown = nullptr;
    QPointer<QCheckBox> isAddToCurrent = nullptr;
    QPointer<QLabel> formulaLabel = nullptr;
    InputLines inputLines;

    QPointer<QFormLayout> inputForm = nullptr;
    QList<QPointer<QLabel>> labels;

    Modeling::Type currentType = Modeling::Type::SingleImpulse;

    void DisableToggle();

private:
    void ChangedModel();
    void DrawGraph();

    bool timeStepConverting = false;
    void TimeStepChanged();
    void TimeFreqChanged();

    void CurrentCheckChanged();
    void closeEvent(QCloseEvent *event);
    void closeWidget();

    void saveModel();

    Ui::ModellingWidget *ui;

    void newRow(QString str, QPointer<QLineEdit> line);
    void inputFormRemoveRows();

    void ARMA_Choose();
};

#endif // MODELLINGWIDGET_H
