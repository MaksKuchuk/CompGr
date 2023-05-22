#ifndef TRANSFORMTOWAVELETOGRAM_H
#define TRANSFORMTOWAVELETOGRAM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph3DData.hpp"
#include <QList>

class TransformToWaveletogram {

    static void DirectTransform(QList<double>& wavelet, QList<double>& toTranform);

public:

    static std::shared_ptr<Graph2DData> transform(const std::shared_ptr<Graph2DData> data);

};

#endif
