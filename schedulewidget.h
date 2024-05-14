// schedulewidget.h
#ifndef SCHEDULEWIDGET_H
#define SCHEDULEWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>

class ScheduleWidget : public QWidget
{
    Q_OBJECT
public:
     explicit ScheduleWidget(const QString &studentId, QWidget *parent = nullptr);

private:
    QCalendarWidget *calendarWidget;
    QTableView *taskView;
    QSqlTableModel *taskModel;
    QString studentId;
    void setupUI();
    void createTaskTable();
    void populateTaskTable(const QDate &date);

private slots:
    void onDateClicked(const QDate &date);
};

#endif // SCHEDULEWIDGET_H
