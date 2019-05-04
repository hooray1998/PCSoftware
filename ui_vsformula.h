/********************************************************************************
** Form generated from reading UI file 'vsformula.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSFORMULA_H
#define UI_VSFORMULA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VSFormula
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_rule;
    QPushButton *pushButtonDel;
    QComboBox *comboBox;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QPushButton *pushButtonAdd;
    QLabel *labelMsg;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *VSFormula)
    {
        if (VSFormula->objectName().isEmpty())
            VSFormula->setObjectName(QStringLiteral("VSFormula"));
        VSFormula->resize(536, 372);
        verticalLayout = new QVBoxLayout(VSFormula);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_rule = new QLabel(VSFormula);
        label_rule->setObjectName(QStringLiteral("label_rule"));

        gridLayout->addWidget(label_rule, 0, 0, 1, 3);

        pushButtonDel = new QPushButton(VSFormula);
        pushButtonDel->setObjectName(QStringLiteral("pushButtonDel"));

        gridLayout->addWidget(pushButtonDel, 1, 2, 1, 1);

        comboBox = new QComboBox(VSFormula);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        label_2 = new QLabel(VSFormula);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(VSFormula);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit = new QLineEdit(VSFormula);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 1, 1, 1);

        pushButtonAdd = new QPushButton(VSFormula);
        pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));

        gridLayout->addWidget(pushButtonAdd, 2, 2, 1, 1);

        labelMsg = new QLabel(VSFormula);
        labelMsg->setObjectName(QStringLiteral("labelMsg"));

        gridLayout->addWidget(labelMsg, 3, 0, 1, 3);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(VSFormula);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(VSFormula);
        QObject::connect(buttonBox, SIGNAL(accepted()), VSFormula, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VSFormula, SLOT(reject()));

        QMetaObject::connectSlotsByName(VSFormula);
    } // setupUi

    void retranslateUi(QDialog *VSFormula)
    {
        VSFormula->setWindowTitle(QApplication::translate("VSFormula", "Dialog", Q_NULLPTR));
        label_rule->setText(QApplication::translate("VSFormula", "TextLabel", Q_NULLPTR));
        pushButtonDel->setText(QApplication::translate("VSFormula", "\345\210\240\351\231\244", Q_NULLPTR));
        label_2->setText(QApplication::translate("VSFormula", "\345\275\223\345\211\215\351\200\211\347\224\250\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("VSFormula", "\346\226\260\345\242\236\345\205\254\345\274\217\357\274\232", Q_NULLPTR));
        pushButtonAdd->setText(QApplication::translate("VSFormula", "\345\242\236\345\212\240", Q_NULLPTR));
        labelMsg->setText(QApplication::translate("VSFormula", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class VSFormula: public Ui_VSFormula {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSFORMULA_H
