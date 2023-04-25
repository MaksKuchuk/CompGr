#ifndef SUPERPOSITION_H
#define SUPERPOSITION_H

#include <memory>
#include <QList>
#include "../GraphGlData/Graph2DData.hpp"
#include "../GraphGlData/generaldata.h"

class Superposition
{
public:
    static std::shared_ptr<Graph2DData> LinearSuperposition(std::shared_ptr<GeneralData> data, QList<int> indices, QList<double> coeffs);

    static std::shared_ptr<Graph2DData> MuliplicativeSuperposition(std::shared_ptr<GeneralData> data, QList<int> indices, double coeff);
};

#endif // SUPERPOSITION_H
