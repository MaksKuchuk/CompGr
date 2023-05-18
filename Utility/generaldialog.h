#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QWidget>
#include <QString>

class GeneralDialog : public QWidget
{
    Q_OBJECT
public:
    GeneralDialog();

    static void InfoDialog(const QString text);
    static bool AgreeDialog(const QString text);
    static QString InputDialog(const QString text, const QString defaultText = "");
    static qint64 ChooseDialog(const QString text, const QList<QString> texts);
    static qint64 ButtonDialog(const QString text, const QList<QString> texts);
};

#endif // GENERALDIALOG_H
