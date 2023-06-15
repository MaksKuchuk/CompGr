#ifndef TRANSFORMTOSPECTROGRAM_H
#define TRANSFORMTOSPECTROGRAM_H

#include "../Parser/ParseData.hpp"
#include "../GraphGlData/Graph3DData.hpp"

class TransformToSpectrogram {

    static void column(std::shared_ptr<Graph3DData> spectrogram, std::shared_ptr<Graph2DData> data,
                       qint64 width, qint64 height, qint64 N, qint64 krat,
                       qint64 start_slice, qint64 end_slice);

public:
    static std::shared_ptr<Graph3DData> transform(std::shared_ptr<Graph2DData> data, qint64 width, qint64 height, double coeff = 1);

};

#endif
