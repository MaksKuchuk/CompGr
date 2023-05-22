#include "TransformToWaveletogram.hpp"
#include <memory>
#include "../GraphGlData/Graph2DData.hpp"

void TransformToWaveletogram::DirectTransform(QList<double>& wavelet, QList<double>& toTranform) {
    if (toTranform.size() == 1) {
        wavelet.push_back(toTranform.back());
        return;
    }

    QList<double> next;

    for (size_t i = 0; i < toTranform.size() - 1; i+=2) {
        wavelet.push_back((toTranform[i] - toTranform[i+1]) / 2.0);
        next.push_back((toTranform[i] + toTranform[i+1]) / 2.0);
    }

    DirectTransform(wavelet, next);
}

std::shared_ptr<Graph2DData> TransformToWaveletogram::transform(const std::shared_ptr<Graph2DData> data) {
    auto newData = std::make_shared<Graph2DData>();

    QList<double> samples = data->samples;
    size_t size = samples.size();
    size_t newLogSize = std::log2(size);
    if (std::pow(2, newLogSize) < size)
        ++newLogSize;
    samples.resize((size_t)std::pow(2, newLogSize));

    DirectTransform(newData->samples, samples);

    newData->amountOfSamples = newData->samples.size();
    newData->name = "Wavelet of "+data->name;
    newData->totalSeconds = newData->amountOfSamples - 1;
    newData->lcur = 0;
    newData->rcur = newData->amountOfSamples - 1;

    newData->maxVal = -std::numeric_limits<double>::max();
    newData->minVal = std::numeric_limits<double>::max();
    for (size_t i = 0; i < newData->amountOfSamples; ++i) {
        newData->maxVal = std::max(newData->maxVal, newData->samples[i]);
        newData->minVal = std::min(newData->minVal, newData->samples[i]);
    }
    newData->maxLoc = newData->maxVal;
    newData->minLoc = newData->minVal;

    return newData;
}
