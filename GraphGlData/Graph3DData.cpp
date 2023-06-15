#include "Graph3DData.hpp"

Graph3DData::Graph3DData(qint64 wid, qint64 hei) {
    width = wid;
    height = hei;
    samples.resize(width);
    for (qint64 i = 0; i < width; ++i) {
        samples[i].resize(height);
    }

    lcur = 0;
    rcur = width-1;
}

Graph3DData::~Graph3DData() {
    for (qint64 i = 0; i < width; ++i) {
        samples[i].clear();
    }
    samples.clear();
}
