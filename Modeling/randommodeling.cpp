#include "randommodeling.h"
#include <QtMath>

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
    std::normal_distribution<double> random(a, qSqrt(qAbs(dispersion)));

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

std::shared_ptr<Graph2DData> randomModeling::ARMA(
            const long long N, const double T, const double dispersion,
            const QList<double> as, const QList<double> bs
        )
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<double> random(0, qSqrt(qAbs(dispersion)));

    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Autoregression";
    data2D->source = "Autoregression with p = " + QString::number(as.size()) +
            " and q = " + QString::number(bs.size());

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    QList<double> rand_disp(N);
    for (size_t i = 0; i < N; ++i) {
        rand_disp[i] = random(rng);
    }

    data2D->samples.resize(N);
    for (qint64 i = 0; i < N; ++i) {
        data2D->samples[i] = rand_disp[i];
        for (qint64 j = 0; j < bs.size(); ++j) {
            if (i - j - 1 >= 0) {
                data2D->samples[i] += bs[j] * rand_disp[i - j - 1];
            }
        }
        for (qint64 j = 0; j < as.size(); ++j) {
            if (i - j - 1 >= 0) {
                data2D->samples[i] -= as[j] * data2D->samples[i - j - 1];
            }
        }
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}
