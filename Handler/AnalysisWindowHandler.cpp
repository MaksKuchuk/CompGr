#include "AnalysisWindowHandler.hpp"

AnalysisWindwoHandler* AnalysisWindwoHandler::getInstance() {
    if (instance == nullptr) {
        instance = new AnalysisWindwoHandler();
    }
    return instance;
}

void AnalysisWindwoHandler::analyze2DBy(Graph2DData *data) {

}


