#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"

#include <QVBoxLayout>
#include <QLabel>

GraphTemplate::GraphTemplate(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphTemplate)
{
    ui->setupUi(this);

    glView *gView = new glView(this);

    QLabel *label = new QLabel(this);
    label->setText(name);

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
