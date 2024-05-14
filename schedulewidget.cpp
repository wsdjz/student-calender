#include "schedulewidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>
#include <QHeaderView>
#include <QSqlTableModel>
#include <QTextCharFormat>

ScheduleWidget::ScheduleWidget(const QString &studentId, QWidget *parent)
    : QWidget(parent), studentId(studentId) // 初始化学生ID
{
    setupUI();
    createTaskTable();
    setWindowTitle("日程安排");
    resize(800, 600);
    populateTaskTable(QDate::currentDate());
}

void ScheduleWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 设置背景颜色
    this->setStyleSheet("background-color: #B0E2FF;");

    // 日历控件
    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setStyleSheet("QCalendarWidget { background-color: #B0E2FF; color: #FFFFFF; border: 1px solid #4682B4; font-size: 16px; font-weight: bold; }"
                                       "QCalendarWidget QAbstractItemView { background-color: #B0E2FF; selection-background-color: #87CEFA; }"); // 设置日历控件样式
    connect(calendarWidget, &QCalendarWidget::clicked, this, &ScheduleWidget::onDateClicked);

    // 设置星期几和周数的背景颜色
        QTextCharFormat dayFormat;
        dayFormat.setBackground(QColor("#4682B4")); // 设置背景颜色
        dayFormat.setForeground(Qt::white); // 设置字体颜色
        QTextCharFormat weekNumberFormat;
        weekNumberFormat.setBackground(QColor("#4682B4")); // 设置背景颜色
        weekNumberFormat.setForeground(Qt::white); // 设置字体颜色
        calendarWidget->setWeekdayTextFormat(Qt::Monday, dayFormat); // 星期一
        calendarWidget->setWeekdayTextFormat(Qt::Tuesday, dayFormat); // 星期二
        calendarWidget->setWeekdayTextFormat(Qt::Wednesday, dayFormat); // 星期三
        calendarWidget->setWeekdayTextFormat(Qt::Thursday, dayFormat); // 星期四
        calendarWidget->setWeekdayTextFormat(Qt::Friday, dayFormat); // 星期五
        calendarWidget->setWeekdayTextFormat(Qt::Saturday, dayFormat); // 星期六
        calendarWidget->setWeekdayTextFormat(Qt::Sunday, dayFormat); // 星期日
        calendarWidget->setHeaderTextFormat(weekNumberFormat); // 设置周数格式

        // 任务表格
           taskView = new QTableView(this);
           taskView->setSelectionMode(QAbstractItemView::SingleSelection);
           taskView->setSelectionBehavior(QAbstractItemView::SelectRows);
           taskView->horizontalHeader()->setStretchLastSection(true);
           taskView->setStyleSheet("QTableView { background-color: #FFFFFF; color: #000000; border: 1px solid #87CEFA; }"
                                   "QHeaderView::section { background-color: #4682B4; color: #FFFFFF; border: none; font-size: 16px; }" // 设置表头样式
                                   "QTableView QAbstractItemView { font-size: 14px; font-weight: bold; }"); // 设置表格内字体样式
           // 设置表格网格线
           taskView->setShowGrid(true);
           taskView->setGridStyle(Qt::DotLine);

           layout->addWidget(calendarWidget);
           layout->addWidget(taskView);

           setLayout(layout);
}

void ScheduleWidget::createTaskTable()
{
    taskModel = new QSqlTableModel(this);
    taskModel->setTable("tasks");
    taskModel->select();

    // 设置表头标题
    taskModel->setHeaderData(1, Qt::Horizontal, "完成时间");
    taskModel->setHeaderData(0, Qt::Horizontal, "重要程度");
    taskModel->setHeaderData(2, Qt::Horizontal, "内容");

    // 隐藏用户ID列
    taskView->setModel(taskModel);
    taskView->hideColumn(3);

    // 设置表格不可编辑
    taskView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列宽
    taskView->setColumnWidth(0, 100); // 设置重要程度列宽度为100像素
    taskView->setColumnWidth(1, 185); // 设置时间列宽度为200像素


}



void ScheduleWidget::populateTaskTable(const QDate &date)
{
    QString dateString = date.toString(Qt::ISODate);
    taskModel->setFilter(QString("due_datetime LIKE '%%1%' AND student_id = '%2'")
                        .arg(dateString)
                        .arg(studentId));
    taskModel->select();
}

void ScheduleWidget::onDateClicked(const QDate &date)
{
    populateTaskTable(date);
}
