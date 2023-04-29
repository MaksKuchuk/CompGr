#include "randommodeling.h"

std::shared_ptr<Graph2DData> randomModeling::whiteNoise(
        const long long N, const double T, const double a, const double b
        )
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> random(a, b);

    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Uniform white noise";
    data2D->source = "White noise in range from " + QString::number(a) + " to " + QString::number(b);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        data2D->samples[i] = random(rng);
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> randomModeling::normalDistrWhiteNoise(
        const long long N, const double T, const double a, const double dispersion
        )
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<double> random(a, dispersion);

    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Normal white noise";
    data2D->source = "Normal White noise with mean = " + QString::number(a) +
            " and dispersion = " + QString::number(dispersion);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        data2D->samples[i] = random(rng);
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;

}

std::shared_ptr<Graph2DData> randomModeling::MAOA(
            const long long N, const double T, const double p, const double q, const double dispersion,
            const double* as, const double* bs
        )
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<double> random(0, dispersion);

    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Autoregression";
    data2D->source = "Autoregression with p = " + QString::number(p) +
            " and q = " + QString::number(q);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double* xn = new double[N];

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        data2D->samples[i] = random(rng);
        xn[i] = data2D->samples[i];
        for (int j = 0; j < q; ++j) {
            if (i - j < 0)
                break;
            data2D->samples[i] += bs[j] * xn[i - j];
        }
        for (int j = 0; j < p; ++j) {
            if (i - j < 0)
                break;
            data2D->samples[i] -= as[j] * data2D->samples[i - j];
        }
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    delete[] xn;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}
