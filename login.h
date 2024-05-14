#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>

class QLineEdit;
class QPushButton;

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
private:
  QPushButton *loginButton;
  QPushButton *registerButton;
 void setupButtonConnections();
    void setupUI();
   void applyStylesheet();
    QLineEdit *userIdLineEdit;
    QLineEdit *passwordLineEdit;
    QSqlDatabase db;

    bool createConnection();
    Q_SIGNALS:
    void loginSuccess(const QString &studentId);
};

#endif // LOGIN_H
