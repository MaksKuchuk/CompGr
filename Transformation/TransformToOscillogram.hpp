#ifndef TRANSFORMTOOSCILLOGRAM_H
#define TRANSFORMTOOSCILLOGRAM_H

#include "../GraphGlData/generaldata.h"
#include "../GraphGlData/Graph2DData.hpp"

class TransformToOscillogram {
public:

    static std::shared_ptr<Graph2DData> transform(std::shared_ptr<GeneralData> data, long long n);

};

#endif
