#include "superposition.h"

std::shared_ptr<Graph2DData> Superposition::LinearSuperposition(std::shared_ptr<GeneralData> data, QList<int> indices, QList<double> coeffs) {
    auto graph = std::make_shared<Graph2DData>();
    graph->samples.resize(data->amountOfSamples);
    graph->maxVal = -std::numeric_limits<double>::max();
    graph->minVal = std::numeric_limits<double>::max();
    for (size_t i = 0; i < data->amountOfSamples; ++i) {
        double cur = coeffs.back();
        for (auto ind : indices) {
            cur += coeffs[ind] * data->channels[ind][i];
        }
        graph->samples[i] = cur;
        if (cur > graph->maxVal) graph->maxVal = cur;
        if (cur < graph->minVal) graph->minVal = cur;
    }
    graph->name = "Lin Superpos ";
    graph->source = "Lin Superpos of";
    for (auto i : indices)
        graph->source += " " + QString::number(i);

    return graph;
}

std::shared_ptr<Graph2DData> Superposition::MuliplicativeSuperposition(std::shared_ptr<GeneralData> data, QList<int> indices, double coeff) {
    auto graph = std::make_shared<Graph2DData>();
    graph->samples.resize(data->amountOfSamples);
    graph->amountOfSamples = data->amountOfSamples;
    graph->maxVal = -std::numeric_limits<double>::max();
    graph->minVal = std::numeric_limits<double>::max();
    for (size_t i = 0; i < data->amountOfSamples; ++i) {
        double cur = coeff;
        for (auto ind : indices) {
            cur *= data->channels[ind][i];
        }
        graph->samples[i] = cur;
        if (cur > graph->maxVal) graph->maxVal = cur;
        if (cur < graph->minVal) graph->minVal = cur;
    }
    graph->name = "Mult Superpos ";
    graph->source = "Mult Superpos of";
    for (auto i : indices)
        graph->source += " " + QString::number(i);

    return graph;
}
