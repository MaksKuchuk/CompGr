#include "AnalysisWindowHandler.hpp"
#include "../glViewTemplate/gltemplateoscillogram.h"
#include "../Transformation/TransformToOscillogram.hpp"
#include "../Transformation/TransformToFourierSpectrum.hpp"
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

void AnalysisWindowHandler::analyze2DBy(Graph2DData *data, glType t) {
    if (analyzeWidget == nullptr) return;

    long long layoutSize = AnalysisWindowHandler::getAnalyzeWidget()->layout->count();

    if (t == glType::Oscillogram) {
        glTemplateOscillogram *gView = new glTemplateOscillogram(nullptr, data);
        gView->resize(300, 60);

        AnalysisWindowHandler::getAnalyzeWidget()->layout->insertWidget(layoutSize - 1, gView);
        //AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);
        gView->show();
        return;
    } else if (t == glType::FourierSpectrum) {
        glTemplateOscillogram *gView = new glTemplateOscillogram(nullptr, data);
        gView->resize(300, 60);

        AnalysisWindowHandler::getAnalyzeWidget()->layout->insertWidget(layoutSize - 1, gView);
        //AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);
        gView->show();
        return;
    }

    changeScrollBar(data->amountOfSamples, data->lcur, data->rcur);
}

void AnalysisWindowHandler::analyze3DBy(Graph2DData *data) {
    if (analyzeWidget == nullptr) return;
}

void AnalysisWindowHandler::addWidget(ParseData* pData) {
    if (analyzeWidget == nullptr) return;

    for (long long i = 0; i < pData->getAmountOfChannels(); i++) {
        analyze2DBy(TransformToOscillogram::transform(pData, i), glType::Oscillogram);
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

double AnalysisWindowHandler::scrollF(long long x) {
    const double e = 2.718281828459045;
    //return pow(log(x + 3) / log(2), 2) + 1;
    return x / 16 + 1;
}

void AnalysisWindowHandler::scrollGraph(long long y) {
    if (analyzeWidget == nullptr) return;
    if (ref == nullptr) return;

    if ((y > 0) && (ref->data->rcur - ref->data->lcur < 10)) return;

    ref->data->lcur += 0.01 * y * scrollF(static_cast<double>(ref->data->rcur - ref->data->lcur));
    ref->data->rcur -= 0.01 * y * scrollF(static_cast<double>(ref->data->rcur - ref->data->lcur));

    if (ref->data->lcur < 0) ref->data->lcur = 0;
    if (ref->data->rcur < 0) ref->data->rcur = 0;
    if (ref->data->lcur >= ref->data->amountOfSamples)
        ref->data->lcur = ref->data->amountOfSamples - 1;
    if (ref->data->rcur >= ref->data->amountOfSamples)
        ref->data->rcur = ref->data->amountOfSamples - 1;

    if (ref->data->rcur - ref->data->lcur < 10) {
        if (ref->data->lcur + 10 < ref->data->amountOfSamples) {
            ref->data->rcur = ref->data->lcur + 9;
        } else if (ref->data->rcur - 10 >= 0) {
            ref->data->lcur = ref->data->rcur - 9;
        } else {
            ref->data->lcur = 0;
            ref->data->rcur = ref->data->amountOfSamples - 1;
        }
    }

    ref->gView->updateGraph();

    updateGraphs(ref);
    changeScrollBar(ref->data->amountOfSamples, ref->data->lcur, ref->data->rcur);
}

void AnalysisWindowHandler::moveGraph(long long y) {
    if (analyzeWidget == nullptr) return;
    if (ref == nullptr) return;

    long long ch = y * scrollF(static_cast<double>(ref->data->rcur - ref->data->lcur));

    if (ref->data->lcur + ch < 0) return;
    if (ref->data->rcur + ch >= ref->data->amountOfSamples) return;

    ref->data->lcur += ch;
    ref->data->rcur += ch;

    ref->gView->updateGraph();

    updateGraphs(ref);
    changeScrollBar(ref->data->amountOfSamples, ref->data->lcur, ref->data->rcur);
}

void AnalysisWindowHandler::changeLocalScale(double lmin, double lmax) {
    if (analyzeWidget == nullptr) return;
    if (ref == nullptr) return;

    double ch = 5 * scrollF(static_cast<double>(ref->data->maxLoc - ref->data->minLoc));

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

    scr->setRange(0, amount - (rcur - lcur));
    scr->setValue(lcur);
}

void AnalysisWindowHandler::updateGraphs(glTemplateOscillogram* rf) {
    for (long long i = 0; i < analyzeWidget->layout->count() - 1; i++) {
        glTemplateOscillogram* glTemp = static_cast<glTemplateOscillogram*>
                (analyzeWidget->layout->itemAt(i)->widget());

        glTemp->data->lcur = rf->data->lcur;
        glTemp->data->rcur = rf->data->rcur;
        glTemp->gView->updateGraph();
        glTemp->repaint();
    }

    if (MainWindow::grWid == nullptr) return;

    for (long long i = 0; i < MainWindow::grWid->layout()->count(); i++) {
        GraphTemplate* grTemp =
                static_cast<GraphTemplate*>(MainWindow::grWid->layout()->itemAt(i)->widget());

        static_cast<glView*>
                (grTemp->layout()->itemAt(0)->widget())->
                setCurs(rf->data->lcur, rf->data->rcur);
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

    glTemp->data->lcur = lc;
    glTemp->data->rcur = rc;

    updateGraphs(glTemp);
}



