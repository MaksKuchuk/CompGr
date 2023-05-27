#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include <QVBoxLayout>
#include <QEvent>
#include "glViewTemplate/gltemplateoscillogram.h"
#include <QScrollBar>

#include <iostream>

#include "Transformation/TransformToFourierSpectrum.hpp"
#include "Transformation/TransformToWaveletogram.hpp"

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget),
    layout(new QVBoxLayout())
{
    ui->setupUi(this);
    instance = this;

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setLayout(layout);
}

QPointer<AnalyzeWidget> AnalyzeWidget::getInstance() {
    if (instance == nullptr)
        instance = new AnalyzeWidget;
    return instance;
}

void AnalyzeWidget::analyze2DBy(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t) {
    QPointer<glTemplateOscillogram> gView;

    if (t == glType::Oscillogram) {
        gView = new glTemplateOscillogram(nullptr, data, templ);
        connect(gView, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
//        connect(gView, &glTemplateOscillogram::BiasChanged, [](qint64 a,qint64 b) {qDebug() << a << " " << b;});

    } else if (t == glType::FourierSpectrum) {
        gView = new glTemplateOscillogram(nullptr,
                                          TransformToFourierSpectrum::transform(data, 1,
                                                SpectrumModes::PSD, FourierModes::KEEP_FIRST_VAL), templ);

    } else if (t == glType::Waveletogram) {
        gView = new glTemplateOscillogram(nullptr, TransformToWaveletogram::transform(data), templ);


    }
    layout->addWidget(gView);
    connect(gView,  &glTemplateOscillogram::BiasChanged, gView->gView, &glOscillogram::updateGraph);
    gView->resize(300, 60);

    gView->show();
}

void AnalyzeWidget::analyze3DBy(std::shared_ptr<Graph2DData> data) {
    return;
}


AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
    delete layout;
}


void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
//    AnalysisWindowHandler::getInstance()->destroyWidget();
}

