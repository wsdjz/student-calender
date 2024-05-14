// taskmanagerwidget.h
#ifndef TASKMANAGERWIDGET_H
#define TASKMANAGERWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QLineEdit>


class QTableView;

class TaskManagerWidget : public QWidget
{
    Q_OBJECT
public:
   explicit TaskManagerWidget(const QString &studentId, QWidget *parent = nullptr);



private:

     QString studentId;
    void setupUI();
    void createTaskTable();
    void populateTaskTable();
    void modifyTask();

 QLineEdit *contentLineEdit;
    QSqlTableModel *taskModel;
    QTableView *taskView;
};

#endif // TASKMANAGERWIDGET_H
