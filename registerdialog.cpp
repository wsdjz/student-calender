// registerdialog.cpp

#include "registerdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlError>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("注册");
    setFixedSize(380, 300); // 设置窗口大小
    setStyleSheet("background-color: #6CA6CD;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("用户注册", this);
    titleLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: white; align: center;");

    QLabel *userIdLabel = new QLabel("用户名:", this);
    userIdLabel->setStyleSheet("color: black;");
    userIdLineEdit = new QLineEdit(this);
    userIdLineEdit->setStyleSheet("background-color: #FFFFFF; color: black;");

    QLabel *passwordLabel = new QLabel("密码:", this);
    passwordLabel->setStyleSheet("color: black;");
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("background-color: #FFFFFF; color: black;");

    QLabel *confirmPasswordLabel = new QLabel("确认密码:", this);
    confirmPasswordLabel->setStyleSheet("color: black;");
    confirmPasswordLineEdit = new QLineEdit(this);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setStyleSheet("background-color: #FFFFFF; color: black;");

    registerButton = new QPushButton("确认", this);
    registerButton->setStyleSheet("background-color: #409eff; color: white;");

    layout->addWidget(titleLabel);
    layout->addWidget(userIdLabel);
    layout->addWidget(userIdLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordLineEdit);
    layout->addWidget(registerButton);
    layout->setAlignment(titleLabel, Qt::AlignCenter); // 标题在布局中居中

    connect(registerButton, &QPushButton::clicked, this, &RegisterDialog::registerUser);

    setLayout(layout);
}



void RegisterDialog::registerUser() {
    QString userId = userIdLineEdit->text();
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    if (userId.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::about(this, tr("错误！"), "用户名和密码不能为空");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::about(this, tr("错误！"), "密码与确认密码不匹配");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE student_id = :id");
    query.bindValue(":id", userId);
    if (query.exec() && query.next()) {
        QMessageBox::about(this, tr("错误！"), "用户名已存在");
        return;
    }

    query.prepare("INSERT INTO students (student_id, password) VALUES (:id, :hash)");
    query.bindValue(":id", userId);
    query.bindValue(":hash", password);
    if (!query.exec()) {
        QMessageBox::about(this, tr("错误！"), "注册失败，用户名或密码不符合规范");
        return;
    }

    QMessageBox::about(this, tr("注册成功"), tr("注册成功"));
    emit registrationSuccess(userId, password);
    close();
}
