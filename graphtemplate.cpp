#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"

#include <QVBoxLayout>
#include <QLabel>

GraphTemplate::GraphTemplate(QWidget *parent,
                             std::string name,
                             long long parNum,
                             double* data,
                             long long maxMod) :
    QWidget(parent),
    ui(new Ui::GraphTemplate)
{
    ui->setupUi(this);

    glView *gView = new glView(this, parNum, data, maxMod);
    gView->setFixedHeight(60);
    gView->setFixedWidth(200);

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
    layout->addWidget(label);
    this->setLayout(layout);
}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
