#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QWidget>
#include <QString>

class GeneralDialog : public QWidget
{
    Q_OBJECT
public:
    GeneralDialog();

    static bool AgreeDialog(const QString text);
    static qint64 ChooseDialog(const QString text, const QList<QString>);
};

#endif // GENERALDIALOG_H
