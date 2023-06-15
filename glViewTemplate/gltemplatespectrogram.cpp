#include "gltemplatespectrogram.h"

#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glspectrogram.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include "../analyzewidget.h"
#include "../Utility/config.h"
#include "../Utility/utility.h"
#include "../graphtemplate.h"


#include <QMenu>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <iostream>

glTemplateSpectrogram::glTemplateSpectrogram(QWidget *parent,
                                             std::shared_ptr<Graph3DData> data) :
    QWidget(parent),
    ui(new Ui::glTemplateOscillogram),
    data(data)
{   
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    gSpec = new glSpectrogram(this, data);
    gSpec->setMinimumSize(100,100);

    QLabel *label = new QLabel(this);
    label->setText(data->name);

    QFont font = label->font();
    font.setPixelSize(24);
    label->setFont(font);

    label->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    label->setFixedHeight(24);
    label->setContentsMargins(0,0,0,0);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(65, 5, 35, 5);

    layout->addWidget(gSpec);
    layout->addSpacing(10);
    layout->addWidget(label);

    scrollBar = new QScrollBar(Qt::Horizontal, this);
    layout->addWidget(scrollBar);
    ChangeScrollBar();

    slider = new QSlider(Qt::Horizontal, this);
    layout->addWidget(slider);
    slider->setTracking(true);
    slider->setRange(0,99);
    slider->setValue(0);


    setLayout(layout);

    auto grUpdate = [&](){
        gSpec->updateGraph();
        repaint();
    };

    connect(slider, &QSlider::valueChanged, [&](int pos){
        brightness = 1 - pos / 100.;
    });
    connect(slider, &QSlider::valueChanged, grUpdate);

    connect(scrollBar, &QScrollBar::valueChanged, this, &glTemplateSpectrogram::ScrollBarChanged);

    connect(this, &glTemplateSpectrogram::BiasChanged, this, &glTemplateSpectrogram::ChangeScrollBar);

    connect(this, &glTemplateSpectrogram::BiasChanged, grUpdate);

    connect(this, &glTemplateSpectrogram::ScaleChanged, grUpdate);

    connect(slider, &QSlider::sliderReleased, grUpdate);

    if (parent != nullptr)
        connect(parent, &QWidget::destroyed, this, &QWidget::close);

}

void glTemplateSpectrogram::paintEvent(QPaintEvent *event) {
    int margin = 65; // adjust as needed
    int xAxisY = gSpec->height() + gSpec->y();
    int yAxisX = margin;
    QPainter painter(this);
    painter.drawLine(yAxisX, 5, yAxisX, xAxisY); // draw the y-axis
    painter.drawLine(yAxisX, xAxisY, gSpec->width() - margin, xAxisY); // draw the x-axis

    int tickLength = 5; // adjust as needed
    int numTicks = 10; // adjust as needed
    int xTickSpacing = 100;
    int yTickSpacing = 60;
    int numTicksX = gSpec->width() / xTickSpacing + 1;
    int numTicksY = gSpec->height() / yTickSpacing + 1;

    auto xScaler = &Utility::LinearScale;
    if (xLogScale)
        xScaler = &Utility::ExpScale;

    auto yScaler = &Utility::LinearScale;
    if (yLogScale)
        yScaler = &Utility::ExpScale;

    for (int i = 0; i <= numTicksX; i++) {
        // draw tick on x-axis
        int x = (yAxisX + i * gSpec->width() / numTicksX);
        painter.drawLine(x, xAxisY, x, xAxisY + tickLength);

        // add tick labels
        auto num = xScaler(i, numTicksX, data->lcur, data->rcur);
        QString xTickLabel = QString::number(num / data->Hz, 'g', 3);

        painter.drawText(x - tickLength / 2, xAxisY + tickLength + 10, xTickLabel);
    }

    for (int i = 0; i <= numTicksY; i++) {
        // draw tick on y-axis
        int y = xAxisY - i * gSpec->height() / numTicksY;
        painter.drawLine(yAxisX - tickLength, y, yAxisX, y);

        // add tick labels
        auto num = yScaler(i, numTicksY, 0, data->height);
        QString yTickLabel = QString::number(num, 'g', 3);
        painter.drawText(yAxisX - tickLength - 5 - painter.fontMetrics()
                         .horizontalAdvance(yTickLabel),
                         y + painter.fontMetrics().height() / 2, yTickLabel);
    }
}

void glTemplateSpectrogram::drawMenu(QPoint globalPos) {
    QMenu *menu = new QMenu();

    QAction* action2 = new QAction(QString::fromUtf8("Local scale"), this);
//    menu->addAction(action2);
    QAction* action3 = new QAction(QString::fromUtf8("Global scale"), this);
//    menu->addAction(action3);
    QAction* action4 = new QAction(QString::fromUtf8("Set scale"), this);
//    menu->addAction(action4);
    menu->addSeparator();
    QAction* action5 = new QAction(QString::fromUtf8("Global bias"), this);
    menu->addAction(action5);
    QAction* action6 = new QAction(QString::fromUtf8("Set bias"), this);
    menu->addAction(action6);
    menu->addSeparator();


    QAction* scalex = new QAction(QString::fromUtf8("Log Scale X"), this);
    scalex->setCheckable(true);
    scalex->setChecked(xLogScale);
    menu->addAction(scalex);
    QAction* scaley = new QAction(QString::fromUtf8("Log Scale Y"), this);
    scaley->setCheckable(true);
    scaley->setChecked(yLogScale);
    menu->addAction(scaley);
    menu->addSeparator();

//    QAction* timestep = new QAction(QString::fromUtf8("Time Step"), this);
//    timestep->setCheckable(true);
//    timestep->setChecked(isTimeStep);
//    if (type == glType::FourierSpectrum) {
//        menu->addAction(timestep);
//    }


    QAction* selectedItem = menu->exec(globalPos);
    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Local scale") {
        setLocalScale();
    } else if (selectedItem->text() == "Global scale") {
        setGlobalScale();
    } else if (selectedItem->text() == "Set scale") {
        selectScale();
    } else if (selectedItem->text() == "Global bias") {
        setGlobalBias();
    } else if (selectedItem->text() == "Set bias") {
        selectBias();
    } else if (selectedItem->text() == "Log Scale X") {
        xLogScale = scalex->isChecked();
    } else if (selectedItem->text() == "Log Scale Y") {
        yLogScale = scaley->isChecked();
    }
//    else if (selectedItem->text() == "Time Step") {
//        isTimeStep = timestep->isChecked();
//    }
    gSpec->updateGraph();
}

void glTemplateSpectrogram::SetBias(long long lcur, long long rcur) {
    data->lcur = lcur;
    data->rcur = rcur;
    emit BiasChanged(lcur, rcur);
}

void glTemplateSpectrogram::SetScale(double bottom, double top) {
    data->minLoc = bottom;
    data->maxLoc = top;
    emit ScaleChanged(bottom, top);
}

void glTemplateSpectrogram::ResetBias() {
    data->lcur = 0;
    data->rcur = data->width - 1;
    emit BiasChanged(data->lcur, data->rcur);
}
void glTemplateSpectrogram::ResetScale() {
    data->minLoc = data->minVal;
    data->maxLoc = data->maxVal;
    emit ScaleChanged(data->minLoc, data->maxLoc);
}

void glTemplateSpectrogram::selectScale() {
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Y scale"));

    QLineEdit *ledit1 = new QLineEdit(&dlg);
    QLineEdit *ledit2 = new QLineEdit(&dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Y maximum"), ledit1);
    layout->addRow(tr("Y minimum:"), ledit2);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted) {
        if (data == nullptr) return;

        SetScale(ledit2->text().toDouble(), ledit1->text().toDouble());

    }
}

void glTemplateSpectrogram::selectBias() {
    QDialog dlg(this);
    dlg.setWindowTitle(tr("X bias"));

    QLineEdit *ledit1 = new QLineEdit(&dlg);
    QLineEdit *ledit2 = new QLineEdit(&dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("X left seconds"), ledit1);
    layout->addRow(tr("X right seconds"), ledit2);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted) {
        if (data == nullptr || ledit1->text().toDouble() >= ledit2->text().toDouble()) return;

        long long lx = ledit1->text().toDouble() * data->Hz;
        long long rx = ledit2->text().toDouble() * data->Hz;

        if (lx < 0 || rx >= data->width) return;

        SetBias(lx, rx);
    }
}

void glTemplateSpectrogram::setGlobalBias() {
    SetBias(0, data->width - 1);
}

void glTemplateSpectrogram::setLocalScale() {

    return;
    double lmi, lma;

//    lmi = data->samples[data->lcur];
//    lma = data->samples[data->lcur];
//    for (long long i = data->lcur; i <= data->rcur; i++) {
//        if (lmi > data->samples[i]) lmi = data->samples[i];
//        if (lma < data->samples[i]) lma = data->samples[i];
//    }

    SetScale(lmi, lma);
}

void glTemplateSpectrogram::setGlobalScale() {
//    SetScale(data->minVal, data->maxVal);
}

void glTemplateSpectrogram::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        drawMenu(QCursor::pos());
    } else if (event->button() == Qt::LeftButton && AnalyzeWidget::scaleMod) {
        AnalyzeWidget::xpress = event->pos().x() - gSpec->geometry().x();
        AnalyzeWidget::ypress = event->pos().y() - gSpec->geometry().y();
    }
}

void glTemplateSpectrogram::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && AnalyzeWidget::scaleMod) {
        if (AnalyzeWidget::xpress == -1 ||
            AnalyzeWidget::ypress == -1) return;

        AnalyzeWidget::xrelease = event->pos().x() - gSpec->geometry().x();
        AnalyzeWidget::yrelease = event->pos().y() - gSpec->geometry().y();

//        std::cout << gView->geometry().x() << ' '
//                  << gView->geometry().y() << "\n"
//                  << gView->geometry().width() << ' '
//                  << gView->geometry().height() << "\n";

//        std::cout << AnalysisWindowHandler::xpress
//                  << ' '
//                  << AnalysisWindowHandler::ypress
//                  << '\n'
//                  << AnalysisWindowHandler::xrelease
//                  << ' '
//                  << AnalysisWindowHandler::yrelease
//                  << "\n\n" << std::endl;

        AnalyzeWidget::xleft = std::min(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gSpec->geometry().width();
        AnalyzeWidget::xright = std::max(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gSpec->geometry().width();
        AnalyzeWidget::ybottom = 1 - std::max(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gSpec->geometry().height();
        AnalyzeWidget::ytop = 1 - std::min(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gSpec->geometry().height();

//        std::cout << AnalysisWindowHandler::xleft << ' ' << AnalysisWindowHandler::xright
//                  << ' ' << AnalysisWindowHandler::ybottom << ' ' << AnalysisWindowHandler::ytop << std::endl;

        auto xScaler = &Utility::LinearScale;
        if (xLogScale)
            xScaler = &Utility::ExpScale;

        auto yScaler = &Utility::LinearScale;
        if (yLogScale)
            yScaler = &Utility::ExpScale;

        double newmaxLoc = yScaler(AnalyzeWidget::ytop, 1.0, data->minLoc, data->maxLoc);
        double newminLoc = yScaler(AnalyzeWidget::ybottom, 1.0, data->minLoc, data->maxLoc);
        qint64 newlcur = xScaler(AnalyzeWidget::xleft, 1.0, data->lcur, data->rcur);
        qint64 newrcur = xScaler(AnalyzeWidget::xright, 1.0, data->lcur, data->rcur);

//        double newmaxLoc = AnalyzeWidget::ytop * (data->maxLoc - data->minLoc) + data->minLoc;
//        double newminLoc = AnalyzeWidget::ybottom * (data->maxLoc - data->minLoc) + data->minLoc;
//        qint64 newlcur = AnalyzeWidget::xleft * (data->rcur - data->lcur) + data->lcur;
//        qint64 newrcur = AnalyzeWidget::xright * (data->rcur - data->lcur) + data->lcur;

//        std::cout << newmaxLoc << ' ' << newminLoc << ' ' << newlcur << ' ' << newrcur << std::endl;

        if (data == nullptr || newmaxLoc > data->maxVal || newminLoc < data->minVal
            || newlcur < 0 || newrcur >= data->width || (newrcur - newlcur < 10)) {
            AnalyzeWidget::xpress = -1;
            AnalyzeWidget::ypress = -1;
            AnalyzeWidget::xrelease = -1;
            AnalyzeWidget::yrelease = -1;
            gSpec->updateGraph();
            this->repaint();
            return;
        }

        //data->maxLoc = newmaxLoc;
        //data->minLoc = newminLoc;
//        data->lcur = newlcur;
//        data->rcur = newrcur;
        SetBias(newlcur, newrcur);

        AnalyzeWidget::xpress = -1;
        AnalyzeWidget::ypress = -1;
        AnalyzeWidget::xrelease = -1;
        AnalyzeWidget::yrelease = -1;
    }
}

void glTemplateSpectrogram::mouseMoveEvent(QMouseEvent* event) {
    if (AnalyzeWidget::scaleMod) {
        AnalyzeWidget::xrelease = event->pos().x() - gSpec->geometry().x();
        AnalyzeWidget::yrelease = event->pos().y() - gSpec->geometry().y();

        AnalyzeWidget::xleft = std::min(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gSpec->geometry().width();
        AnalyzeWidget::xright = std::max(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gSpec->geometry().width();
        AnalyzeWidget::ybottom = 1 - std::max(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gSpec->geometry().height();
        AnalyzeWidget::ytop = 1 - std::min(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gSpec->geometry().height();

        gSpec->updateGraph();
        repaint();
    }
}

glTemplateSpectrogram::~glTemplateSpectrogram()
{
    delete ui;
}

void glTemplateSpectrogram::ChangeScrollBar() {
    scrollBar->setRange(0, data->width - (data->rcur - data->lcur) - 1);
    scrollBar->setValue(data->lcur);
}

void glTemplateSpectrogram::ScrollBarChanged() {
    long long lc = scrollBar->value();

    long long diff = data->rcur - data->lcur;

    long long rc = lc + diff;

    if (rc >= data->width) {
        auto diff = rc - data->width + 1;
        lc -= diff;
        rc -= diff;
    }

    SetBias(lc, rc);
}


double glTemplateSpectrogram::scrollF(long long x) {
    const double e = 2.718281828459045;
    //return pow(log(x + 3) / log(2), 2) + 1;
    return x / 16 + 1;
}

void glTemplateSpectrogram::scrollGraph(long long y) {
    if ((y > 0) && (data->rcur - data->lcur < 10)) return;

    auto lcur = data->lcur;
    auto rcur = data->rcur;
    auto amountOfSamples = data->width;

    lcur += 0.01 * y * scrollF(static_cast<double>(rcur - lcur));
    rcur -= 0.01 * y * scrollF(static_cast<double>(rcur - lcur));

    if (lcur < 0) lcur = 0;
    if (rcur < 0) rcur = 0;
    if (lcur >= amountOfSamples)
        lcur = amountOfSamples - 1;
    if (rcur >= amountOfSamples)
        rcur = amountOfSamples - 1;

    if (rcur - lcur < 10) {
        if (lcur + 10 < amountOfSamples) {
            rcur = lcur + 9;
        } else if (rcur - 10 >= 0) {
            lcur = rcur - 9;
        } else {
            lcur = 0;
            rcur = amountOfSamples - 1;
        }
    }

    SetBias(lcur, rcur);
}

void glTemplateSpectrogram::moveGraph(long long y) {

    auto lcur = data->lcur;
    auto rcur = data->rcur;
    auto amountOfSamples = data->width;

    long long ch = y * scrollF(static_cast<double>(rcur - lcur));

    if (lcur + ch < 0) {
        ch = -lcur;
    }
    if (rcur + ch >= amountOfSamples) {
        ch = amountOfSamples - rcur - 1;
    }

    lcur += ch;
    rcur += ch;

    SetBias(lcur, rcur);
}

void glTemplateSpectrogram::keyPressEvent(QKeyEvent* event) {
    auto code = event->nativeScanCode();
    //38 - a, 40 - d
    if (code == 30 || code == 32
            || code == 38 || code == 40) {
        long long x = (code == 38 || code == 30) ? -1 : 1;

        moveGraph(x);
    }

    if (event->key() == Qt::Key_Alt) {
        AnalyzeWidget::scaleMod = true;
    }
}

void glTemplateSpectrogram::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        AnalyzeWidget::scaleMod = false;
    }
}

void glTemplateSpectrogram::wheelEvent(QWheelEvent *event) {
    scrollGraph(event->angleDelta().y());
}
