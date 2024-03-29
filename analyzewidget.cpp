#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include <QVBoxLayout>
#include <QEvent>
#include "glViewTemplate/gltemplateoscillogram.h"
#include "glViewTemplate/gltemplatespectrogram.h"
#include "GraphGlData/Graph3DData.hpp"
#include <QScrollBar>

#include <iostream>

#include "Transformation/TransformToFourierSpectrum.hpp"
#include "Transformation/TransformToSpectrogram.hpp"
#include "Utility/config.h"
#include "Utility/generaldialog.h"
#include "fourierDialog.h"
#include "mainwindow.h"

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
}

QPointer<AnalyzeWidget> AnalyzeWidget::getInstance() {
    if (instance == nullptr)
        instance = new AnalyzeWidget;
    return instance;
}

void AnalyzeWidget::analyze(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t) {

    switch (t) {
        case glType::Oscillogram: {
            MainWindow::openAnalysisWindow();

            auto gView = new glTemplateOscillogram(this, data);
            connect(gView, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
            connect(gView, &glTemplateOscillogram::BiasChanged,
                    [&](qint64 a, qint64 b){ if (this != nullptr) multipleBiasStart(a,b);}
            );
            connect(this, &AnalyzeWidget::multipleBiasStartSignal, gView, &glTemplateOscillogram::SetBias);
            connect(this, &AnalyzeWidget::setGlobalScaleSignal, gView, &glTemplateOscillogram::SetScale);
            connect(this, &AnalyzeWidget::ResetBiasSignal, gView, &glTemplateOscillogram::ResetBias);
            connect(this, &AnalyzeWidget::ResetScaleSignal, gView, &glTemplateOscillogram::ResetScale);
            connect(this, &AnalyzeWidget::SetSingleScaleSignal, gView, &glTemplateOscillogram::setLocalScale);


            layout->addWidget(gView);
            gView->resize(300, 60);
            gView->show();

            templ->oscilogram = gView;

            break;
        }

        case glType::FourierSpectrum: {

            long long smoothing;
            SpectrumModes mode;
            FourierModes first_val;

            auto dlg = new fourierDialog(0, data->amountOfSamples / 2);
            if (!dlg->exec())
                return;

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

            qint64 fill_zeroes = 0;
            if (Config::fourierFill) {
                auto diff = data->rcur - data->lcur;
                fill_zeroes = std::pow(2, (qint64)std::log2(diff));
                if (fill_zeroes != diff)
                    fill_zeroes *= 2;
            }


            auto gView = new glTemplateOscillogram(nullptr,
                                          TransformToFourierSpectrum::transform(data, smoothing,
                                                mode, first_val, fill_zeroes), glType::FourierSpectrum);


            gView->setWindowTitle("Fourier Spectrum");
            gView->setMinimumSize(300, 100);
            MainWindow::AddWidget(gView);
            break;
        }
        case glType::Waveletogram: {
//            auto gView = new glTemplateOscillogram(nullptr, TransformToWaveletogram::transform(data), templ);
            // Spectrogram test

            auto data3d = std::make_shared<Graph3DData>(3, 3);
            data3d->samples = {{0.1,0.2,0.3},{0.4,0.5,0.6},{0.7,0.8,0.9}};

            data3d->minVal = 0;
            data3d->maxVal = 0.9;


            auto spect = new glTemplateSpectrogram(nullptr, data3d);


            spect->setMinimumSize(300, 100);
            MainWindow::AddWidget(spect);
            break;
        }
        case glType::Spectrogram: {
            auto size = 1000;
            if (templ->oscilogram != nullptr)
                size = templ->oscilogram->gView->width();


            auto sizes = GeneralDialog::MultiInputDialog("Spectrogram", {"Width", "Height", "Overlap"},
                                                         {QString::number(size), "128", "1.5"});
            if (sizes[0] == "__REJECTED_INPUT__" || sizes[0].toULongLong() == 0 || sizes[1].toULongLong() == 0)
                return;

            if (sizes[0].toULongLong() > data->amountOfSamples / 2) {
                GeneralDialog::InfoDialog("Width is too big!");
                return;
            }

            auto spect = new glTemplateSpectrogram(nullptr,
                                                   TransformToSpectrogram::transform(data, sizes[0].toULongLong(),
                                                   sizes[1].toULongLong(), sizes[2].toDouble()));


            spect->setMinimumSize(300, 100);
            spect->gSpec->setMinimumSize(size, 100);
            MainWindow::AddWidget(spect);

            templ->spectrogram = spect;
            if (templ->oscilogram != nullptr) {
                auto bias_func = [spect, data](qint64 l, qint64 r) {
                    spect->SetBias(l * spect->data->width / data->amountOfSamples, r * spect->data->width / data->amountOfSamples);
                };

                auto conn = connect(templ->oscilogram, &glTemplateOscillogram::BiasChanged, bias_func);
                connect(spect, &QWidget::destroyed, [=](){
                    disconnect(conn);
                });
            }

            break;
        }
    }
//    ui->verticalLayout->addWidget(gView);

}

void AnalyzeWidget::multipleBiasStart(qint64 l, qint64 r) {
    if (!AnalyzeWidget::isMultipleBiasStarted && Config::multipleBias) {
        AnalyzeWidget::isMultipleBiasStarted = true;

        emit multipleBiasStartSignal(l, r);

        AnalyzeWidget::isMultipleBiasStarted = false;
    }
}

void AnalyzeWidget::SetSingleScale(double min, double max) {
    emit setGlobalScaleSignal(min, max);
}

void AnalyzeWidget::SetGlobalScale() {
    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();
    for (qint64 i = 0; i < layout->count(); ++i) {
       auto osc = qobject_cast<glTemplateOscillogram*>( layout->itemAt(i)->widget());
       min = std::min(min, osc->gView->data->minVal);
       max = std::max(max, osc->gView->data->maxVal);
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
    AnalyzeWidget::isOpened = false;
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
