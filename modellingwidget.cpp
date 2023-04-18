#include "modellingwidget.h"
#include "ui_graphwidget.h"
#include "GraphGlData/Graph2DData.hpp"
#include <QVBoxLayout>
#include "mainwindow.h"
#include <QLineEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLayoutItem>
#include <QTextItem>

#include "glview.h"

#include "Modeling/modeling.h"

#include "Parser/Parser.hpp"
#include "Parser/ParseData.hpp"

#include <QDebug>


ModellingWidget::ModellingWidget(QWidget *parent, std::shared_ptr<GeneralData> generalData)
: QWidget(parent),
  ui(new Ui::ModellingWidget),
  inputLines(ModellingWidget::InputLines(this, generalData))
{
    ui->setupUi(this);

    form = new QFormLayout();
    inputForm = new QFormLayout();
//    layout->setContentsMargins(0, 0, 0, 0);

    dropDown = new QComboBox();
    for (size_t i = 0; i < Modeling::AmountOfTypes(); ++i)
        dropDown->addItem(Modeling::TypeToString(Modeling::Type(i)));
    dropDown->setCurrentIndex((int)currentType);

    form->addWidget(dropDown);

    ChangedModel();

    form->addItem(inputForm);


    QDialogButtonBox *btn_box = new QDialogButtonBox(this);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addWidget(btn_box);

    this->setLayout(form);



    auto connectDraw = [&](QPointer<QLineEdit> line){
        connect(line, &QLineEdit::editingFinished, this, &ModellingWidget::DrawGraph);
    };

    connect(dropDown,&QComboBox::currentIndexChanged, this, &ModellingWidget::ChangedModel);

    connectDraw(inputLines._amountOfSamples);
    connectDraw(inputLines._delay);
    connectDraw(inputLines._timeStep);
    connectDraw(inputLines._scale1);
    connectDraw(inputLines._scale2);
    connectDraw(inputLines._phase);
    connectDraw(inputLines._freq1);
    connectDraw(inputLines._freq2);
}

void ModellingWidget::ChangedModel() {
    size_t index = dropDown->currentIndex();
    qDebug() << index;
    currentType = (Modeling::Type)index;
//    inputLines.Disconnect();
    inputLines.Hide();
    inputFormRemoveRows();

    newRow("No. samples", inputLines._amountOfSamples);
    switch (currentType) {
    case Modeling::Type::SingleImpulse:
    case Modeling::Type::SingleHop:{
        newRow("Delay", inputLines._delay);
        break;
    }
    case Modeling::Type::DecreasingExponent: {
        newRow("Base", inputLines._scale1);
        break;
    }
    case Modeling::Type::Saw:
    case Modeling::Type::Meander: {
        newRow("Period", inputLines._delay);
        break;
    }
    case Modeling::Type::SineWave: {
        newRow("Amplitude", inputLines._scale1);
        newRow("Circle phase", inputLines._scale2);
        newRow("Phase", inputLines._phase);
    }
    default:
        break;
    }

    DrawGraph();
}

void ModellingWidget::ChangeSamples() {
    DrawGraph();
}

void ModellingWidget::DrawGraph() {

    qDebug() << inputLines.samples();
    auto a_samples = inputLines.samples();
    auto delay = inputLines.delay();
    delay = delay < a_samples ? delay : a_samples-1;
    switch (currentType) {
    case Modeling::Type::SingleImpulse: {
        data = Modeling::delayedSingleImpulse(a_samples, delay);
        break;
    }
    case Modeling::Type::SingleHop: {
        data = Modeling::delayedSingleHop(a_samples, delay);
        break;
    }
    case Modeling::Type::DecreasingExponent: {
        data = Modeling::sampledDecreasingExponent(a_samples, inputLines.scale1());
        break;
    }
    case Modeling::Type::SineWave: {
        data = Modeling::sampledSineWave(a_samples, inputLines.scale1(), inputLines.scale2(), inputLines.phase());
        break;
    }
    case Modeling::Type::Meander: {
        data = Modeling::meander(a_samples, delay);
        break;
    }
    case Modeling::Type::Saw: {
        data = Modeling::saw(a_samples, delay);
        break;
    }
    default:
        break;
    }
    delete gv;
    gv = new glView(this, data->amountOfSamples, data->samples, data->maxVal, data->minVal);
    gv->setFixedHeight(60);
    gv->setFixedWidth(300);
    form->setWidget(1, QFormLayout::FieldRole, gv);

    inputForm->update();
    this->update();
    QCoreApplication::processEvents();
}


void ModellingWidget::newRow(QString str, QPointer<QLineEdit> line) {
    line->setVisible(true);
    auto label = new QLabel(str, this);
    inputForm->addWidget(label);
    label->show();
    labels.append(label);
    inputForm->addWidget(line);
}

void ModellingWidget::inputFormRemoveRows() {
    qDebug() << inputForm->rowCount();
    for (size_t i = 0; i < inputForm->rowCount(); ++i) {
        inputForm->takeRow(0);
    }
    for (const auto& l : labels)
        l->close();
}


ModellingWidget::InputLines::InputLines(QWidget* parent, std::shared_ptr<GeneralData> data) {
    _amountOfSamples = new QLineEdit(QString::number( data == nullptr ? 10 : data->amountOfSamples), parent);

    _timeStep = new QLineEdit(QString::number( data == nullptr ? 1 : 1/data->Hz), parent);
    _delay = new QLineEdit(QString::number(0), parent);
    _scale1 = new QLineEdit(QString::number(1), parent);
    _scale2 = new QLineEdit(QString::number(1), parent);
    _phase = new QLineEdit(QString::number(0), parent);
    _freq1 = new QLineEdit(QString::number(1), parent);
    _freq2 = new QLineEdit(QString::number(1), parent);
    Hide();
}
