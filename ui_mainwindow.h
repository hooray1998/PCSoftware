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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
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
    QAction *actionchange_theme;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *pushButtonUntie;
    QListView *listView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QPushButton *pushButtonTie;
    QListView *listView_2;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QWidget *tab_2;
    QTextEdit *logTextEdit;
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
        MainWindow->resize(1216, 728);
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
        actionchange_theme = new QAction(MainWindow);
        actionchange_theme->setObjectName(QStringLiteral("actionchange_theme"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_4 = new QHBoxLayout(centralWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        pushButtonUntie = new QPushButton(centralWidget);
        pushButtonUntie->setObjectName(QStringLiteral("pushButtonUntie"));

        horizontalLayout_3->addWidget(pushButtonUntie);


        verticalLayout->addLayout(horizontalLayout_3);

        listView = new QListView(centralWidget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setDragEnabled(false);

        verticalLayout->addWidget(listView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        pushButtonTie = new QPushButton(centralWidget);
        pushButtonTie->setObjectName(QStringLiteral("pushButtonTie"));

        horizontalLayout_2->addWidget(pushButtonTie);


        verticalLayout->addLayout(horizontalLayout_2);

        listView_2 = new QListView(centralWidget);
        listView_2->setObjectName(QStringLiteral("listView_2"));

        verticalLayout->addWidget(listView_2);


        horizontalLayout_4->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidget = new QTableWidget(tab);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout->addWidget(tableWidget);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        logTextEdit = new QTextEdit(centralWidget);
        logTextEdit->setObjectName(QStringLiteral("logTextEdit"));
        logTextEdit->setMaximumSize(QSize(99999, 200));

        verticalLayout_2->addWidget(logTextEdit);


        horizontalLayout_4->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1216, 31));
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
        menu_4->addAction(actionchange_theme);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


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
        actionchange_theme->setText(QApplication::translate("MainWindow", "change theme", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\204\345\210\227\350\241\250\357\274\232 ", Q_NULLPTR));
        pushButtonUntie->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\204\350\247\243\347\273\221", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\346\234\252\347\273\221\345\256\232\347\232\204\350\256\276\345\244\207\345\210\227\350\241\250\357\274\232", Q_NULLPTR));
        pushButtonTie->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\204\347\273\221\345\256\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", " VS\350\260\203\350\257\225", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\347\262\276\345\272\246\350\260\203\350\257\225", Q_NULLPTR));
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
