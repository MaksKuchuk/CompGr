#ifndef ANALYSISWINDOWHANDLER_H
#define ANALYSISWINDOWHANDLER_H

#include "../GraphGlData/Graph2DData.hpp"
#include "../GraphGlData/Graph3DData.hpp"
#include "../analyzewidget.h"
#include "../GraphGlData/generaldata.h"
#include "../glViewType/glType.hpp"
#include <cmath>
#include "../glViewTemplate/gltemplateoscillogram.h"
#include "../graphtemplate.h"

class AnalysisWindowHandler: public QWidget {
    AnalysisWindowHandler();

    double scrollF(long long x);

    glTemplateOscillogram* glTemplateOscillogramArr[16]{};
    long long glTemplateOscillogramArrSize = 0;

    void addWidInArr(glTemplateOscillogram* r);

    void removeWdFromArr(glTemplateOscillogram* r);

public:
    static inline AnalysisWindowHandler* instance = nullptr;

    static inline AnalyzeWidget* analyzeWidget = nullptr;

    glTemplateOscillogram* ref = nullptr;

    static AnalysisWindowHandler* getInstance();

    void analyze2DBy(std::shared_ptr<Graph2DData> data, glType t, QPointer<GraphTemplate> templ = nullptr);

    void analyze3DBy(std::shared_ptr<Graph3DData> data);

    void addWidget(std::shared_ptr<GeneralData> pData);

    AnalyzeWidget* getAnalyzeWidget();

    void destroyWidget();

    bool isNullAnalyzeWidget();

    void setLocalRef(glTemplateOscillogram* ref);

    glTemplateOscillogram* getLocalRef();

    void scrollGraph(long long x);

    void moveGraph(long long x);

    void changeLocalScale(double lmin, double lmax);

    static void setDefaultScale();

    static void setDefaultBias();

    static void changeScrollBar(long long amount, long long lcur, long long rcur);

    static void changeSingleLocalScale(double lmin, double lmax);

    static void setSingleGlobalScale();

    static void updateGraphs(glTemplateOscillogram* rf);

    ~AnalysisWindowHandler();

public slots:

    void scrollBarHasChanged();
};

#endif
