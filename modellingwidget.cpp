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
#include <QMdiSubWindow>

#include "Modeling/modeling.h"
#include "Modeling/randommodeling.h"
#include "Utility/generaldialog.h"

#include <QDebug>


ModellingWidget::ModellingWidget(QWidget *parent, std::shared_ptr<GeneralData> generalData)
: QWidget(parent),
  ui(new Ui::ModellingWidget),
  inputLines(ModellingWidget::InputLines(this, generalData)),
  amoutOfSamples(generalData == nullptr ? 10 : generalData->amountOfSamples),
  Hz(generalData == nullptr ? 1 : generalData->Hz)
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
    isAddToCurrent->setChecked(false);
    isAddToCurrent->setText("Add to current signals");
    if (MainWindow::grWid == nullptr) {
        isAddToCurrent->setCheckable(false);
        isAddToCurrent->setDisabled(true);
    } else {
        isAddToCurrent->setChecked(true);
    }
    CurrentCheckChanged();

    ChangedModel();
    form->addWidget(isAddToCurrent);

    form->addItem(inputForm);


    QDialogButtonBox *btn_box = new QDialogButtonBox(this);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addWidget(btn_box);

    this->setLayout(form);


    connect(dropDown,&QComboBox::currentIndexChanged, this, &ModellingWidget::ChangedModel);
    connect(isAddToCurrent, &QCheckBox::stateChanged, this, &ModellingWidget::CurrentCheckChanged);
    connect(btn_box, &QDialogButtonBox::rejected, this, &ModellingWidget::closeWidget);
    connect(btn_box, &QDialogButtonBox::accepted, this, &ModellingWidget::saveModel);

    auto connectDraw = [&](QPointer<QLineEdit> line){
        connect(line, &QLineEdit::textChanged, this, &ModellingWidget::DrawGraph);
    };
    connectDraw(inputLines._amountOfSamples);
    connectDraw(inputLines._delay);

    connect(inputLines._timeStep, &QLineEdit::textChanged, this, &ModellingWidget::TimeStepChanged);
    connect(inputLines._timeFreq, &QLineEdit::textChanged, this, &ModellingWidget::TimeFreqChanged);

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
        newRow("Time freq", inputLines._timeFreq);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. width", inputLines._scale2);
        newRow("Freq", inputLines._freq1);
        newRow("Phase", inputLines._phase);
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        newRow("Time step", inputLines._timeStep);
        newRow("Time freq", inputLines._timeFreq);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. freq", inputLines._freq1);
        newRow("Carry freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        newRow("Time step", inputLines._timeStep);
        newRow("Time freq", inputLines._timeFreq);
        newRow("Amplitude", inputLines._scale1);
        newRow("Env. freq", inputLines._freq1);
        newRow("Carry freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        newRow("Module index (0-1)", inputLines._scale2);
        break;
    }
    case Modeling::Type::LFM: {
        newRow("Time step", inputLines._timeStep);
        newRow("Time freq", inputLines._timeFreq);
        newRow("Amplitude", inputLines._scale1);
        newRow("Start freq", inputLines._freq1);
        newRow("End freq", inputLines._freq2);
        newRow("Phase", inputLines._phase);
        break;
    }
    case Modeling::Type::WhiteNoise: {
        newRow("Time step", inputLines._timeStep);
        newRow("Time freq", inputLines._timeFreq);
        newRow("From", inputLines._scale1);
        newRow("To", inputLines._scale2);
        break;
    }
    default:
        break;
    }

    if (MainWindow::modelWindow != nullptr) {
        MainWindow::modelWindow->resize(MainWindow::modelWindow->sizeHint().width(),
                                        210 + 18*inputForm->rowCount());
    }
    DrawGraph();
}

void ModellingWidget::DrawGraph() {
    switch (currentType) {
    case Modeling::Type::SingleImpulse: {
        data = Modeling::delayedSingleImpulse(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::SingleHop: {
        data = Modeling::delayedSingleHop(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::DecreasingExponent: {
        data = Modeling::sampledDecreasingExponent(inputLines.samples(), inputLines.scale1());
        break;
    }
    case Modeling::Type::SineWave: {
        data = Modeling::sampledSineWave(inputLines.samples(), inputLines.scale1(), inputLines.scale2(), inputLines.phase());
        break;
    }
    case Modeling::Type::Meander: {
        data = Modeling::meander(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::Saw: {
        data = Modeling::saw(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::ExponentialEnvelope: {
        data = Modeling::exponentialEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
                                             inputLines.scale2(), inputLines.freq1(), inputLines.phase());
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        data = Modeling::balanceEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
                                         inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        data = Modeling::tonalEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
                                       inputLines.freq1(), inputLines.freq2(), inputLines.phase(),
                                       inputLines.scale2());
        break;
    }
    case Modeling::Type::LFM: {
        data = Modeling::LFM(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
                             inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    case Modeling::Type::WhiteNoise: {
        data = randomModeling::whiteNoise(inputLines.samples(), inputLines.timeStep(),
                                          inputLines.scale1(), inputLines.scale2());
        break;
    }
    default:
        break;
    }
    delete graphTemplate;
    graphTemplate = new GraphTemplate(this, data, false);
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
    for (qint64 i = inputForm->rowCount() - 1; i >= 0 ; --i) {
        inputForm->takeRow(0);
    }
    for (qint64 i = labels.size() - 1; i >= 0; --i) {
        labels[i]->close();
    }
}

void ModellingWidget::CurrentCheckChanged() {
    if (isAddToCurrent->isChecked()) {
        inputLines._amountOfSamples->setText(QString::number(amoutOfSamples));
        inputLines._amountOfSamples->setDisabled(true);
        inputLines._timeFreq->setText(QString::number(Hz));
        inputLines._timeStep->setText(QString::number(1/Hz));

        inputLines._timeFreq->setDisabled(true);
        inputLines._timeStep->setDisabled(true);
    } else {
        inputLines._amountOfSamples->setDisabled(false);
        inputLines._timeFreq->setDisabled(false);
        inputLines._timeStep->setDisabled(false);
    }
}

void ModellingWidget::TimeStepChanged() {
    if (timeStepConverting)
        return;

    timeStepConverting = true;
    inputLines._timeFreq->setText( QString::number(1/inputLines.timeStep()) );
    timeStepConverting = false;
    DrawGraph();
}

void ModellingWidget::TimeFreqChanged() {
    if (timeStepConverting)
        return;

    timeStepConverting = true;
    inputLines._timeStep->setText( QString::number(1/inputLines.timeFreq()) );
    timeStepConverting = false;
    DrawGraph();
}

ModellingWidget::InputLines::InputLines(QWidget* parent, std::shared_ptr<GeneralData> data) {
    _amountOfSamples = new QLineEdit(QString::number( data == nullptr ? 10 : data->amountOfSamples), parent);

    _timeStep = new QLineEdit(QString::number( data == nullptr ? 1 : 1/data->Hz), parent);
    _timeFreq = new QLineEdit(QString::number( data == nullptr ? 1 : data->Hz), parent);
    _delay = new QLineEdit(QString::number(0), parent);
    _scale1 = new QLineEdit(QString::number(1), parent);
    _scale2 = new QLineEdit(QString::number(1), parent);
    _phase = new QLineEdit(QString::number(0), parent);
    _freq1 = new QLineEdit(QString::number(1), parent);
    _freq2 = new QLineEdit(QString::number(1), parent);
    Hide();
}


void ModellingWidget::closeEvent(QCloseEvent *event) {
    MainWindow::modelWid.clear();
}

void ModellingWidget::closeWidget() {
    this->parentWidget()->close();
}

void ModellingWidget::DisableToggle() {
    isAddToCurrent->setCheckable(false);
    isAddToCurrent->setDisabled(true);
}

void ModellingWidget::saveModel() {
    QString dialog_text = "Save model";
    if (MainWindow::grWid != nullptr)
        dialog_text += isAddToCurrent->isChecked() ?
                    " and add to current signals" :
                    " and close old signals";
    dialog_text += "?";
    auto dialog = GeneralDialog::AgreeDialog(dialog_text);
    if (!dialog)
        return;

//    data->source = "Modelling";
    if (MainWindow::grWid == nullptr || !isAddToCurrent->isChecked()) {
        MainWindow::instance->ShowGraphWidget(std::make_shared<GeneralData>(data, data->name + "1"));
        MainWindow::instance->grWid->graphData->modellingCounts[(int)currentType] += 1;

        isAddToCurrent->setDisabled(false);
        isAddToCurrent->setCheckable(true);
    }
    else if (MainWindow::grWid != nullptr) {
        MainWindow::grWid->AddNewChannel(data,
                                         data->name + QString::number( MainWindow::instance->grWid->graphData->modellingCounts[(int)currentType] += 1));
        MainWindow::grWindow->setFixedSize(300, 100 * MainWindow::grWid->graphData->getAmountOfChannels());
    }

    isAddToCurrent->setChecked(true);

//    closeWidget();
}
