#ifndef ANALYSISWINDOWHANDLER_H
#define ANALYSISWINDOWHANDLER_H

#include "../GraphGlData/Graph2DData.hpp"

class AnalysisWindwoHandler {
    static AnalysisWindwoHandler* instance;

    AnalysisWindwoHandler();
public:
    static AnalysisWindwoHandler* getInstance();

    void analyze2DBy(Graph2DData *data);

    void analyze3DBy(Graph2DData *data);
};

#endif
