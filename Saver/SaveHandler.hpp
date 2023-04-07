#ifndef SAVEHANDLER_H
#define SAVEHANDLER_H

#include <QString>
#include <vector>

#include "../GraphGlData/Graph2DData.hpp"

class SaveHandler {
public:
    size_t amountOfChannels = 0;
    size_t amountOfSamples = 0;
    double Hz = 1;
    // recording start datetime (dd-mm-yyyy hh:mm:ss.sss)
    QString startDate{};
    QString startTime{};
    std::vector<QString> channels_name{};

    std::vector<double*> channels{};

    SaveHandler();

    SaveHandler(Graph2DData* data);

    void AddGraph2DData(Graph2DData* data);

private:
    void AddFirst(Graph2DData* data);
    bool CheckConditions(Graph2DData* data);
};

#endif // SAVEHANDLER_H
