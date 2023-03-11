#include "graphinfo.h"
#include "ui_graphinfo.h"

#include <QVBoxLayout>
#include <QLabel>

GraphInfo::GraphInfo(QWidget *parent, ParseData *pData) :
    QWidget(parent),
    ui(new Ui::GraphInfo)
{
    ui->setupUi(this);


    QLabel *amountOfChannels = new QLabel(this);
    amountOfChannels->setText(
                "Number of channels: " +
                QString::number(pData->getAmountOfChannels())
                );

    QFont font = amountOfChannels->font();
    font.setPixelSize(18);

    amountOfChannels->setFont(font);
    amountOfChannels->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    amountOfChannels->setFixedHeight(22);


    QLabel *amountOfSamples = new QLabel(this);
    amountOfSamples->setText(
                "Number of samplees: " +
                QString::number(pData->getAmountOfSamples())
                );
    amountOfSamples->setFont(font);
    amountOfSamples->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    amountOfSamples->setFixedHeight(22);


    QLabel *Hz = new QLabel(this);
    Hz->setText(
                "Sampling frequency: " +
                QString::number(pData->getHz())
                );
    Hz->setFont(font);
    Hz->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    Hz->setFixedHeight(22);


    QLabel *startTime = new QLabel(this);
    startTime->setText(
                "Start time: " +
                QString::fromUtf8(pData->getStartTime())
                );
    startTime->setFont(font);
    startTime->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    startTime->setFixedHeight(22);


    QLabel *stopTime = new QLabel(this);
    stopTime->setText(
                "Stop time: " +
                QString::fromUtf8(pData->getStopTime())
                );
    stopTime->setFont(font);
    stopTime->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    stopTime->setFixedHeight(22);

    QLabel *duration = new QLabel(this);
    duration->setText(
                "Duration: " +
                QString::number(std::get<0>(pData->getDuration())) + " days " +
                QString::number(std::get<1>(pData->getDuration())) + " hours " +
                QString::number(std::get<2>(pData->getDuration())) + " minutes " +
                QString::number(std::get<3>(pData->getDuration())) + " seconds "
                );
    duration->setFont(font);
    duration->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    duration->setFixedHeight(22);


    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(amountOfChannels);
    layout->addWidget(amountOfSamples);
    layout->addWidget(Hz);
    layout->addWidget(startTime);
    layout->addWidget(stopTime);
    layout->addWidget(duration);

    for (int i = 0; i < pData->getAmountOfChannels(); i++) {
        QLabel *cName = new QLabel(this);
        cName->setText(
                        "    " +
                        QString::fromUtf8(pData->getChannelName(i)) +
                        ": from " +
                        QString::fromUtf8(pData->getFileName())
                    );
        cName->setFont(font);
        cName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        cName->setFixedHeight(22);


        layout->addWidget(cName);
    }

    layout->addSpacing(5);
    //layout->addStretch();


    this->setLayout(layout);

}

GraphInfo::~GraphInfo()
{
    delete ui;
}
