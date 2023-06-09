#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include <QVBoxLayout>
#include <QEvent>
#include "glViewTemplate/gltemplateoscillogram.h"
#include "glViewType/glspectrogram.h"
#include "GraphGlData/Graph3DData.hpp"
#include <QScrollBar>

#include <iostream>

#include "Transformation/TransformToFourierSpectrum.hpp"
#include "Transformation/TransformToWaveletogram.hpp"
#include "Utility/config.h"
#include "fourierDialog.h"

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
    instance = this;

    layout=ui->verticalLayout;

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    ui->actionSimultaneous_moving->setChecked(Config::multipleBias);
    ui->actionLog_Scale_X->setChecked(Config::xLogScale);
    ui->actionLog_Scale_Y->setChecked(Config::yLogScale);
}

QPointer<AnalyzeWidget> AnalyzeWidget::getInstance() {
    if (instance == nullptr)
        instance = new AnalyzeWidget;
    return instance;
}

void AnalyzeWidget::analyze(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t) {

    QPointer<glTemplateOscillogram> gView;

    if (t == glType::Oscillogram) {
        gView = new glTemplateOscillogram(nullptr, data, templ);
        connect(gView, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
        connect(gView, &glTemplateOscillogram::BiasChanged,
                [&](qint64 a, qint64 b){ if (this != nullptr) multipleBiasStart(a,b);}
        );
        connect(this, &AnalyzeWidget::multipleBiasStartSignal, gView, &glTemplateOscillogram::SetBias);
        connect(this, &AnalyzeWidget::setGlobalScaleSignal, gView, &glTemplateOscillogram::SetScale);
        connect(this, &AnalyzeWidget::ResetBiasSignal, gView, &glTemplateOscillogram::ResetBias);
        connect(this, &AnalyzeWidget::ResetScaleSignal, gView, &glTemplateOscillogram::ResetScale);
        connect(this, &AnalyzeWidget::SetSingleScaleSignal, gView, &glTemplateOscillogram::setLocalScale);

    } else if (t == glType::FourierSpectrum) {

        long long smoothing;
        SpectrumModes mode;
        FourierModes first_val;

        auto dlg = new fourierDialog();
        if (dlg->exec()) {
            smoothing = dlg->smooth_spin->value();

            auto mode_string = dlg->spectrum_mode_cb->currentText();
            if (mode_string == "Amplitude spectrum")
                mode = SpectrumModes::AMPLITUDE_SPECTRUM;
            else if (mode_string == "PSD")
                mode = SpectrumModes::PSD;

            auto first_val_string = dlg->first_val_cb->currentText();
            if (first_val_string == "Keep first val")
                first_val = FourierModes::KEEP_FIRST_VAL;
            else if (first_val_string == "Zero first val")
                first_val = FourierModes::ZERO_FIRST_VAL;
            else if (first_val_string == "Equal to adj")
                first_val = FourierModes::EQUALIZE_WITH_ADJ;

            gView = new glTemplateOscillogram(nullptr,
                                          TransformToFourierSpectrum::transform(data, smoothing,
                                                mode, first_val), templ);
        }


    } else if (t == glType::Waveletogram) {
        gView = new glTemplateOscillogram(nullptr, TransformToWaveletogram::transform(data), templ);


    } else if (t == glType::Spectrogram) {
        auto data3d = std::make_shared<Graph3DData>();
        data3d->amountOfSamples = 10;
        data3d->depth = 10;
        data3d->lcur = 0;
        data3d->lcur = 9;
        data3d->minLoc = 0;
        data3d->maxLoc = 1;

        auto spect = new glSpectrogram(nullptr, data3d);

        layout->addWidget(spect);
        spect->show();
        return;

    }
//    ui->verticalLayout->addWidget(gView);
    layout->addWidget(gView);
    gView->resize(300, 60);

    gView->show();
}

void AnalyzeWidget::multipleBiasStart(qint64 l, qint64 r) {
    if (!AnalyzeWidget::isMultipleBiasStarted && Config::multipleBias) {
        AnalyzeWidget::isMultipleBiasStarted = true;

        emit multipleBiasStartSignal(l, r);

        AnalyzeWidget::isMultipleBiasStarted = false;
    }
}

void AnalyzeWidget::SetSingleScale() {
    emit SetSingleScaleSignal();
}

void AnalyzeWidget::SetGlobalScale() {
    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();
    for (qint64 i = 0; i < layout->count(); ++i) {
       auto osc = qobject_cast<glTemplateOscillogram*>( layout->itemAt(i)->widget());
       min = std::min(min, osc->gView->data->minLoc);
       max = std::max(max, osc->gView->data->maxLoc);
    }

    emit setGlobalScaleSignal(min, max);
}

void AnalyzeWidget::ResetBias() {
    emit ResetBiasSignal();
}

void AnalyzeWidget::ResetScale() {
    emit ResetScaleSignal();
}


AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
    delete layout;
}


void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
//    AnalysisWindowHandler::getInstance()->destroyWidget();
}

void AnalyzeWidget::on_actionSimultaneous_moving_triggered() {
    Config::multipleBias = ui->actionSimultaneous_moving->isChecked();
}

void AnalyzeWidget::on_actionLog_Scale_X_triggered() {
    Config::xLogScale = ui->actionLog_Scale_X->isChecked();
}

void AnalyzeWidget::on_actionLog_Scale_Y_triggered() {
    Config::yLogScale = ui->actionLog_Scale_Y->isChecked();
}

