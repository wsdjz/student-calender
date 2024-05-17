// taskmanagerwidget.cpp
#include "taskmanagerwidget.h"
#include "taskmodifydialog.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QTableView>
#include <QHeaderView>
#include <QSqlTableModel>
#include <QSqlRecord>

TaskManagerWidget::TaskManagerWidget(const QString &studentId, QWidget *parent) : QWidget(parent), studentId(studentId)
{
    setWindowTitle("任务管理");
    resize(800, 600);
    setupUI();
    createTaskTable();
    populateTaskTable();
}

void TaskManagerWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
      layout->setContentsMargins(20, 20, 20, 20); // 设置边距

      // 设置背景色为浅蓝色
      QPalette palette;
      palette.setColor(QPalette::Background, QColor("#B0E2FF"));
      this->setAutoFillBackground(true);
      this->setPalette(palette);

      QLabel *titleLabel = new QLabel("任务管理", this);
      titleLabel->setAlignment(Qt::AlignCenter);
      titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #002080; margin-bottom: 10px;");

      QLabel *contentLabel = new QLabel("内容:", this);
      QLineEdit *contentLineEdit = new QLineEdit(this);
      contentLineEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #CCCCCC; border-radius: 4px; }");

      QHBoxLayout *dateTimeLayout = new QHBoxLayout; // 完成日期和时间的布局
      QLabel *dueDateLabel = new QLabel("完成日期:", this);
      QDateEdit *dueDateEdit = new QDateEdit(this);
      dueDateEdit->setCalendarPopup(true);
      dueDateEdit->setDate(QDate::currentDate());
      dateTimeLayout->addWidget(dueDateLabel);
      dateTimeLayout->addWidget(dueDateEdit);

      QLabel *dueTimeLabel = new QLabel("完成时间:", this);
      QTimeEdit *dueTimeEdit = new QTimeEdit(this);
      dueTimeEdit->setDisplayFormat("HH:mm");
      dueTimeEdit->setTime(QTime::currentTime());
      dateTimeLayout->addWidget(dueTimeLabel);
      dateTimeLayout->addWidget(dueTimeEdit);

      QLabel *priorityLabel = new QLabel("重要程度:", this);
      QComboBox *priorityComboBox = new QComboBox(this);
      priorityComboBox->addItem("低");
      priorityComboBox->addItem("中");
      priorityComboBox->addItem("高");

      QHBoxLayout *buttonLayout = new QHBoxLayout; // 按钮的布局
      QPushButton *createButton = new QPushButton("创建任务", this);
      createButton->setStyleSheet("QPushButton { background-color: #002080; color: white; padding: 8px 16px; border-radius: 4px; }"
                                  "QPushButton:hover { background-color: #0040FF; }");

      buttonLayout->addWidget(createButton);

      QPushButton *deleteButton = new QPushButton("删除任务", this);
      deleteButton->setStyleSheet("QPushButton { background-color: #CC0000; color: white; padding: 8px 16px; border-radius: 4px; }"
                                  "QPushButton:hover { background-color: #FF3333; }");

      buttonLayout->addWidget(deleteButton);

      QPushButton *modifyButton = new QPushButton("修改任务", this); // 新增修改任务按钮
      modifyButton->setStyleSheet("QPushButton { background-color: #002080; color: white; padding: 8px 16px; border-radius: 4px; }"
                                  "QPushButton:hover { background-color: #0040FF; }");

      buttonLayout->addWidget(modifyButton);

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



      layout->addWidget(titleLabel);
      layout->addWidget(contentLabel);
      layout->addWidget(contentLineEdit);
      layout->addLayout(dateTimeLayout); // 添加完成日期和时间的布局
      layout->addWidget(priorityLabel);
      layout->addWidget(priorityComboBox);
      layout->addLayout(buttonLayout); // 添加按钮的布局
      layout->addWidget(taskView);

      setLayout(layout);



       // 修改 createButton 的点击事件处理方法以将当前登录用户的学生ID与任务一起存储在数据库中
       connect(createButton, &QPushButton::clicked, this, [=]() {
           QString content = contentLineEdit->text();
           QDate dueDate = dueDateEdit->date();
           QTime dueTime = dueTimeEdit->time();
           QString priority = priorityComboBox->currentText();

           // 将任务插入到数据库中，包括当前登录用户的学生ID
           QSqlQuery query;
           query.prepare("INSERT INTO tasks (student_id, content, due_datetime, priority) VALUES (:studentId, :content, :dueDatetime, :priority)");
           QDateTime dueDatetime(dueDate, dueTime);
           query.bindValue(":studentId", studentId); // 存储当前登录用户的学生ID
           query.bindValue(":content", content);
           query.bindValue(":dueDatetime", dueDatetime);
           query.bindValue(":priority", priority);

           if (!query.exec()) {
               QMessageBox::critical(this, "错误", "请勿创建相同的的任务");
           } else {
               QMessageBox::information(this, "成功", "任务创建成功！");
               contentLineEdit->clear();
               dueDateEdit->setDate(QDate::currentDate());
               dueTimeEdit->setTime(QTime::currentTime());
               priorityComboBox->setCurrentIndex(0);
               populateTaskTable(); // 刷新任务列表
           }
       });


       connect(modifyButton, &QPushButton::clicked, this, &TaskManagerWidget::modifyTask);

       connect(deleteButton, &QPushButton::clicked, this, [=]() {
           QModelIndexList selectedIndexes = taskView->selectionModel()->selectedRows();
           if (selectedIndexes.isEmpty()) {
               QMessageBox::warning(this, "警告", "请先选择要删除的任务！");
               return;
           }

           int row = selectedIndexes.first().row();
           if (QMessageBox::question(this, "确认删除", "确定要删除选定的任务吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
               taskModel->removeRow(row);
               taskModel->submitAll();
               populateTaskTable();
               QMessageBox::information(this, "成功", "任务删除成功！");
           }
       });
}




void TaskManagerWidget::createTaskTable()
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






void TaskManagerWidget::modifyTask()
{
    QModelIndexList selectedIndexes = taskView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要修改的任务！");
        return;
    }

    int row = selectedIndexes.first().row();
    QSqlRecord originalRecord = taskModel->record(row);

    TaskModifyDialog dialog(originalRecord, this);
    if (dialog.exec() == QDialog::Accepted) {
        QSqlRecord modifiedRecord = dialog.getModifiedRecord();

        // 检查是否存在与修改后的记录相同的任务
        QString content = modifiedRecord.value("content").toString();
        if (taskModel->rowCount() > 0) {
            for (int i = 0; i < taskModel->rowCount(); ++i) {
                if (i != row && taskModel->record(i).value("content").toString() == content) {
                    QMessageBox::critical(this, "错误", "无法修改任务：请勿与其他任务相同" );
                    return;
                }
            }
        }

        // 将修改后的记录应用到数据库中
        if (!taskModel->setRecord(row, modifiedRecord)) {
            QMessageBox::critical(this, "错误", "无法修改任务" );
            return;
        }

        // 提交更改并刷新任务表格
        if (!taskModel->submitAll()) {
            QMessageBox::critical(this, "错误", "无法提交任务更改" );
            return;
        }

        populateTaskTable();
        QMessageBox::information(this, "成功", "任务修改成功！");
    }
}



void TaskManagerWidget::populateTaskTable()
{
     taskModel->setFilter(QString("student_id = '%1'").arg(studentId));
    taskModel->select();
}





