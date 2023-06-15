#include "Graph2DData.hpp"

std::shared_ptr<Graph2DData> Graph2DData::GetSlice(qint64 l, qint64 r) {
    auto gr = std::make_shared<Graph2DData>();

    gr->name = name;
    gr->source = source;

//    gr->maxVal = maxVal(n);
//    gr->minVal = minVal(n);

//    gr->maxLoc = gr->maxVal;
//    gr->minLoc = gr->minVal;

    gr->amountOfSamples = r-l;
    gr->Hz = Hz;
//    gr->totalSeconds = getTotalDuration();
    gr->samples = samples.sliced(l, r-l);

    gr->lcur = 0;
    gr->rcur = r-l;

    return gr;
}
