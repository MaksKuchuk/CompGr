#include "AnalysisWindowHandler.hpp"
#include "../glViewTemplate/gltemplateoscillogram.h"
#include "../Transformation/TransformToOscillogram.hpp"
#include "../Transformation/TransformToFourierSpectrum.hpp"
#include "../Transformation/TransformToWaveletogram.hpp"
#include "../mainwindow.h"
#include "../glview.h"
#include "../graphtemplate.h"
#include <QWheelEvent>
#include <QScrollBar>

#include <iostream>

AnalysisWindowHandler* AnalysisWindowHandler::getInstance() {
    if (instance == nullptr) {
        instance = new AnalysisWindowHandler();
    }
    return instance;
}

void AnalysisWindowHandler::analyze2DBy(std::shared_ptr<Graph2DData> data, glType t, QPointer<GraphTemplate> templ) {
    if (analyzeWidget == nullptr) return;

    long long layoutSize = AnalysisWindowHandler::getAnalyzeWidget()->layout->count();
    QPointer<glTemplateOscillogram> gView;

    if (t == glType::Oscillogram) {
        gView = new glTemplateOscillogram(nullptr, data, templ);
        connect(gView, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
//        connect(gView, &glTemplateOscillogram::BiasChanged, [](qint64 a,qint64 b) {qDebug() << a << " " << b;});

        AnalysisWindowHandler::getAnalyzeWidget()->layout->insertWidget(layoutSize - 1, gView);
        //AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);
    } else if (t == glType::FourierSpectrum) {
        std::shared_ptr<Graph2DData> fourier_data = FourierData(data, 1, SpectrumModes::PSD, FourierModes::KEEP_FIRST_VAL);
        gView = new glTemplateOscillogram(nullptr, fourier_data, templ);

        AnalysisWindowHandler::getAnalyzeWidget()->layout->insertWidget(layoutSize - 1, gView);
        //AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);
    } else if (t == glType::Waveletogram) {
        gView = new glTemplateOscillogram(nullptr, TransformToWaveletogram::transform(data), templ);

        AnalysisWindowHandler::getAnalyzeWidget()->layout->insertWidget(layoutSize - 1, gView);
        //AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);

    }
    connect(gView,  &glTemplateOscillogram::BiasChanged, gView->gView, &glOscillogram::updateGraph);
    connect(gView,  &glTemplateOscillogram::BiasChanged,
            [&](qint64 l,qint64 r){ changeScrollBar(data->amountOfSamples, l, r); });

    gView->resize(300, 60);

    gView->show();


    changeScrollBar(data->amountOfSamples, data->lcur, data->rcur);
}

void AnalysisWindowHandler::analyze3DBy(std::shared_ptr<Graph3DData> data) {
    if (analyzeWidget == nullptr) return;
}

void AnalysisWindowHandler::addWidget(std::shared_ptr<GeneralData> pData) {
    if (analyzeWidget == nullptr) return;

    for (long long i = 0; i < pData->getAmountOfChannels(); i++) {
        analyze2DBy(pData->channelTo2D(i), glType::Oscillogram,
                    static_cast<GraphTemplate*>(MainWindow::grWid->layout()->itemAt(i)->widget()));
        //analyze2DBy(TransformToFourierSpectrum::transform(pData, i), glType::Oscillogram);
    }
}

AnalysisWindowHandler::AnalysisWindowHandler() = default;

AnalysisWindowHandler::~AnalysisWindowHandler() = default;

AnalyzeWidget* AnalysisWindowHandler::getAnalyzeWidget() {
    if (analyzeWidget == nullptr) {
        analyzeWidget = new AnalyzeWidget();
    }
    return analyzeWidget;
}

void AnalysisWindowHandler::destroyWidget() {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->templ->resetCurs();
    }

    analyzeWidget = nullptr;
    ref = nullptr;
}

bool AnalysisWindowHandler::isNullAnalyzeWidget() {
    return analyzeWidget == nullptr;
}

void AnalysisWindowHandler::setLocalRef(glTemplateOscillogram* rf) {
    ref = rf;
}

glTemplateOscillogram* AnalysisWindowHandler::getLocalRef() {
    return ref;
}



void AnalysisWindowHandler::changeLocalScale(double lmin, double lmax) {
    if (analyzeWidget == nullptr) return;
    if (ref == nullptr) return;

    double ch = 5;// * scrollF(static_cast<double>(ref->data->maxLoc - ref->data->minLoc));

    if (ref->data->minLoc + lmin * ch >= ref->data->maxLoc + lmax * ch) return;

    ref->data->minLoc += lmin * ch;
    ref->data->maxLoc += lmax * ch;

    ref->gView->updateGraph();
}

void AnalysisWindowHandler::changeSingleLocalScale(double lmin, double lmax) {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->data->minLoc = lmin;
        glTemp->data->maxLoc = lmax;
        glTemp->gView->updateGraph();
        glTemp->repaint();
    }
}

void AnalysisWindowHandler::setSingleGlobalScale() {
    double lmi = 0, lma = 0;
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        if (i == 0) {
            lmi = glTemp->data->minVal;
            lma = glTemp->data->maxVal;
        }

        if (lmi > glTemp->data->minVal) lmi = glTemp->data->minVal;
        if (lma < glTemp->data->maxVal) lma = glTemp->data->maxVal;
    }

    if (lmi != 0 && lma != 0) {
        changeSingleLocalScale(lmi, lma);
    }

}

void AnalysisWindowHandler::setDefaultScale() {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->data->minLoc = glTemp->data->minVal;
        glTemp->data->maxLoc = glTemp->data->maxVal;
        glTemp->gView->updateGraph();
        glTemp->repaint();
    }
}

void AnalysisWindowHandler::setDefaultBias() {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->data->lcur = 0;
        glTemp->data->rcur = glTemp->data->amountOfSamples - 1;
        glTemp->gView->updateGraph();
        glTemp->repaint();
    }
}

void AnalysisWindowHandler::changeScrollBar(long long amount, long long lcur, long long rcur) {
    if (analyzeWidget == nullptr) return;

    QScrollBar* scr = static_cast<QScrollBar*>(analyzeWidget
                            ->layout->itemAt(analyzeWidget->layout->count() - 1)->widget());

    scr->setRange(0, amount - (rcur - lcur) - 1);
    scr->setValue(lcur);
}

void AnalysisWindowHandler::updateGraphs(glTemplateOscillogram* rf) {
    return;
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->data->lcur = rf->data->lcur;
        glTemp->data->rcur = rf->data->rcur;
        glTemp->gView->updateGraph();
        glTemp->repaint();

        if (glTemp->templ != nullptr && glTemp->templ->gView != nullptr)
            glTemp->templ->gView->setCurs(rf->data->lcur, rf->data->rcur);
    }
}

void AnalysisWindowHandler::changeScaleByMouse() {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
            (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->gView->updateGraph();
        glTemp->repaint();
    }
}

void AnalysisWindowHandler::scrollBarHasChanged() {
    if (analyzeWidget == nullptr) return;

    QScrollBar* scr = static_cast<QScrollBar*>(analyzeWidget
                                ->layout->itemAt(analyzeWidget->layout->count() - 1)->widget());

    long long lc = scr->value();

    long long lcurtmp = static_cast<glTemplateOscillogram*>(
        analyzeWidget->layout->itemAt(0)->widget())->data->lcur;

    long long rcurtmp = static_cast<glTemplateOscillogram*>(
                analyzeWidget->layout->itemAt(0)->widget())->data->rcur;

    long long diff = rcurtmp - lcurtmp;

    long long rc = lc + diff;

    glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
            (analyzeWidget->layout->itemAt(0)->widget());

    if (rc >= glTemp->data->amountOfSamples) {
        auto diff = rc - glTemp->data->amountOfSamples + 1;
        lc -= diff;
        rc -= diff;
    }

    glTemp->data->lcur = lc;
    glTemp->data->rcur = rc;

    updateGraphs(glTemp);
}

long long AnalysisWindowHandler::getXPosition() {
     if (analyzeWidget == nullptr) return 0;

     return analyzeWidget->mapToGlobal(QPoint(0,0)).x();
}

long long AnalysisWindowHandler::getYPosition() {
     if (analyzeWidget == nullptr) return 0;

     return analyzeWidget->mapToGlobal(QPoint(0,0)).y();          
}

std::shared_ptr<Graph2DData> AnalysisWindowHandler::FourierData
    (
        std::shared_ptr<Graph2DData> data,
        long long smooth,
        SpectrumModes mode,
        FourierModes first_val_mode
    )
{
    auto new_data = TransformToFourierSpectrum::transform(data, smooth, mode, first_val_mode);
    return new_data;
}



