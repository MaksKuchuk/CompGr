#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsLayout>
#include <QPushButton>

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QValueAxis>


#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(QWidget *parent, const std::shared_ptr<Graph2DData> graphData)
    : QWidget{parent},
      statistics(new Statistics(graphData))
{
    statistics->calcHistogramm();

    auto layout = new QVBoxLayout;

    auto setLabel = [&](const QString pretext, double value) {
        auto label = new QLabel(pretext+QString::number(value), this);
        layout->addWidget(label);
        return label;
    };

    meanLabel = setLabel("Mean: ", statistics->mean);
    dispersionLabel = setLabel("Dispersion: ", statistics->dispersion);
    standardDeviationLabel = setLabel("Standard deviation: ", statistics->standardDeviation);
    variationCoefficientLabel = setLabel("Variation: ", statistics->variationCoefficient);
    asymmetricCoefficientLabel = setLabel("Asymmetric: ", statistics->asymmetricCoefficient);
    kurtosisCoefficientLabel = setLabel("Kurtosis (Excess): ", statistics->kurtosisCoefficient);
    minValLabel = setLabel("Min: ", statistics->minVal);
    maxValLabel = setLabel("Max: ", statistics->maxVal);
    quintile05Label =setLabel("Quintile05: ", statistics->quintile05);
    quintile95Label = setLabel("Quintile95: ", statistics->quintile95);
    medianLabel = setLabel("Median: ", statistics->median);

    histChart.set = new QBarSet("", this);
    double maxInHist = 0;
    for (auto x : statistics->histogram) {
        *histChart.set << x;
        maxInHist = std::max(maxInHist, x);
    }
    histChart.series = new QBarSeries();
    histChart.series->append(histChart.set);
    histChart.series->setBarWidth(1);

    histChart.chart = new QChart();
    histChart.chart->addSeries(histChart.series);
//    chart->setTitle("Simple barchart example");
//    chart->setAnimationOptions(QChart::SeriesAnimations);

    histChart.chart->setLocalizeNumbers(true);
    histChart.chart->setContentsMargins(-10,-10,-10,-10);
    histChart.chart->layout()->setContentsMargins(0,0,0,0);
    histChart.chart->legend()->hide();

    histChart.axisX = new QValueAxis;
    histChart.axisX->setRange(statistics->minVal, statistics->maxVal);
    auto xCount = statistics->histogram.size() <= 20 ? statistics->histogram.size()+1 : 20;
    histChart.axisX->setTickCount(xCount);
    histChart.chart->addAxis(histChart.axisX, Qt::AlignBottom);

    histChart.axisY = new QValueAxis;
    histChart.axisY->setRange(0, maxInHist);
    histChart.chart->addAxis(histChart.axisY, Qt::AlignLeft);

    histChart.chartView = new QChartView(histChart.chart);
    histChart.chartView->setRenderHint(QPainter::Antialiasing);
    histChart.chartView->setMinimumSize(320, 240);

    layout->addWidget(histChart.chartView);

    numberOfDivisions = new QLineEdit(QString::number(statistics->histDivs), this);
    layout->addWidget(numberOfDivisions);

    auto buttonRecalc = new QPushButton("Recalculate", this);
    layout->addWidget(buttonRecalc);
    connect(buttonRecalc, &QPushButton::clicked, this, &StatisticsWidget::recalcualte);


    setLayout(layout);
    setWindowTitle("Statistics");

    isCorrect = true;
}

void StatisticsWidget::recalcualte() {
    statistics->Recalc(numberOfDivisions->text().toLongLong());

    auto setText = [](QPointer<QLabel> label, const QString pretext, double value) {
        label->setText(pretext+QString::number(value));
    };

    setText(meanLabel, "Mean: ", statistics->mean);
    setText(dispersionLabel, "Dispersion: ", statistics->dispersion);
    setText(standardDeviationLabel, "Standard deviation: ", statistics->standardDeviation);
    setText(variationCoefficientLabel, "Variation: ", statistics->variationCoefficient);
    setText(asymmetricCoefficientLabel, "Asymmetric: ", statistics->asymmetricCoefficient);
    setText(kurtosisCoefficientLabel, "Kurtosis (Excess): ", statistics->kurtosisCoefficient);
    setText(minValLabel, "Min: ", statistics->minVal);
    setText(maxValLabel, "Max: ", statistics->maxVal);
    setText(quintile05Label, "Quintile05: ", statistics->quintile05);
    setText(quintile95Label, "Quintile95: ", statistics->quintile95);
    setText(medianLabel, "Median: ", statistics->median);

    histChart.series->remove(histChart.set);
    histChart.chart->removeAllSeries();
    histChart.set = new QBarSet("", this);
    double maxInHist = 0;
    for (auto x : statistics->histogram) {
        *histChart.set << x;
        maxInHist = std::max(maxInHist, x);
    }

    histChart.series = new QBarSeries();
    histChart.series->append(histChart.set);
    histChart.series->setBarWidth(1);

    histChart.chart->addSeries(histChart.series);

    histChart.axisX->setRange(statistics->minVal, statistics->maxVal);
    auto xCount = statistics->histogram.size() <= 20 ? statistics->histogram.size()+1 : 20;
    histChart.axisX->setTickCount(xCount);

    histChart.axisY->setRange(0, maxInHist);

}
