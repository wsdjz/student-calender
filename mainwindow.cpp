#include "mainwindow.h"
#include "schedulewidget.h"
#include "taskmanagerwidget.h"
#include "login.h"
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include<QHeaderView>
#include <QtSql>

MainWindow::MainWindow(const QString &studentId, QWidget *parent) : QMainWindow(parent), studentId(studentId) {
    setWindowTitle("时间管理系统");



    // 创建垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 设置背景颜色
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#B0E2FF")); // 浅蓝色背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 添加时间标签
    QLabel *timeLabel = new QLabel(this);
    updateTimeLabel(timeLabel); // 更新时间标签
    QTimer *timer = new QTimer(this); // 创建定时器
       connect(timer, &QTimer::timeout, [this, timeLabel](){ updateTimeLabel(timeLabel); });
      // 定时更新时间
       timer->start(1000); // 每秒更新一次
    timeLabel->setStyleSheet("font-size: 36px; color: #002080; font-weight: bold;"); // 设置时间标签样式
    mainLayout->addWidget(timeLabel);


    // 添加问候语
    QLabel *greetingLabel = new QLabel(this);
    updateGreetingLabel(greetingLabel); // 更新问候语
    greetingLabel->setStyleSheet("font-size: 18px; color: #002080;"); // 设置问候语标签样式
    mainLayout->addWidget(greetingLabel);

    // 添加任务表格
    createTaskTable(studentId);
    taskView->setStyleSheet("background-color: #F0F0F0; color: #002080; gridline-color: #002080; border: 1px solid #002080;"); // 设置表格样式
    mainLayout->addWidget(taskView);

    // 添加按钮部件
    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    QPushButton *scheduleButton = new QPushButton("日程安排", this);
    QPushButton *taskManagerButton = new QPushButton("任务管理", this);
    QPushButton *logoutButton = new QPushButton("退出登录", this);
    scheduleButton->setStyleSheet("background-color: #002080; color: white; padding: 12px 20px; border-radius: 5px; font-size: 16px;");
    taskManagerButton->setStyleSheet("background-color: #002080; color: white; padding: 12px 20px; border-radius: 5px; font-size: 16px;");
    logoutButton->setStyleSheet("background-color: #002080; color: white; padding: 12px 20px; border-radius: 5px; font-size: 16px;");

    // 添加鼠标悬停样式
    scheduleButton->setStyleSheet(scheduleButton->styleSheet() + "QPushButton:hover { background-color: #0040FF; }");
    taskManagerButton->setStyleSheet(taskManagerButton->styleSheet() + "QPushButton:hover { background-color: #0040FF; }");
    logoutButton->setStyleSheet(logoutButton->styleSheet() + "QPushButton:hover { background-color: #0040FF; }");

    buttonLayout->addWidget(scheduleButton);
    buttonLayout->addWidget(taskManagerButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(logoutButton);
    mainLayout->addWidget(buttonWidget);

    // 设置主窗口布局
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接按钮点击事件
    connect(scheduleButton, &QPushButton::clicked, this, &MainWindow::openSchedule);
    connect(taskManagerButton, &QPushButton::clicked, this, &MainWindow::openTaskManager);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logout);
}


void MainWindow::createTaskTable(const QString &studentId) {
    taskModel = new QSqlTableModel(this);
    taskModel->setTable("tasks");
    taskModel->setFilter(QString("student_id = '%1'").arg(studentId)); // 设置过滤条件只显示特定用户的任务
    taskModel->select();


    taskModel->setHeaderData(2, Qt::Horizontal, "任务");

    taskView = new QTableView(this);
    taskView->setModel(taskModel);
    taskView->hideColumn(3);
     taskView->hideColumn(0);
        taskView->hideColumn(1);
        taskView->setStyleSheet("QTableView { background-color: #F0F0F0; color: #002080; gridline-color: #002080; border: 1px solid #002080; }"
                                   "QTableView::item { padding: 10px; }"
                                  "QHeaderView::section { background-color: #0040FF; color: white; padding: 8px; border: none; font-size: 16px; }"
                                    "QTableView::item:selected { background-color: #cceeff; }");
        taskView->horizontalHeader()->setStretchLastSection(true);
        taskView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}


//这个函数比较畸形，完成了很多功能
void MainWindow::updateTimeLabel(QLabel *label) {
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("hh:mm:ss"); // 格式化时间字符串
    label->setText(timeString); // 更新时间标签

    // 检查数据库中是否有任务截止时间与当前时间相差1秒以内的任务
    // 获取当前时间
    QDateTime dbDateTime = currentTime.addSecs(1); // 当前时间加一秒

    // 在这里执行查询，检查是否有任务的截止时间与当前时间相差1秒以内
    QSqlQuery query;
    query.prepare("SELECT * FROM tasks WHERE due_datetime BETWEEN :currentTime AND :nextSecond AND student_id = :studentId");
    query.bindValue(":currentTime", currentTime);
    query.bindValue(":nextSecond", dbDateTime);
    query.bindValue(":studentId", studentId);

    if (query.exec() && query.next()) {
        // 如果查询成功且有结果，则弹出提醒窗口并显示任务内容
                QString taskContent = query.value("content").toString();
                QMessageBox::information(this, "提醒", "有任务截止了！\n任务内容：" + taskContent);
    }

}




void MainWindow::updateGreetingLabel(QLabel *label) {
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    int hour = currentTime.time().hour();
    QString greeting;
    if (hour < 12) {
        greeting = "早上好！";
    } else if (hour < 18) {
        greeting = "下午好！";
    } else {
        greeting = "晚上好！";
    }
    label->setText(greeting + " " + studentId); // 更新问候语
}

void MainWindow::openSchedule() {
    ScheduleWidget *scheduleWidget = new ScheduleWidget(studentId);
    scheduleWidget->show();
    taskModel->setFilter(QString("student_id = '%1'").arg(studentId));
   taskModel->select();
}

void MainWindow::openTaskManager() {
    TaskManagerWidget *taskManagerWidget = new TaskManagerWidget(studentId);
    taskManagerWidget->show();
    taskModel->setFilter(QString("student_id = '%1'").arg(studentId));
   taskModel->select();

}

void MainWindow::logout() {
    close(); // 关闭当前窗口
}
