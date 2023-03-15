#include "AnalysisWindowHandler.hpp"
#include "../glViewTemplate/glTemplateOscillogram.hpp"
#include "../Transformation/TransformToOscillogram.hpp"

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

AnalysisWindowHandler::AnalysisWindowHandler() {
    analyzeWidget = new AnalyzeWidget();
}

AnalyzeWidget* AnalysisWindowHandler::getAnalyzeWidget() {
    if (analyzeWidget == nullptr) {
        analyzeWidget = new AnalyzeWidget();
    }
    return analyzeWidget;
}

void AnalysisWindowHandler::destroyWidget() {
    analyzeWidget = nullptr;
}
