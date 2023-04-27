#include "generaldialog.h"
#include "../mainwindow.h"
#include <QString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>

GeneralDialog::GeneralDialog() = default;

bool GeneralDialog::AgreeDialog(const QString text) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Agree"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    MainWindow::connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    MainWindow::connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    return dlg.exec() == QDialog::Accepted;
}

qint64 GeneralDialog::ChooseDialog(const QString text, const QList<QString> data) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Choose"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    MainWindow::connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    MainWindow::connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));

    QPointer<QButtonGroup> group = new QButtonGroup(&dlg);

    for (qint64 i = 0; i < data.size(); ++i) {
        auto radio = new QRadioButton(data[i], &dlg);
        group->addButton(radio, i);
        layout->addWidget(radio);
    }

    layout->addWidget(btn_box);
    dlg.setLayout(layout);

    dlg.exec();

    return group->checkedId();
}
