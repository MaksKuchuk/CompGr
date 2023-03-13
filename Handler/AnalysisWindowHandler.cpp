#include "AnalysisWindowHandler.hpp"

AnalysisWindowHandler* AnalysisWindowHandler::getInstance() {
    if (instance == nullptr) {
        instance = new AnalysisWindowHandler();
    }
    return instance;
}

void AnalysisWindowHandler::analyze2DBy(Graph2DData *data) {

}

void AnalysisWindowHandler::analyze3DBy(Graph2DData *data) {

}

void AnalysisWindowHandler::addWidget(ParseData* pData) {

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
