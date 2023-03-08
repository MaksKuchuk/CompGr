#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"

#include <QVBoxLayout>
#include <QLabel>

GraphTemplate::GraphTemplate(QWidget *parent,
                             std::string name,
                             long long parNum,
                             double* data,
                             double maxVal,
                             double minVal
                             ) :
    QWidget(parent),
    ui(new Ui::GraphTemplate)
{
    ui->setupUi(this);

    glView *gView = new glView(this, parNum, data, maxVal, minVal);
    gView->setFixedHeight(60);
    gView->setFixedWidth(300);

    QLabel *label = new QLabel(this);
    label->setText(QString::fromUtf8(name.c_str()));

    QFont font = label->font();
    font.setPixelSize(24);
    label->setFont(font);

    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(18);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(gView);
    layout->setSpacing(3);
    layout->addWidget(label);
    this->setLayout(layout);
}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
