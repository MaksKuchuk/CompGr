#include "superpositiondialog.h"
#include "../GraphGlData/generaldata.h"
#include "../GraphGlData/Graph2DData.hpp"
#include "../mainwindow.h"
#include "../Modeling/superposition.h"
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLineEdit>

void SuperpositionDialog::LinSupDialog(std::shared_ptr<GeneralData> data) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Select Channels"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    MainWindow::connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    MainWindow::connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    auto coeff = new QLineEdit("0", &dlg);
    layout->addRow(new QLabel("Free C", &dlg), coeff);

    QList<QPointer<QLineEdit>> coeffsLines(data->amountOfChannels);
    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        coeffsLines[i] = new QLineEdit("1", &dlg);
        layout->addRow(new QLabel("C" + QString::number(i) + ": " + data->channels_names[i], &dlg),
                       coeffsLines[i]);
    }

    layout->addWidget(btn_box);

    dlg.setLayout(layout);
\
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QList<int> indicies;
    QList<double> coeffs;
    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        auto c = coeffsLines[i]->text().toDouble();
        if (c != 0) {
            indicies.push_back(i);
            coeffs.push_back(c);
        }
    }

    auto sup = Superposition::LinearSuperposition(data, indicies, coeffs);
    MainWindow::grWid->AddNewChannel(sup);
}

void SuperpositionDialog::MultSupDialog(std::shared_ptr<GeneralData> data) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Select Channels"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    MainWindow::connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    MainWindow::connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    auto coeff = new QLineEdit("1", &dlg);
    layout->addWidget(coeff);

    QList<QPointer<QCheckBox>> checkBoxes(data->amountOfChannels);
    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        checkBoxes[i] = new QCheckBox(QString::number(i) + ": " + data->channels_names[i], &dlg);
        checkBoxes[i]->setCheckState(Qt::CheckState::Checked);
//        if (i == 0)
//            layout->addRow(tr("Include channels"), checkBoxes[i]);
//        else
            layout->addWidget(checkBoxes[i]);
    }

    layout->addWidget(btn_box);

    dlg.setLayout(layout);
\
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QList<int> indicies;
    for (size_t i = 0; i < data->amountOfChannels; ++i) {
        if (checkBoxes[i]->isChecked())
            indicies.push_back(i);
    }

    auto sup = Superposition::MuliplicativeSuperposition(data, indicies, coeff->text().toDouble());
    MainWindow::grWid->AddNewChannel(sup);
}
