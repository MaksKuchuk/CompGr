#include "generaldialog.h"
#include "../mainwindow.h"
#include <QString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>

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
