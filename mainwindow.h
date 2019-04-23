#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "group.h"
#include "mythread.h"
#include "expression.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getGroupList();//show
    void addGroup();//show a small widget
    void removeGroup();//show a small widget

private:
    Ui::MainWindow *ui;

    Group *curGroup;
    MyThread *allMachine[200];
    int machineSize;
    Group *allGroup[200];
    int groupsize;

    Expression allExpression;


};

#endif // MAINWINDOW_H
