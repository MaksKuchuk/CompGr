#ifndef FOURIER_DIALOG_H
#define FOURIER_DIALOG_H

#include <QDialog>
#include "ui_fourier.h"

class fourierDialog : public QDialog, public Ui::fourier {
    Q_OBJECT   

public:
    fourierDialog(QWidget* parent = 0, long long samples = 100);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // FOURIER_DIALOG_H
