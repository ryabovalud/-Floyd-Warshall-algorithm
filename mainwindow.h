#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <QPainter>

namespace Ui {
class MainWindow;
class Widget;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString readFile();
    explicit MainWindow(QWidget *parent = 0);
    void func_way(int from,int to);
    ~MainWindow();

private slots:
    void on_info_clicked();

    void on_pushButton_PTY_clicked();

    void on_clear_clicked();

    void on_exit_clicked();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);

};


#endif // MAINWINDOW_H
