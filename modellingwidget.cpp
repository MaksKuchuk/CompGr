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

    auto upd = [&](){ ChangeSamples(); };
    connect(dropDown,&QComboBox::currentIndexChanged, [&](){ ChangedModel(); });
    connect(inputLines._amountOfSamples, &QLineEdit::editingFinished, upd);
    connect(inputLines._delay, &QLineEdit::editingFinished, upd);
    connect(inputLines._timeStep, &QLineEdit::editingFinished, upd);
    connect(inputLines._scale1, &QLineEdit::editingFinished, upd);
    connect(inputLines._scale2, &QLineEdit::editingFinished, upd);
    connect(inputLines._phase, &QLineEdit::editingFinished, upd);
    connect(inputLines._freq1, &QLineEdit::editingFinished, upd);
    connect(inputLines._freq2, &QLineEdit::editingFinished, upd);
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
    case Modeling::Type::SingleHop: {
        newRow("Delay", inputLines._delay);
        break;
    }
    case Modeling::Type::SineWave: {

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
    case Modeling::Type::Saw: {
        data = Modeling::saw(a_samples, delay);
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
    auto label = new QLabel();
    label->setText(str);
    label->resize(300,100);
    inputForm->addWidget(label);
    inputForm->addWidget(line);
//    form->addWidget(label);
}

void ModellingWidget::inputFormRemoveRows() {
    qDebug() << inputForm->rowCount();
    for (size_t i = 0; i < inputForm->rowCount(); ++i) {
        auto row = inputForm->takeRow(0);
    }
}


ModellingWidget::InputLines::InputLines(QWidget* parent, std::shared_ptr<GeneralData> data) {
    _amountOfSamples = new QLineEdit(QString::number( data == nullptr ? 10 : data->amountOfSamples), parent);

    _timeStep = new QLineEdit(QString::number( data == nullptr ? 1 : 1/data->Hz), parent);
    _delay = new QLineEdit(QString::number(0), parent);
    _scale1 = new QLineEdit(QString::number(0), parent);
    _scale2 = new QLineEdit(QString::number(0), parent);
    _phase = new QLineEdit(QString::number(0), parent);
    _freq1 = new QLineEdit(QString::number(0), parent);
    _freq2 = new QLineEdit(QString::number(0), parent);
    Hide();
}
