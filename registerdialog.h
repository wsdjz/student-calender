#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

signals:
    void registrationSuccess(const QString &userId, const QString &password);

private slots:
    void registerUser();

private:
    QLineEdit *userIdLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *registerButton;
};

#endif // REGISTERDIALOG_H
