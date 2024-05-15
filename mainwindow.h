#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlTableModel>
#include  <QTableView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
   // explicit MainWindow(QWidget *parent = nullptr);    //之前版本
    explicit MainWindow(const QString &studentId, QWidget *parent = nullptr);

private slots:
    void openSchedule();
    void openTaskManager();
    void logout();
    void reSetTable();
private:
    QVBoxLayout *layout;
    QString studentId;
    QSqlTableModel *taskModel;
     QTableView *taskView;
       void updateTimeLabel(QLabel *label); // 更新时间标签
       void updateGreetingLabel(QLabel *label); // 更新问候语
       void createTaskTable(const QString &studentId);
};

#endif // MAINWINDOW_H
