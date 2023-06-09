#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include <QVBoxLayout>
#include <QEvent>
#include "glViewTemplate/gltemplateoscillogram.h"
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

void AnalyzeWidget::analyze2DBy(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t) {
    QPointer<glTemplateOscillogram> gView;

    if (t == glType::Oscillogram) {
        gView = new glTemplateOscillogram(nullptr, data, templ);
        connect(gView, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
        connect(gView, &glTemplateOscillogram::BiasChanged,
                [&](qint64 a, qint64 b){ if (this != nullptr) multipleBiasStart(a,b);}
        );
        connect(this, &AnalyzeWidget::multipleBiasStartSignal, gView, &glTemplateOscillogram::SetBias);

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


    }
//    ui->verticalLayout->addWidget(gView);
    layout->addWidget(gView);
    gView->resize(300, 60);

    gView->show();
}

void AnalyzeWidget::analyze3DBy(std::shared_ptr<Graph2DData> data) {
    return;
}

void AnalyzeWidget::multipleBiasStart(qint64 l, qint64 r) {
    if (!AnalyzeWidget::isMultipleBiasStarted && Config::multipleBias) {
        AnalyzeWidget::isMultipleBiasStarted = true;

        emit multipleBiasStartSignal(l, r);

        AnalyzeWidget::isMultipleBiasStarted = false;
    }
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

