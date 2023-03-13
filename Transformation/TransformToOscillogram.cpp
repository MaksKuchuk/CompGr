#include "../GraphGlData/Graph2DData.hpp"
#include "../Parser/ParseData.hpp"

static Graph2DData* transform(ParseData* data, long long n) {
    Graph2DData* gr = new Graph2DData;

    gr->name = data->getChannelName(n);
    gr->source = data->getSource();

    gr->maxVal = data->maxVal(n);
    gr->minVal = data->minVal(n);

    gr->amountOfSamples = data->getAmountOfSamples();
    gr->Hz = data->getHz();
    gr->totalSeconds = data->getTotalDuration();
    gr->samples = data->getChannel(n);

    return gr;
}
