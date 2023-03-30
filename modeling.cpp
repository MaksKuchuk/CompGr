#include "modeling.h"

Graph2DData* Modeling::delayedSingleImpulse(const long long N, const double T, const long long n) {
    Graph2DData* data2D = new Graph2DData();
    data2D->name = "Model_J_1";
    data2D->source = "Delayed Single Impulse n = " + std::to_string(n);

    data2D->maxVal = 1;
    data2D->minVal = 0;

    data2D->maxLoc = 1;
    data2D->minLoc = 0;
    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1) * T;

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double* samples = new double[N]();
    samples[n] = 1;
    data2D->samples = samples;

    return data2D;
}
