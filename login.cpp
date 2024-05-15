#include "login.h"
#include "mainwindow.h"
#include "registerdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>
#include <QDir>
#include <QApplication>
#include <QDebug>



LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    createConnection();

    setWindowTitle("登录");

}

void LoginWidget::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 设置标题
     QLabel *titleLabel = new QLabel("大学生时间管理系统", this);
     titleLabel->setObjectName("le_title"); // 设置对象名称以应用样式表
     titleLabel->setAlignment(Qt::AlignCenter);
     titleLabel->setStyleSheet("font-size: 36px; color: white;"); // 设置标题样式
     layout->addWidget(titleLabel);

     // 设置背景颜色
        this->setObjectName("bg"); // 设置对象名称以应用样式表

     QLabel *userIdLabel = new QLabel("用户名:", this);
         userIdLineEdit = new QLineEdit(this);

         QLabel *passwordLabel = new QLabel("密码:", this);
         passwordLineEdit = new QLineEdit(this);
         passwordLineEdit->setEchoMode(QLineEdit::Password);



         layout->addWidget(userIdLabel);
            layout->addWidget(userIdLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);


    // 添加登录和注册按钮
      QPushButton *loginButton = new QPushButton("登录", this);
      QPushButton *registerButton = new QPushButton("注册", this);
      loginButton->setObjectName("loginBtn"); // 设置对象名称以应用样式表
      registerButton->setObjectName("registerBtn"); // 设置对象名称以应用样式表
      connect(loginButton, SIGNAL(clicked()), this, SLOT(on_loginButton_clicked()));
      connect(registerButton, SIGNAL(clicked()), this, SLOT(on_registerButton_clicked()));

      // 将注册按钮放在左边，登录按钮放在右边
      QHBoxLayout *buttonLayout = new QHBoxLayout;
      buttonLayout->addWidget(registerButton);
      buttonLayout->addStretch(); // 添加伸缩项，使登录按钮右对齐
      buttonLayout->addWidget(loginButton);
      layout->addLayout(buttonLayout);





    setLayout(layout);



    setStyleSheet(
            "QWidget#bg {"
                "background-color: #6CA6CD;" // 浅蓝色背景
                "padding: 20px;" // 设置内边距
            "}"
            "QPushButton#loginBtn, QPushButton#registerBtn {"
                "background-color: #409eff;"
                "color: white;"
                "border-radius: 4px;"
                "font-size: 18px;"
                "padding: 8px 16px;"
            "}"
            "QPushButton#loginBtn:hover, QPushButton#registerBtn:hover {"
                "background-color: #64b5f6;"
            "}"
            "QLineEdit {"
                "border: 2px solid #ccc;" // 设置输入框边框
                "border-radius: 4px;" // 设置输入框边框圆角
            "}"
        );
}














void LoginWidget::on_loginButton_clicked() {
    QString userId = userIdLineEdit->text();
    QString password = passwordLineEdit->text();

    QSqlQuery query;//执行sql查询


    query.exec("select student_id from students where student_id = '" + userId + "' and password = '" + password + "'");


    if (!query.next()) {
        QMessageBox::about(this, tr("错误！"), "用户名或密码错误");
        return;
    }

    QString studentId = query.value(0).toString(); // 获取学生 ID

    QMessageBox::about(this, tr("Success"), tr("登陆成功"));
    // 登录成功，创建并显示主界面
    MainWindow *mainwindow = new MainWindow(studentId); // 传递学生 ID
    mainwindow->show();

    // 关闭登录界面
    close();
}




//void LoginWidget::on_registerButton_clicked() {
//    QString userId = userIdLineEdit->text();
//    QString password = passwordLineEdit->text();

//    QSqlQuery query;

//    // 准备 SQL 插入语句
//    QString insertStatement = "INSERT INTO students (student_id, password) VALUES (:id, :hash)";


//    // 使用 prepare() 方法准备 SQL 语句，并为参数设置占位符
//    query.prepare(insertStatement);

//    // 绑定参数值
//    query.bindValue(":id", userId);
//    query.bindValue(":hash", password);


//    if (!query.exec()) {
//        // 如果执行失败，输出错误信息，后续再加错误原因
//        QMessageBox::about(this, tr("错误！"), "用户名或者密码不符合规范");
//    } else {
//        // 如果执行成功，可以提示用户注册成功
//        QMessageBox::about(this, tr("注册成功"), tr("注册成功"));
//    }

//}

void LoginWidget::on_registerButton_clicked() {
    RegisterDialog registerDialog;
        registerDialog.exec();
}






bool LoginWidget::createConnection() {
    //QCoreApplication a(argc, argv);

       // 获取应用程序的目录路径
      QString currentDir = QDir::currentPath();
     qDebug() <<currentDir;
       // 指定数据库文件的相对路径
      QString dbFilePath = currentDir + "/timemaster.db";


    // 创建数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

if (!db.open()) {
     QMessageBox::about(this, tr("错误！"), "数据库连接失败");
     return false;
}


        return true;
}



















