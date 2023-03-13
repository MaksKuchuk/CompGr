#ifndef ANALYSISWINDOWHANDLER_H
#define ANALYSISWINDOWHANDLER_H

#include "../GraphGlData/Graph2DData.hpp"
#include "../analyzewidget.h"
#include "../Parser/ParseData.hpp"

class AnalysisWindowHandler {
    static inline AnalysisWindowHandler* instance;

    static inline AnalyzeWidget* analyzeWidget;

public:
    AnalysisWindowHandler();

    static AnalysisWindowHandler* getInstance();

    void analyze2DBy(Graph2DData *data);

    void analyze3DBy(Graph2DData *data);

    void addWidget(ParseData* pData);

    AnalyzeWidget* getAnalyzeWidget();

    void destroyWidget();

    ~AnalysisWindowHandler();
};

#endif
