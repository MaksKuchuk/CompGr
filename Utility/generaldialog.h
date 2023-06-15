#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QWidget>
#include <QString>
#include <QList>

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
    static QList<QString> MultiInputDialog(const QString text, const QList<QString> labels, const QList<QString> defTexts);
};

#endif // GENERALDIALOG_H
