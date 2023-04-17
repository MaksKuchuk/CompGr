#include "../GraphGlData/Graph2DData.hpp"
#include "../GraphGlData/generaldata.h"
#include "TransformToOscillogram.hpp"

#include <memory>

std::shared_ptr<Graph2DData> TransformToOscillogram::transform(std::shared_ptr<GeneralData> data, long long n) {
    auto gr = std::make_shared<Graph2DData>();

    gr->name = data->getChannelName(n);
    gr->source = data->getSource();

    gr->maxVal = data->maxVal(n);
    gr->minVal = data->minVal(n);

    gr->maxLoc = gr->maxVal;
    gr->minLoc = gr->minVal;

    gr->amountOfSamples = data->getAmountOfSamples();
    gr->Hz = data->getHz();
    gr->totalSeconds = data->getTotalDuration();
    gr->samples = data->getChannel(n);

    gr->lcur = 0;
    gr->rcur = gr->amountOfSamples - 1;

    return gr;
}
