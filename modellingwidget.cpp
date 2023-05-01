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
#include "Utility/utility.h"

#include <QtMath>

#include <QDebug>


ModellingWidget::ModellingWidget(QWidget *parent, std::shared_ptr<GeneralData> generalData, Modeling::Type type)
: QWidget(parent),
  ui(new Ui::ModellingWidget),
  currentType(type),
  inputLines(ModellingWidget::InputLines(this, generalData, type)),
  amoutOfSamples(generalData == nullptr ? 100 : generalData->amountOfSamples),
  Hz(generalData == nullptr ? 1 : generalData->Hz)
{
    ui->setupUi(this);

    form = new QFormLayout();
    inputForm = new QFormLayout();
//    layout->setContentsMargins(0, 0, 0, 0);

//    dropDown = new QComboBox();
//    for (size_t i = 0; i < Modeling::AmountOfTypes(); ++i)
//        dropDown->addItem(Modeling::TypeToString(Modeling::Type(i)));
//    dropDown->setCurrentIndex((int)currentType);

//    form->addWidget(dropDown);

    auto name = new QLabel(Modeling::TypeToString(currentType), this);
    auto nameFont = name->font();
    nameFont.setPixelSize(20);
    name->setFont(nameFont);
    name->setAlignment(Qt::AlignCenter);
    form->addWidget(name);

    formulaLabel = new QLabel(Modeling::TypeToFormula(currentType), this);
    formulaLabel->setAlignment(Qt::AlignCenter);
    form->addWidget(formulaLabel);

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


//    connect(dropDown,&QComboBox::currentIndexChanged, this, &ModellingWidget::ChangedModel);
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

    connect(inputLines._line1, &QLineEdit::editingFinished, this, &ModellingWidget::DrawGraph);
    connect(inputLines._line2, &QLineEdit::editingFinished, this, &ModellingWidget::DrawGraph);
}

void ModellingWidget::ChangedModel() {
//    currentType = (Modeling::Type)dropDown->currentIndex();
    inputLines.Hide();
    inputFormRemoveRows();

    newRow("No. samples (N)", inputLines._amountOfSamples);
    newRow("Time step", inputLines._timeStep);
    newRow("Time freq", inputLines._timeFreq);
    switch (currentType) {
    case Modeling::Type::SingleImpulse:
    case Modeling::Type::SingleHop:{
        newRow("Delay", inputLines._delay);
        break;
    }
    case Modeling::Type::DecreasingExponent: {
        newRow("Base (0 < a < 1)", inputLines._scale1);
        break;
    }
    case Modeling::Type::Saw:
    case Modeling::Type::Meander: {
        newRow("Period (L)", inputLines._delay);
        break;
    }
    case Modeling::Type::SineWave: {
        newRow("Amplitude (a)", inputLines._scale1);
        newRow("Circle phase (0 < w < pi)", inputLines._scale2);
        newRow("Phase (0 < p < 2pi)", inputLines._phase);
        break;
    }
    case Modeling::Type::ExponentialEnvelope: {
        newRow("Amplitude (a)", inputLines._scale1);
        newRow("Env. width (x) ", inputLines._scale2);
        newRow("Freq (0 < f < 0.5 time freq)", inputLines._freq1);
        newRow("Phase (p)", inputLines._phase);
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        newRow("Amplitude (a)", inputLines._scale1);
        newRow("Env. freq (fo)", inputLines._freq1);
        newRow("Carry freq (fn)", inputLines._freq2);
        newRow("Phase (p)", inputLines._phase);
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        newRow("Amplitude (a)", inputLines._scale1);
        newRow("Env. freq (fo)", inputLines._freq1);
        newRow("Carry freq (fn)", inputLines._freq2);
        newRow("Phase (p)", inputLines._phase);
        newRow("Module index (0 < m < 1)", inputLines._scale2);
        break;
    }
    case Modeling::Type::LFM: {
        newRow("Amplitude (a)", inputLines._scale1);
        newRow("Start freq (f0)", inputLines._freq1);
        newRow("End freq (fk)", inputLines._freq2);
        newRow("Phase (p)", inputLines._phase);
        break;
    }
    case Modeling::Type::UniformWhiteNoise: {
        newRow("From (a)", inputLines._scale1);
        newRow("To (b)", inputLines._scale2);
        break;
    }
    case Modeling::Type::NormalWhiteNoise: {
        newRow("Mean (a)", inputLines._scale1);
        newRow("Dispersion (d^2)", inputLines._scale2);
        break;
    }
    case Modeling::Type::AutoregressiveMovingAverage: {
        newRow("Dispersion (d^2)", inputLines._scale1);
        newRow("Array a", inputLines._line1);
        newRow("Array b", inputLines._line2);
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
    case Modeling::Type::UniformWhiteNoise: {
        data = randomModeling::whiteNoise(inputLines.samples(), inputLines.timeStep(),
                                          inputLines.scale1(), inputLines.scale2());
        break;
    }
    case Modeling::Type::NormalWhiteNoise: {
        data = randomModeling::normalDistrWhiteNoise(inputLines.samples(), inputLines.timeStep(),
                                                     inputLines.scale1(), inputLines.scale2());
        break;
    }
    case Modeling::Type::AutoregressiveMovingAverage: {
        data = randomModeling::ARMA(inputLines.samples(), inputLines.timeStep(),
                                    inputLines.scale1(),  Utility::StringToDList(inputLines.line1()),
                                    Utility::StringToDList(inputLines.line2()));
    }
    default:
        break;
    }
    delete graphTemplate;
    graphTemplate = new GraphTemplate(this, data, false);
    formulaLabel->setText(Modeling::TypeToFormula(currentType));
    form->setWidget(2, QFormLayout::FieldRole, graphTemplate);

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

ModellingWidget::InputLines::InputLines(QWidget* parent, std::shared_ptr<GeneralData> data, Modeling::Type type) {
    _amountOfSamples = new QLineEdit(QString::number( data == nullptr ? 1000 : data->amountOfSamples), parent);
    _timeStep = new QLineEdit(QString::number( data == nullptr ? 1 : 1/data->Hz), parent);
    _timeFreq = new QLineEdit(QString::number( data == nullptr ? 1 : data->Hz), parent);

    size_t delay = 1;
    double scale1 = 1;
    double scale2 = 1;
    double phase = 0;
    double freq1 = 1;
    double freq2 = 1;

    switch (type) {
    case Modeling::Type::SingleImpulse:
    case Modeling::Type::SingleHop: {
        delay = samples()/4;
        break;
    }
    case Modeling::Type::DecreasingExponent: {
        scale1 = qPow(0.01, 1.0/samples());
        break;
    }
    case Modeling::Type::SineWave: {
        scale2 = 20 * M_PI / samples();
//        data = Modeling::sampledSineWave(inputLines.samples(), inputLines.scale1(), inputLines.scale2(), inputLines.phase());
        break;
    }
    case Modeling::Type::Meander: {
        delay = samples()/10;
//        data = Modeling::meander(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::Saw: {
        delay = samples()/10;
//        data = Modeling::saw(inputLines.samples(), inputLines.delay());
        break;
    }
    case Modeling::Type::ExponentialEnvelope: {
        scale2 = samples() / (2.0 * qLn(10) * timeFreq());
        freq1 = 20 * timeFreq() / samples(); // 20 periods
//        data = Modeling::exponentialEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
//                                             inputLines.scale2(), inputLines.freq1(), inputLines.phase());
        break;
    }
    case Modeling::Type::BalanceEnvelope: {
        freq1 = 2 * timeFreq() / samples();
        freq2 = 50 * timeFreq() / samples();
//        data = Modeling::balanceEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
//                                         inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    case Modeling::Type::TonalEnvelope: {
        freq1 = 3 * timeFreq() / samples();
        freq2 = 50 * timeFreq() / samples();
        scale2 = 0.5;
//        data = Modeling::tonalEnvelope(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
//                                       inputLines.freq1(), inputLines.freq2(), inputLines.phase(),
//                                       inputLines.scale2());
        break;
    }
    case Modeling::Type::LFM: {
        freq1 = 0;
        freq2 = (20.0 * timeFreq()) / samples();
//        data = Modeling::LFM(inputLines.samples(), inputLines.timeStep(), inputLines.scale1(),
//                             inputLines.freq1(), inputLines.freq2(), inputLines.phase());
        break;
    }
    case Modeling::Type::UniformWhiteNoise:
    case Modeling::Type::NormalWhiteNoise: {
        scale1 = 0;
        scale2 = 1;
//        data = randomModeling::whiteNoise(inputLines.samples(), inputLines.timeStep(),
//                                          inputLines.scale1(), inputLines.scale2());
        break;
    }
    default:
        break;
    }

    _delay = new QLineEdit(QString::number(delay), parent);
    _scale1 = new QLineEdit(QString::number(scale1), parent);
    _scale2 = new QLineEdit(QString::number(scale2), parent);
    _phase = new QLineEdit(QString::number(phase), parent);
    _freq1 = new QLineEdit(QString::number(freq1), parent);
    _freq2 = new QLineEdit(QString::number(freq2), parent);
    _line1 = new QLineEdit("", parent);
    _line2 = new QLineEdit("", parent);
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
//        MainWindow::grWindow->resize(300, 100 * MainWindow::grWid->graphData->getAmountOfChannels());
    }

    isAddToCurrent->setChecked(true);

//    closeWidget();
}
