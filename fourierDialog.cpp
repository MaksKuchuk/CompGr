#include "fourierDialog.h"

fourierDialog::fourierDialog(QWidget* parent, long long samples)
    : QDialog(parent)
{
    setupUi(this);
    smooth_spin->setRange(0, samples);
}

void fourierDialog::on_buttonBox_clicked(QAbstractButton *button)
{

}

