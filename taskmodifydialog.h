// taskmodifydialog.h

#ifndef TASKMODIFYDIALOG_H
#define TASKMODIFYDIALOG_H

#include <QDialog>
#include <QSqlRecord>

class QLineEdit;
class QComboBox;
class QDateTimeEdit;

class TaskModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskModifyDialog(const QSqlRecord &record, QWidget *parent = nullptr);
    QSqlRecord getModifiedRecord() const;

private slots:
    void saveChanges();

private:
    QSqlRecord originalRecord;
    QSqlRecord modifiedRecord;
    QLineEdit *contentLineEdit;
    QComboBox *priorityComboBox;
    QDateTimeEdit *dueDateTimeEdit;
};

#endif // TASKMODIFYDIALOG_H
