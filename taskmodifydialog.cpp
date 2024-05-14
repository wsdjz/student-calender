// taskmodifydialog.cpp

#include "taskmodifydialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDateTimeEdit>

TaskModifyDialog::TaskModifyDialog(const QSqlRecord &record, QWidget *parent)
    : QDialog(parent), originalRecord(record)
{
     resize(400, height());
    modifiedRecord = originalRecord;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20); // 设置边距

    // 设置背景颜色
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#B0E2FF"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QLabel *titleLabel = new QLabel("修改任务", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #002080; margin-bottom: 20px;");
    layout->addWidget(titleLabel);

    QLabel *contentLabel = new QLabel("内容:", this);
    contentLineEdit = new QLineEdit(originalRecord.value("content").toString(), this);
    contentLineEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #CCCCCC; border-radius: 4px; }");
    layout->addWidget(contentLabel);
    layout->addWidget(contentLineEdit);

    // Priority
    QLabel *priorityLabel = new QLabel("重要程度:", this);
    priorityComboBox = new QComboBox(this);
    priorityComboBox->addItem("低");
    priorityComboBox->addItem("中");
    priorityComboBox->addItem("高");
    priorityComboBox->setCurrentText(originalRecord.value("priority").toString());
    layout->addWidget(priorityLabel);
    layout->addWidget(priorityComboBox);

    // Due Date and Time
    QLabel *dueDateTimeLabel = new QLabel("截止时间:", this);
    dueDateTimeEdit = new QDateTimeEdit(this);
    dueDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    dueDateTimeEdit->setDateTime(originalRecord.value("due_datetime").toDateTime()); // 设置为原始任务截止时间
    layout->addWidget(dueDateTimeLabel);
    layout->addWidget(dueDateTimeEdit);

    // Save
    QPushButton *saveButton = new QPushButton("保存", this);
    saveButton->setStyleSheet("QPushButton { background-color: #002080; color: white; padding: 8px 16px; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #0040FF; }");
    connect(saveButton, &QPushButton::clicked, this, &TaskModifyDialog::saveChanges);
    layout->addWidget(saveButton);

    setLayout(layout);
}

void TaskModifyDialog::saveChanges()
{
    modifiedRecord.setValue("content", contentLineEdit->text());
    modifiedRecord.setValue("priority", priorityComboBox->currentText());
    modifiedRecord.setValue("due_datetime", dueDateTimeEdit->dateTime().addSecs(-dueDateTimeEdit->dateTime().time().second())); // 移除秒

    accept();
}

QSqlRecord TaskModifyDialog::getModifiedRecord() const
{
    return modifiedRecord;
}
