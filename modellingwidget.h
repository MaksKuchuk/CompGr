#ifndef MODELLINGWIDGET_H
#define MODELLINGWIDGET_H

#include <QWidget>
#include "GraphGlData/Graph2DData.hpp"
#include "GraphGlData/generaldata.h"
#include "glview.h"
#include "ui_modellingwidget.h"
#include "Modeling/modeling.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

namespace Ui {
class ModellingWidget;
}

class ModellingWidget : public QWidget
{
    Q_OBJECT
    class InputLines
    {
    public:
        InputLines(QWidget* parent, std::shared_ptr<GeneralData> data);

        QPointer<QLineEdit> _amountOfSamples;
        QPointer<QLineEdit> _timeStep;
        QPointer<QLineEdit> _delay;
        QPointer<QLineEdit> _scale1;
        QPointer<QLineEdit> _scale2;
        QPointer<QLineEdit> _phase;
        QPointer<QLineEdit> _freq1;
        QPointer<QLineEdit> _freq2;

        void Disconnect() {
            _amountOfSamples->disconnect();
            _timeStep->disconnect();
            _delay->disconnect();
            _scale1->disconnect();
            _scale2->disconnect();
            _phase->disconnect();
            _freq1->disconnect();
            _freq2->disconnect();
        }

        void Hide() {
            _amountOfSamples->setVisible(false);
            _timeStep->setVisible(false);
            _delay->setVisible(false);
            _scale1->setVisible(false);
            _scale2->setVisible(false);
            _phase->setVisible(false);
            _freq1->setVisible(false);
            _freq2->setVisible(false);
        }

        size_t samples() {
            return _amountOfSamples->text().toULongLong();
        }
        size_t timeStep() {
            return _timeStep->text().toULongLong();
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
    };
public:
    const std::string nm = "ModellingWidget";

    std::shared_ptr<Graph2DData> data = nullptr;

    ModellingWidget(QWidget* parent, std::shared_ptr<GeneralData> generalData = nullptr);
    ~ModellingWidget() = default;

    QPointer<glView> gv = nullptr;
    QPointer<QFormLayout> form = nullptr;
    QPointer<QComboBox> dropDown = nullptr;
    InputLines inputLines;

    QPointer<QFormLayout> inputForm = nullptr;
    QList<QPointer<QLabel>> labels;

    Modeling::Type currentType = Modeling::Type::Saw;

    void ChangedModel();
    void ChangeSamples();
    void DrawGraph();

    void Disconnect();

private:
    Ui::ModellingWidget *ui;

    void newRow(QString str, QPointer<QLineEdit> line);
    void inputFormRemoveRows();
};

#endif // MODELLINGWIDGET_H
