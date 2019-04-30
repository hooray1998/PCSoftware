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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
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
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QLineEdit *lineEditVS1_mode1;
    QLabel *label_4;
    QLineEdit *lineEditVS2_mode1;
    QLabel *label_5;
    QLineEdit *lineEditJiNumber_mode1;
    QLabel *label_6;
    QLineEdit *lineEditCeName_mode1;
    QTableWidget *tableWidgetvs1;
    QTableWidget *tableWidgetvs2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_11;
    QLineEdit *lineEditVS1_mode2;
    QLabel *label_7;
    QLineEdit *lineEditVS2_mode2;
    QLabel *label_8;
    QLineEdit *lineEditYinNumber_mode2;
    QLabel *label_9;
    QLineEdit *lineEditJiNumber_mode2;
    QLabel *label_10;
    QLineEdit *lineEditCeName_mode2;
    QTableWidget *tableWidget_2;
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
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_5->addWidget(pushButton);

        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_5->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        lineEditVS1_mode1 = new QLineEdit(tab);
        lineEditVS1_mode1->setObjectName(QStringLiteral("lineEditVS1_mode1"));

        horizontalLayout_5->addWidget(lineEditVS1_mode1);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        lineEditVS2_mode1 = new QLineEdit(tab);
        lineEditVS2_mode1->setObjectName(QStringLiteral("lineEditVS2_mode1"));

        horizontalLayout_5->addWidget(lineEditVS2_mode1);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        lineEditJiNumber_mode1 = new QLineEdit(tab);
        lineEditJiNumber_mode1->setObjectName(QStringLiteral("lineEditJiNumber_mode1"));

        horizontalLayout_5->addWidget(lineEditJiNumber_mode1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_5->addWidget(label_6);

        lineEditCeName_mode1 = new QLineEdit(tab);
        lineEditCeName_mode1->setObjectName(QStringLiteral("lineEditCeName_mode1"));

        horizontalLayout_5->addWidget(lineEditCeName_mode1);


        verticalLayout_4->addLayout(horizontalLayout_5);

        tableWidgetvs1 = new QTableWidget(tab);
        tableWidgetvs1->setObjectName(QStringLiteral("tableWidgetvs1"));

        verticalLayout_4->addWidget(tableWidgetvs1);

        tableWidgetvs2 = new QTableWidget(tab);
        tableWidgetvs2->setObjectName(QStringLiteral("tableWidgetvs2"));

        verticalLayout_4->addWidget(tableWidgetvs2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout->addWidget(label_11);

        lineEditVS1_mode2 = new QLineEdit(tab_2);
        lineEditVS1_mode2->setObjectName(QStringLiteral("lineEditVS1_mode2"));

        horizontalLayout->addWidget(lineEditVS1_mode2);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout->addWidget(label_7);

        lineEditVS2_mode2 = new QLineEdit(tab_2);
        lineEditVS2_mode2->setObjectName(QStringLiteral("lineEditVS2_mode2"));

        horizontalLayout->addWidget(lineEditVS2_mode2);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout->addWidget(label_8);

        lineEditYinNumber_mode2 = new QLineEdit(tab_2);
        lineEditYinNumber_mode2->setObjectName(QStringLiteral("lineEditYinNumber_mode2"));

        horizontalLayout->addWidget(lineEditYinNumber_mode2);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout->addWidget(label_9);

        lineEditJiNumber_mode2 = new QLineEdit(tab_2);
        lineEditJiNumber_mode2->setObjectName(QStringLiteral("lineEditJiNumber_mode2"));

        horizontalLayout->addWidget(lineEditJiNumber_mode2);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout->addWidget(label_10);

        lineEditCeName_mode2 = new QLineEdit(tab_2);
        lineEditCeName_mode2->setObjectName(QStringLiteral("lineEditCeName_mode2"));

        horizontalLayout->addWidget(lineEditCeName_mode2);


        verticalLayout_3->addLayout(horizontalLayout);

        tableWidget_2 = new QTableWidget(tab_2);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));

        verticalLayout_3->addWidget(tableWidget_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        logTextEdit = new QTextEdit(centralWidget);
        logTextEdit->setObjectName(QStringLiteral("logTextEdit"));
        logTextEdit->setMaximumSize(QSize(99999, 200));
        logTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(logTextEdit);


        horizontalLayout_4->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1216, 25));
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
        pushButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213VS1\350\260\203\350\257\225", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213VS2\350\260\203\350\257\225", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\344\270\255\346\255\242\345\275\223\345\211\215\350\260\203\350\257\225", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "VS1\345\216\237\345\200\274\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "  VS2\345\216\237\345\200\274\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "  \346\234\272\345\231\250\345\217\267\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "  \346\265\213\350\257\225\344\272\272\345\221\230\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", " VS\350\260\203\350\257\225", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "VS1\345\216\237\345\200\274\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "  VS2\345\216\237\345\200\274\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "  \345\274\225\346\265\201\347\263\273\346\225\260\345\216\237\345\200\274\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "  \346\234\272\345\231\250\345\217\267\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "  \346\265\213\350\257\225\344\272\272\345\221\230\357\274\232", Q_NULLPTR));
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
