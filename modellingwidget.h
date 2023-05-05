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

        void Hide() {
            _amountOfSamples->setVisible(false);
            _timeStep->setVisible(false);
            _timeFreq->setVisible(false);
            _delay->setVisible(false);
            _scale1->setVisible(false);
            _scale2->setVisible(false);
            _phase->setVisible(false);
            _freq1->setVisible(false);
            _freq2->setVisible(false);
            _line1->setVisible(false);
            _line2->setVisible(false);
        }

        size_t samples() {
            return _amountOfSamples->text().toULongLong();
        }
        double timeStep() {
            return _timeStep->text().toDouble();
        }
        double timeFreq() {
            return _timeFreq->text().toDouble();
        }
        size_t delay() {
            return _delay->text().toULongLong();
        }
        double scale1() {
            return _scale1->text().toDouble();
        }
        double scale2() {
            return _scale2->text().toDouble();
        }
        double phase() {
            return _phase->text().toDouble();
        }
        double freq1() {
            return _freq1->text().toDouble();
        }
        double freq2() {
            return _freq2->text().toDouble();
        }
        QString line1() {
            return _line1->text();
        }
        QString line2() {
            return _line2->text();
        }
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
};

#endif // MODELLINGWIDGET_H
