#include "graphinfo.h"
#include "ui_graphinfo.h"

#include <QVBoxLayout>
#include <QLabel>

GraphInfo::GraphInfo(QWidget *parent, std::shared_ptr<GeneralData> pData) :
    QWidget(parent),
    ui(new Ui::GraphInfo)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(6, 2, 6, 2);

    QFont font;
    font.setPixelSize(18);

    auto setText = [&](QString text) {
        QLabel *label = new QLabel(this);
        label->setText(text);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        label->setFixedHeight(24);
        layout->addWidget(label);
    };

    setText("Number of channels: " + QString::number(pData->getAmountOfChannels()));
    setText("Number of samplees: " + QString::number(pData->getAmountOfSamples()));
    setText("Sampling frequency: " + QString::number(pData->getHz()));
    setText("Start time: " + pData->getStartTime());
    setText("Stop time: " + pData->getStopTime());
    setText("Duration: " +
            QString::number(std::get<0>(pData->getDuration())) + " days " +
            QString::number(std::get<1>(pData->getDuration())) + " hours " +
            QString::number(std::get<2>(pData->getDuration())) + " minutes " +
            QString::number(std::get<3>(pData->getDuration())) + " seconds "
    );

    for (int i = 0; i < pData->getAmountOfChannels(); i++) {
        QLabel *cName = new QLabel(this);
        setText("    " +
                pData->getChannelName(i) +
                ": from " +
                pData->getChannelSource(i));
    }

//    layout->addSpacing(5);
    //layout->addStretch();


    this->setLayout(layout);

}

GraphInfo::~GraphInfo()
{
    delete ui;
}
