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
  inputLines(ModellingWidget::InputLines(this, generalData)),
  amoutOfSamples(generalData == nullptr ? 10 : generalData->amountOfSamples)
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

    isAddToCurrent = new QCheckBox(this);
    isAddToCurrent->setCheckState(Qt::CheckState::Unchecked);
    if (MainWindow::grWid == nullptr) {
        isAddToCurrent->setCheckable(false);
        isAddToCurrent->setDisabled(true);
//        isAddToCurrent->hide();
    }
    isAddToCurrent->setText("Add to current signals");

    ChangedModel();
    form->addWidget(isAddToCurrent);

    form->addItem(inputForm);


    QDialogButtonBox *btn_box = new QDialogButtonBox(this);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addWidget(btn_box);

    this->setLayout(form);


    connect(dropDown,&QComboBox::currentIndexChanged, this, &ModellingWidget::ChangedModel);
    connect(isAddToCurrent, &QCheckBox::stateChanged, this, &ModellingWidget::CurrentCheckChanged);

    auto connectDraw = [&](QPointer<QLineEdit> line){
        connect(line, &QLineEdit::textChanged, this, &ModellingWidget::DrawGraph);
    };
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
    currentType = (Modeling::Type)dropDown->currentIndex();
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
        break;
    }
    case Modeling::Type::ExponentialEnvelope: {
        newRow("Time step", inputLines._timeStep);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. width", inputLines._scale2);
        newRow("Freq", inputLines._freq1);
        newRow("Phase", inputLines._phase);
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        newRow("Time step", inputLines._timeStep);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. freq", inputLines._freq1);
        newRow("Carry freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        newRow("Time step", inputLines._timeStep);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. freq", inputLines._freq1);
        newRow("Carry freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        newRow("Module index (0-1)", inputLines._scale2);
        break;
    }
    case Modeling::Type::LFM: {
        newRow("Time step", inputLines._timeStep);
        newRow("Amplitude", inputLines._scale1);
        newRow("Start freq", inputLines._freq1);
        newRow("End freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        break;
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
    auto a_samples = inputLines.samples();
    auto delay0 = inputLines.delay();
    auto delay = delay0 < a_samples ? delay0 : a_samples-1;
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
        data = Modeling::meander(a_samples, delay0);
        break;
    }
    case Modeling::Type::Saw: {
        data = Modeling::saw(a_samples, delay0);
        break;
    }
    case Modeling::Type::ExponentialEnvelope: {
        data = Modeling::exponentialEnvelope(a_samples, inputLines.timeStep(), inputLines.scale1(),
                                            inputLines.scale2(), inputLines.freq1(), inputLines.phase());
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        data = Modeling::balanceEnvelope(a_samples, inputLines.timeStep(), inputLines.scale1(),
                                         inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        data = Modeling::tonalEnvelope(a_samples, inputLines.timeStep(), inputLines.scale1(),
                                       inputLines.freq1(), inputLines.freq2(), inputLines.phase(),
                                       inputLines.scale2());
        break;
    }
    case Modeling::Type::LFM: {
        data = Modeling::LFM(a_samples, inputLines.timeStep(), inputLines.scale1(),
                                       inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    default:
        break;
    }
    delete graphTemplate;
    graphTemplate = new GraphTemplate(this, data);
//    gv->setFixedHeight(60);
//    gv->setFixedWidth(300);
    form->setWidget(1, QFormLayout::FieldRole, graphTemplate);

    this->update();
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

void ModellingWidget::CurrentCheckChanged() {
    if (isAddToCurrent->isChecked()) {
        inputLines._amountOfSamples->setText(QString::number(amoutOfSamples));
        inputLines._amountOfSamples->setDisabled(true);
    } else {
        inputLines._amountOfSamples->setDisabled(false);
    }
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


void ModellingWidget::closeEvent(QCloseEvent *event) {
    MainWindow::isModelling = false;
}
