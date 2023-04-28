#include "generaldialog.h"
#include "../mainwindow.h"
#include <QString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

GeneralDialog::GeneralDialog() = default;

void GeneralDialog::InfoDialog(const QString text) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Agree"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    dlg.exec();
}

bool GeneralDialog::AgreeDialog(const QString text) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Agree"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    return dlg.exec() == QDialog::Accepted;
}

qint64 GeneralDialog::ChooseDialog(const QString text, const QList<QString> texts) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Choose"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));

    QPointer<QButtonGroup> group = new QButtonGroup(&dlg);

    for (qint64 i = 0; i < texts.size(); ++i) {
        auto radio = new QRadioButton(texts[i], &dlg);
        group->addButton(radio, i);
        layout->addWidget(radio);
    }

    layout->addWidget(btn_box);
    dlg.setLayout(layout);

    dlg.exec();

    return group->checkedId();
}

qint64 GeneralDialog::ButtonDialog(const QString text, const QList<QString> texts) {
    QDialog dlg(MainWindow::instance);
    dlg.setWindowTitle(tr("Select"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addWidget(new QLabel(text));

    QPointer<QButtonGroup> group = new QButtonGroup(&dlg);
    qint64 out = -1;

    for (qint64 i = 0; i < texts.size(); ++i) {
        auto radio = new QPushButton(texts[i], &dlg);
        group->addButton(radio, i);
        layout->addWidget(radio);
        connect(radio, &QPushButton::clicked, &dlg, &QDialog::accept);
        connect(radio, &QPushButton::clicked, [&out, i]() { out = i; });
    }

    layout->addWidget(btn_box);
    dlg.setLayout(layout);

    if (dlg.exec() == QDialog::Accepted) {
        return out;
    }

    return -1;
}
