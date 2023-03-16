#include "AnalysisWindowHandler.hpp"
#include "../glViewTemplate/gltemplateoscillogram.h"
#include "../Transformation/TransformToOscillogram.hpp"
#include <QWheelEvent>

#include <iostream>

AnalysisWindowHandler* AnalysisWindowHandler::getInstance() {
    if (instance == nullptr) {
        instance = new AnalysisWindowHandler();
    }
    return instance;
}

void AnalysisWindowHandler::analyze2DBy(Graph2DData *data, glType t) {
    if (analyzeWidget == nullptr) return;

    if (t == glType::Oscillogram) {
        glTemplateOscillogram *gView = new glTemplateOscillogram(nullptr, data);
        gView->resize(300, 60);

        AnalysisWindowHandler::getAnalyzeWidget()->layout->addWidget(gView);
        gView->show();
        return;
    } else if (t == glType::FourierSpectrum) {return;}

    //analyzeWidget->layout->addWidget();
}

void AnalysisWindowHandler::analyze3DBy(Graph2DData *data) {
    if (analyzeWidget == nullptr) return;
}

void AnalysisWindowHandler::addWidget(ParseData* pData) {
    if (analyzeWidget == nullptr) return;

    for (long long i = 0; i < pData->getAmountOfChannels(); i++) {
        analyze2DBy(TransformToOscillogram::transform(pData, i), glType::Oscillogram);
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
    return x / 5;
}

void AnalysisWindowHandler::scrollGraph(long long y) {
    if (ref == nullptr) return;

    //if ((y > 0) && (ref->data->rcur - ref->data->lcur < 10)) return;

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

    std::cout << ref->data->lcur << " " << ref->data->rcur << " " << y << std::endl;
    std::cout << ref->gView->data->lcur << " " << ref->gView->data->rcur << " " << y << std::endl;

    ref->gView->updateGraph();
}



