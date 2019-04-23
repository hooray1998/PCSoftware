/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionVs;
    QAction *actionJingdu;
    QAction *actionGroupBound;
    QAction *actionGroupUnbound;
    QAction *actionSetIP;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1203, 729);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        actionVs = new QAction(MainWindow);
        actionVs->setObjectName(QStringLiteral("actionVs"));
        actionJingdu = new QAction(MainWindow);
        actionJingdu->setObjectName(QStringLiteral("actionJingdu"));
        actionGroupBound = new QAction(MainWindow);
        actionGroupBound->setObjectName(QStringLiteral("actionGroupBound"));
        actionGroupUnbound = new QAction(MainWindow);
        actionGroupUnbound->setObjectName(QStringLiteral("actionGroupUnbound"));
        actionSetIP = new QAction(MainWindow);
        actionSetIP->setObjectName(QStringLiteral("actionSetIP"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1203, 31));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menu->addAction(actionVs);
        menu->addAction(actionJingdu);
        menu_2->addAction(actionGroupBound);
        menu_2->addAction(actionGroupUnbound);
        menu_4->addAction(actionSetIP);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
        actionSave_as->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", Q_NULLPTR));
        actionVs->setText(QApplication::translate("MainWindow", "VS\350\260\203\350\257\225", Q_NULLPTR));
        actionJingdu->setText(QApplication::translate("MainWindow", "\347\262\276\345\272\246\350\260\203\350\257\225", Q_NULLPTR));
        actionGroupBound->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\204\347\273\221\345\256\232", Q_NULLPTR));
        actionGroupUnbound->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\204\350\247\243\347\273\221", Q_NULLPTR));
        actionSetIP->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256IP", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\350\260\203\350\257\225", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\345\205\254\345\274\217", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
