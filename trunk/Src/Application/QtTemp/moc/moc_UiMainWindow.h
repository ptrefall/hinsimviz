/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Sat 25. Sep 14:05:28 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MOC_UIMAINWINDOW_H
#define MOC_UIMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "QtExtensions/GLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_close;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    GLWidget *scene;
    QGridLayout *gridLayout_2;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuObjects;
    QStatusBar *statusbar;
    QDockWidget *properties;
    QWidget *propertiesContents;
    QGridLayout *gridLayout_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1215, 640);
        action_close = new QAction(MainWindow);
        action_close->setObjectName(QString::fromUtf8("action_close"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scene = new GLWidget(centralwidget);
        scene->setObjectName(QString::fromUtf8("scene"));
        gridLayout_2 = new QGridLayout(scene);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

        gridLayout->addWidget(scene, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1215, 21));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuObjects = new QMenu(menubar);
        menuObjects->setObjectName(QString::fromUtf8("menuObjects"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        properties = new QDockWidget(MainWindow);
        properties->setObjectName(QString::fromUtf8("properties"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(173);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(properties->sizePolicy().hasHeightForWidth());
        properties->setSizePolicy(sizePolicy);
        properties->setAutoFillBackground(false);
        properties->setFloating(false);
        properties->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        properties->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        propertiesContents = new QWidget();
        propertiesContents->setObjectName(QString::fromUtf8("propertiesContents"));
        gridLayout_3 = new QGridLayout(propertiesContents);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        properties->setWidget(propertiesContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), properties);
        properties->raise();

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuObjects->menuAction());
        menu_File->addAction(action_close);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Flow3D Viewer", 0, QApplication::UnicodeUTF8));
        action_close->setText(QApplication::translate("MainWindow", "&Close", 0, QApplication::UnicodeUTF8));
        action_close->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuObjects->setTitle(QApplication::translate("MainWindow", "Objects", 0, QApplication::UnicodeUTF8));
        properties->setWindowTitle(QApplication::translate("MainWindow", "  Properties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MOC_UIMAINWINDOW_H
