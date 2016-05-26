/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_About;
    QAction *action_Exit;
    QAction *action_Connect;
    QAction *actionSave_Config;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QCheckBox *Output1;
    QCheckBox *Output2;
    QCheckBox *Output3;
    QCheckBox *Output4;
    QRadioButton *staticIP;
    QRadioButton *dhcp;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *dhcp_name;
    QLabel *dhcp_name_label;
    QLineEdit *mac;
    QLabel *label_5;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 369);
        action_About = new QAction(MainWindow);
        action_About->setObjectName(QStringLiteral("action_About"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        action_Connect = new QAction(MainWindow);
        action_Connect->setObjectName(QStringLiteral("action_Connect"));
        actionSave_Config = new QAction(MainWindow);
        actionSave_Config->setObjectName(QStringLiteral("actionSave_Config"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 230, 371, 91));
        Output1 = new QCheckBox(groupBox);
        Output1->setObjectName(QStringLiteral("Output1"));
        Output1->setGeometry(QRect(10, 30, 81, 21));
        Output2 = new QCheckBox(groupBox);
        Output2->setObjectName(QStringLiteral("Output2"));
        Output2->setGeometry(QRect(10, 60, 81, 21));
        Output3 = new QCheckBox(groupBox);
        Output3->setObjectName(QStringLiteral("Output3"));
        Output3->setGeometry(QRect(120, 30, 81, 21));
        Output4 = new QCheckBox(groupBox);
        Output4->setObjectName(QStringLiteral("Output4"));
        Output4->setGeometry(QRect(120, 60, 81, 21));
        staticIP = new QRadioButton(centralWidget);
        staticIP->setObjectName(QStringLiteral("staticIP"));
        staticIP->setGeometry(QRect(10, 40, 97, 21));
        dhcp = new QRadioButton(centralWidget);
        dhcp->setObjectName(QStringLiteral("dhcp"));
        dhcp->setGeometry(QRect(10, 160, 97, 21));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(90, 70, 131, 23));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(90, 100, 131, 23));
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(90, 130, 131, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 70, 71, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 100, 56, 21));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 130, 71, 21));
        dhcp_name = new QLineEdit(centralWidget);
        dhcp_name->setObjectName(QStringLiteral("dhcp_name"));
        dhcp_name->setGeometry(QRect(90, 190, 131, 23));
        dhcp_name_label = new QLabel(centralWidget);
        dhcp_name_label->setObjectName(QStringLiteral("dhcp_name_label"));
        dhcp_name_label->setGeometry(QRect(10, 190, 71, 21));
        mac = new QLineEdit(centralWidget);
        mac->setObjectName(QStringLiteral("mac"));
        mac->setGeometry(QRect(90, 10, 131, 23));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 71, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        menuBar->setAcceptDrops(false);
        menuBar->setDefaultUp(false);
        menuBar->setNativeMenuBar(false);
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addSeparator();
        menu_File->addSeparator();
        menu_File->addAction(action_Exit);
        menu_File->addAction(action_Connect);
        menu_File->addAction(actionSave_Config);
        menu_Help->addAction(action_About);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action_About->setText(QApplication::translate("MainWindow", "&About", 0));
        action_Exit->setText(QApplication::translate("MainWindow", "&Exit", 0));
        action_Connect->setText(QApplication::translate("MainWindow", "&Connect", 0));
        actionSave_Config->setText(QApplication::translate("MainWindow", "Save Config", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Control", 0));
        Output1->setText(QApplication::translate("MainWindow", "Output1", 0));
        Output2->setText(QApplication::translate("MainWindow", "Output2", 0));
        Output3->setText(QApplication::translate("MainWindow", "Output3", 0));
        Output4->setText(QApplication::translate("MainWindow", "Output4", 0));
        staticIP->setText(QApplication::translate("MainWindow", "Use static IP", 0));
        dhcp->setText(QApplication::translate("MainWindow", "Use dhcp", 0));
        label->setText(QApplication::translate("MainWindow", "IP address", 0));
        label_2->setText(QApplication::translate("MainWindow", "Netmask", 0));
        label_3->setText(QApplication::translate("MainWindow", "Gateway", 0));
        dhcp_name_label->setText(QApplication::translate("MainWindow", "DHCP name", 0));
        label_5->setText(QApplication::translate("MainWindow", "MAC", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menu_Help->setTitle(QApplication::translate("MainWindow", "&Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
