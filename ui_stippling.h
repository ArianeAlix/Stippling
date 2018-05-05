/********************************************************************************
** Form generated from reading UI file 'stippling.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STIPPLING_H
#define UI_STIPPLING_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "glviewer.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionClear;
    QAction *actionRandom;
	QAction *actionLinear;
	QAction *actionOpti;
	QAction *actionLoad;
	QAction *actionLloyd;
    QWidget *centralwidget;
	QWidget *lld;
	QWidget *wdg;
    QGridLayout *gridLayout;
	QGridLayout *gridLayoutPic;
	QGridLayout *gridLayoutLld;
	GlViewer *viewer;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuPoints;
	QMenu *menuPicture;
	QMenu *menuLloyd;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(0, 0);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        QIcon icon;
        icon.addFile(QStringLiteral("icons/fileNew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon);
        actionRandom = new QAction(MainWindow);
        actionRandom->setObjectName(QStringLiteral("actionRandom"));
		actionLinear = new QAction(MainWindow);
		actionLinear->setObjectName(QStringLiteral("actionLinear"));
		actionOpti = new QAction(MainWindow);
		actionOpti->setObjectName(QStringLiteral("actionOpti"));


		//New action : load picture
		actionLoad = new QAction(MainWindow);
		actionLoad->setObjectName(QStringLiteral("actionLoad"));

		//New action : LLoyd
		actionLloyd = new QAction(MainWindow);
		actionLloyd->setObjectName(QStringLiteral("actionLloyd"));


        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

		wdg = new QWidget();
		wdg->setObjectName(QStringLiteral("wdg"));
		gridLayoutPic = new QGridLayout(wdg);
		//Pour LLoyd
		lld = new QWidget();
		lld->setObjectName(QStringLiteral("lld"));
		gridLayoutLld = new QGridLayout(lld);

		// init scene
		viewer = new GlViewer(lld);
		viewer->setObjectName(QStringLiteral("viewer"));
		viewer->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));


        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 578, 21));


		//menuPicture
		menuPicture = new QMenu(menubar);
		menuPicture->setObjectName(QStringLiteral("menuPicture"));
		//menuData
		menuPoints = new QMenu(menubar);
		menuPoints->setObjectName(QStringLiteral("menuPoints"));
		//menuLloyd
		menuLloyd = new QMenu(menubar);
		menuLloyd->setObjectName(QStringLiteral("menuLloyd"));


		MainWindow->setMenuBar(menubar);

		menubar->addAction(menuPoints->menuAction());
		menubar->addAction(menuPicture->menuAction());
		menubar->addAction(menuLloyd->menuAction());

		menuPicture->addAction(actionLoad);

        menuPoints->addAction(actionRandom);
		menuPoints->addAction(actionLinear);
		menuPoints->addSeparator();
		menuPoints->addAction(actionClear);

		menuLloyd->addAction(actionOpti);
		menuLloyd->addAction(actionLloyd);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Stippling picture", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionClear->setText(QApplication::translate("MainWindow", "Clear", nullptr));
#ifndef QT_NO_STATUSTIP
        actionClear->setStatusTip(QApplication::translate("MainWindow", "Clear", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_SHORTCUT
        actionClear->setShortcut(QApplication::translate("MainWindow", "Space", nullptr));
#endif // QT_NO_SHORTCUT
        actionRandom->setText(QApplication::translate("MainWindow", "Random", nullptr));
		actionLinear->setText(QApplication::translate("MainWindow", "Linar algo for density", nullptr));
		actionOpti->setText(QApplication::translate("MainWindow", "Lloyd initialization", nullptr));
		actionLoad->setText(QApplication::translate("MainWindow", "Load pic", nullptr));
		actionLloyd->setText(QApplication::translate("MainWindow", "Lloyd iteration", nullptr));
        menuPoints->setTitle(QApplication::translate("MainWindow", "Points", nullptr));
		menuPicture->setTitle(QApplication::translate("MainWindow", "Picture", nullptr));
		menuLloyd->setTitle(QApplication::translate("MainWindow", "Lloyd", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STIPPLING_H
