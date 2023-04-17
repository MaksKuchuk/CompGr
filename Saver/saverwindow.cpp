#include "saverwindow.h"
#include "../mainwindow.h"
#include "../GraphGlData/generaldata.h"

#include <utility>

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFormLayout>
#include <QCheckBox>
#include <QString>

SaverDataHandler SaverWindow::openWindow(std::shared_ptr<GeneralData> data) {
    return openWindow(data->channels_names, data->amountOfChannels, 0, data->amountOfSamples - 1);
}

SaverDataHandler SaverWindow::openWindow(std::shared_ptr<GeneralData> data, const size_t first_sample, const size_t last_sample) {
    return openWindow(data->channels_names, data->amountOfChannels, first_sample, last_sample);
}

SaverDataHandler SaverWindow::openWindow(const QList<QString>& names, const size_t names_count, const size_t first_sample, const size_t last_sample) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Save graphs"));

    QLineEdit *fsamp = new QLineEdit(QString::number(first_sample), &dlg);
    QLineEdit *lsamp = new QLineEdit(QString::number(last_sample), &dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    MainWindow::connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    MainWindow::connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("From sample"), fsamp);
    layout->addRow(tr("To sample"), lsamp);


    auto checkBoxes = new QCheckBox*[names_count];
    for (size_t i = 0; i < names_count; ++i) {
        checkBoxes[i] = new QCheckBox(QString::number(i) + ": " + names[i], &dlg);
        checkBoxes[i]->setCheckState(Qt::CheckState::Checked);
        if (i == 0)
            layout->addRow(tr("Include channels"), checkBoxes[i]);
        else
            layout->addWidget(checkBoxes[i]);
    }

    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    SaverDataHandler outData;

    if(dlg.exec() == QDialog::Accepted) {
        outData.first_sample = fsamp->text().toULongLong();
        outData.last_sample = lsamp->text().toULongLong() + 1;
        for (size_t i = 0 ; i < names_count; ++i) {
            if (checkBoxes[i]->isChecked())
                outData.channels.push_back(i);
        }
        outData.success = true;
    }

    delete[] checkBoxes;

    return outData;
}
