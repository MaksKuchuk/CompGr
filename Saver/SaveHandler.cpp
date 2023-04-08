#include "SaveHandler.hpp"

#include <QString>
#include <QStringList>
#include <vector>

#include "../GraphGlData/Graph2DData.hpp"

SaveHandler::SaveHandler() = default;

SaveHandler::SaveHandler(Graph2DData* data) {
    AddFirst(data);
}

void SaveHandler::AddFirst(Graph2DData* data) {
    amountOfChannels++;
    amountOfSamples = data->amountOfSamples;
    Hz = data->Hz;

    auto time = data->startDate.split(" ");

    startDate = time[0];
    startTime = time[1];

    channels_name.push_back(data->name);
    channels.push_back(data->samples);
}

bool SaveHandler::CheckConditions(Graph2DData* data) {
    return (Hz == data->Hz) &&
            (amountOfSamples == data->amountOfSamples);
}

void SaveHandler::AddGraph2DData(Graph2DData* data) {
    if (amountOfChannels == 0) {
        AddFirst(data);
        return;
    }
    if (!CheckConditions(data))
        throw std::runtime_error("channels are different Hz or AmountOfSamples");

    amountOfChannels++;
    channels_name.push_back(data->name);
    channels.push_back(data->samples);
}

