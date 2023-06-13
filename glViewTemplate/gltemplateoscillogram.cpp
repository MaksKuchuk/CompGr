#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glOscillogram.hpp"
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

void glTemplateOscillogram::paintEvent(QPaintEvent *event) {
    int margin = 65; // adjust as needed
    int xAxisY = gView->height() + gView->y();
    int yAxisX = margin;
    QPainter painter(this);
    painter.drawLine(yAxisX, 5, yAxisX, xAxisY); // draw the y-axis
    painter.drawLine(yAxisX, xAxisY, gView->width() - margin, xAxisY); // draw the x-axis

    int tickLength = 5; // adjust as needed
    int numTicks = 10; // adjust as needed
    int xTickSpacing = 100;
    int yTickSpacing = 60;
    int numTicksX = gView->width() / xTickSpacing + 1;
    int numTicksY = gView->height() / yTickSpacing + 1;

    auto xScaler = &Utility::LinearScale;
    if (xLogScale)
        xScaler = &Utility::ExpScale;

    auto yScaler = &Utility::LinearScale;
    if (yLogScale)
        yScaler = &Utility::ExpScale;

    for (int i = 0; i <= numTicksX; i++) {
        // draw tick on x-axis
        int x = (yAxisX + i * gView->width() / numTicksX);
        painter.drawLine(x, xAxisY, x, xAxisY + tickLength);

        // add tick labels
        auto num = xScaler(i, numTicksX, data->lcur, data->rcur);
        QString xTickLabel;
        if (isTimeStep)
            xTickLabel = freqToTime(num / data->Hz);
        else
            xTickLabel = QString::number(num / data->Hz, 'g', 3);

        painter.drawText(x - tickLength / 2, xAxisY + tickLength + 10, xTickLabel);
    }

    for (int i = 0; i <= numTicksY; i++) {
        // draw tick on y-axis
        int y = xAxisY - i * gView->height() / numTicksY;
        painter.drawLine(yAxisX - tickLength, y, yAxisX, y);

        // add tick labels
        auto num = yScaler(i, numTicksY, data->minLoc, data->maxLoc);
        QString yTickLabel = QString::number(num, 'g', 3);
        painter.drawText(yAxisX - tickLength - 5 - painter.fontMetrics()
                         .horizontalAdvance(yTickLabel),
                         y + painter.fontMetrics().height() / 2, yTickLabel);
    }
}

QString glTemplateOscillogram::freqToTime(double freq) {
    if (freq <= 0)
        return "-";

    auto time = 1/freq;

    constexpr double secInHour = 3600;
    constexpr double secInDay = secInHour * 24;
    constexpr double secInYear = secInDay * 365;

    if (time > secInYear)
        return QString::number(time / secInYear, 'g', 3)+" y";
    if (time > 7 * secInDay)
        return QString::number(time / secInDay, 'g', 3)+" d";
    if (time > secInHour)
        return QString::number(time / secInHour, 'g', 3)+" h";
    if (time > 3 * 60)
        return QString::number(time / 60, 'g', 3)+" m";

    if (time > 1)
        return QString::number(time, 'g', 3)+" s";
    if (time > 1e-3)
        return QString::number(time * 1e3, 'g', 3)+" ms";

    return QString::number(time * 1e6, 'g', 3)+" mcs";

}

void glTemplateOscillogram::ChangeInfoLabel() {
    infoLabel->setText("Samples: "+QString::number(data->rcur - data->lcur+1));
}


glTemplateOscillogram::glTemplateOscillogram(QWidget *parent, std::shared_ptr<Graph2DData> data,
                                             QPointer<GraphTemplate> templ_, glType oscType) :
    QWidget(parent),
    ui(new Ui::glTemplateOscillogram),
    data(data),
    templ(templ_),
    type(oscType)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    gView = new glOscillogram(this, data);

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

    infoLabel = new QLabel("Samples: "+QString::number(data->rcur - data->lcur+1),this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setFixedHeight(14);

    layout->addWidget(gView);
    layout->addSpacing(12);
    layout->addWidget(infoLabel);
    layout->addWidget(label);

    scrollBar = new QScrollBar(Qt::Horizontal, this);
    layout->addWidget(scrollBar);
    ChangeScrollBar();

    setLayout(layout);

    connect(scrollBar, &QScrollBar::valueChanged, this, &glTemplateOscillogram::ScrollBarChanged);

    connect(this, &glTemplateOscillogram::BiasChanged, this, &glTemplateOscillogram::ChangeScrollBar);

    connect(this, &glTemplateOscillogram::BiasChanged, this, &glTemplateOscillogram::ChangeInfoLabel);

    connect(this, &glTemplateOscillogram::BiasChanged, gView, &glOscillogram::updateGraph);
    connect(this, &glTemplateOscillogram::BiasChanged, [&](){ repaint(); });

    connect(this, &glTemplateOscillogram::ScaleChanged, gView, &glOscillogram::updateGraph);
    connect(this, &glTemplateOscillogram::ScaleChanged, [&](){ repaint(); });

    if (parent != nullptr)
        connect(parent, &QWidget::destroyed, this, &QWidget::close);
}

void glTemplateOscillogram::drawMenu(QPoint globalPos) {
    QMenu *menu = new QMenu();

    if (type == glType::Oscillogram) {
        QAction* action1 = new QAction(QString::fromUtf8("Close"), this);
        menu->addAction(action1);
        menu->addSeparator();
    }
    QAction* action2 = new QAction(QString::fromUtf8("Local scale"), this);
    menu->addAction(action2);
    QAction* action3 = new QAction(QString::fromUtf8("Global scale"), this);
    menu->addAction(action3);
    QAction* action4 = new QAction(QString::fromUtf8("Set scale"), this);
    menu->addAction(action4);
    menu->addSeparator();
    QAction* action5 = new QAction(QString::fromUtf8("Global bias"), this);
    menu->addAction(action5);
    QAction* action6 = new QAction(QString::fromUtf8("Set bias"), this);
    menu->addAction(action6);
    menu->addSeparator();

    if (type == glType::Oscillogram) {
        QAction* action7 = new QAction(QString::fromUtf8("Single local scale"), this);
        menu->addAction(action7);
        QAction* action8 = new QAction(QString::fromUtf8("Single global scale"), this);
        menu->addAction(action8);
        menu->addSeparator();
        QAction* action9 = new QAction(QString::fromUtf8("Default scale for all"), this);
        menu->addAction(action9);
        QAction* action10 = new QAction(QString::fromUtf8("Default bias for all"), this);
        menu->addAction(action10);
        menu->addSeparator();
    }

    QAction* scalex = new QAction(QString::fromUtf8("Log Scale X"), this);
    scalex->setCheckable(true);
    scalex->setChecked(xLogScale);
    menu->addAction(scalex);
    QAction* scaley = new QAction(QString::fromUtf8("Log Scale Y"), this);
    scaley->setCheckable(true);
    scaley->setChecked(yLogScale);
    menu->addAction(scaley);
    menu->addSeparator();

    QAction* timestep = new QAction(QString::fromUtf8("Time Step"), this);
    timestep->setCheckable(true);
    timestep->setChecked(isTimeStep);
    if (type == glType::FourierSpectrum) {
        menu->addAction(timestep);
    }


    QAction* selectedItem = menu->exec(globalPos);
    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Close") {
        close();
    } else if (selectedItem->text() == "Local scale") {
        setLocalScale();
    } else if (selectedItem->text() == "Global scale") {
        setGlobalScale();
    } else if (selectedItem->text() == "Set scale") {
        selectScale();
    } else if (selectedItem->text() == "Global bias") {
        setGlobalBias();
    } else if (selectedItem->text() == "Set bias") {
        selectBias();
    } else if (selectedItem->text() == "Single local scale") {
        selectSingleLocalScale();
    } else if (selectedItem->text() == "Single global scale") {
        selectSingleGlobalScale();
    } else if (selectedItem->text() == "Default scale for all") {
        setDefaultScale();
    } else if (selectedItem->text() == "Default bias for all") {
        setDefaultBias();
    } else if (selectedItem->text() == "Log Scale X") {
        xLogScale = scalex->isChecked();
    } else if (selectedItem->text() == "Log Scale Y") {
        yLogScale = scaley->isChecked();
    } else if (selectedItem->text() == "Time Step") {
        isTimeStep = timestep->isChecked();
    }
    gView->updateGraph();
}

void glTemplateOscillogram::SetBias(long long lcur, long long rcur) {
    data->lcur = lcur;
    data->rcur = rcur;
    emit BiasChanged(lcur, rcur);
}

void glTemplateOscillogram::SetScale(double bottom, double top) {
    data->minLoc = bottom;
    data->maxLoc = top;
    emit ScaleChanged(bottom, top);
}

void glTemplateOscillogram::ResetBias() {
    data->lcur = 0;
    data->rcur = data->amountOfSamples - 1;
    emit BiasChanged(data->lcur, data->rcur);
}
void glTemplateOscillogram::ResetScale() {
    data->minLoc = data->minVal;
    data->maxLoc = data->maxVal;
    emit ScaleChanged(data->minLoc, data->maxLoc);
}

void glTemplateOscillogram::setDefaultScale(){
    AnalyzeWidget::getInstance()->ResetScale();
}

void glTemplateOscillogram::setDefaultBias() {
    AnalyzeWidget::getInstance()->ResetBias();
}

void glTemplateOscillogram::selectSingleLocalScale() {
    AnalyzeWidget::getInstance()->SetSingleScale();
}

void glTemplateOscillogram::selectSingleGlobalScale() {
    AnalyzeWidget::getInstance()->SetGlobalScale();
}

void glTemplateOscillogram::selectScale() {
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

void glTemplateOscillogram::selectBias() {
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

        if (lx < 0 || rx >= data->amountOfSamples) return;

        SetBias(lx, rx);
    }
}

void glTemplateOscillogram::setGlobalBias() {
    SetBias(0, data->amountOfSamples - 1);
}

void glTemplateOscillogram::setLocalScale() {

    double lmi, lma;

    lmi = data->samples[data->lcur];
    lma = data->samples[data->lcur];
    for (long long i = data->lcur; i <= data->rcur; i++) {
        if (lmi > data->samples[i]) lmi = data->samples[i];
        if (lma < data->samples[i]) lma = data->samples[i];
    }

    SetScale(lmi, lma);
}

void glTemplateOscillogram::setGlobalScale() {
    SetScale(data->minVal, data->maxVal);
}

void glTemplateOscillogram::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        drawMenu(QCursor::pos());
    } else if (event->button() == Qt::LeftButton && AnalyzeWidget::scaleMod) {
        AnalyzeWidget::xpress = event->pos().x() - gView->geometry().x();
        AnalyzeWidget::ypress = event->pos().y() - gView->geometry().y();
    }
}

void glTemplateOscillogram::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && AnalyzeWidget::scaleMod) {
        if (AnalyzeWidget::xpress == -1 ||
            AnalyzeWidget::ypress == -1) return;

        AnalyzeWidget::xrelease = event->pos().x() - gView->geometry().x();
        AnalyzeWidget::yrelease = event->pos().y() - gView->geometry().y();

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

        AnalyzeWidget::xleft = std::min(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gView->geometry().width();
        AnalyzeWidget::xright = std::max(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gView->geometry().width();
        AnalyzeWidget::ybottom = 1 - std::max(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gView->geometry().height();
        AnalyzeWidget::ytop = 1 - std::min(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gView->geometry().height();

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
            || newlcur < 0 || newrcur >= data->amountOfSamples || (newrcur - newlcur < 10)) {
            AnalyzeWidget::xpress = -1;
            AnalyzeWidget::ypress = -1;
            AnalyzeWidget::xrelease = -1;
            AnalyzeWidget::yrelease = -1;
            gView->updateGraph();
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

void glTemplateOscillogram::mouseMoveEvent(QMouseEvent* event) {
    if (AnalyzeWidget::scaleMod) {
        AnalyzeWidget::xrelease = event->pos().x() - gView->geometry().x();
        AnalyzeWidget::yrelease = event->pos().y() - gView->geometry().y();

        AnalyzeWidget::xleft = std::min(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gView->geometry().width();
        AnalyzeWidget::xright = std::max(AnalyzeWidget::xpress, AnalyzeWidget::xrelease) / (double)gView->geometry().width();
        AnalyzeWidget::ybottom = 1 - std::max(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gView->geometry().height();
        AnalyzeWidget::ytop = 1 - std::min(AnalyzeWidget::ypress, AnalyzeWidget::yrelease) / (double)gView->geometry().height();

        gView->updateGraph();
        repaint();
    }
}

glTemplateOscillogram::~glTemplateOscillogram()
{
    delete ui;
}

void glTemplateOscillogram::ChangeScrollBar() {
    scrollBar->setRange(0, data->amountOfSamples - (data->rcur - data->lcur) - 1);
    scrollBar->setValue(data->lcur);
}

void glTemplateOscillogram::ScrollBarChanged() {
    long long lc = scrollBar->value();

    long long diff = data->rcur - data->lcur;

    long long rc = lc + diff;

    if (rc >= data->amountOfSamples) {
        auto diff = rc - data->amountOfSamples + 1;
        lc -= diff;
        rc -= diff;
    }

    SetBias(lc, rc);
}


double glTemplateOscillogram::scrollF(long long x) {
    const double e = 2.718281828459045;
    //return pow(log(x + 3) / log(2), 2) + 1;
    return x / 16 + 1;
}

void glTemplateOscillogram::scrollGraph(long long y) {
    if ((y > 0) && (data->rcur - data->lcur < 10)) return;

    auto lcur = data->lcur;
    auto rcur = data->rcur;
    auto amountOfSamples = data->amountOfSamples;

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

void glTemplateOscillogram::moveGraph(long long y) {

    auto lcur = data->lcur;
    auto rcur = data->rcur;
    auto amountOfSamples = data->amountOfSamples;

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

void glTemplateOscillogram::keyPressEvent(QKeyEvent* event) {
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

void glTemplateOscillogram::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        AnalyzeWidget::scaleMod = false;
    }
}

void glTemplateOscillogram::wheelEvent(QWheelEvent *event) {
    scrollGraph(event->angleDelta().y());
}


void glTemplateOscillogram::closeEvent(QCloseEvent *event)
{
    if (type == glType::Oscillogram) {
        AnalyzeWidget::isMultipleBiasStarted = true;
        SetBias(0, data->amountOfSamples-1);
        AnalyzeWidget::isMultipleBiasStarted = false;
    }

    if (templ != nullptr && templ->gView != nullptr)
        disconnect(this, &glTemplateOscillogram::BiasChanged, templ->gView, &glView::setCurs);
}

