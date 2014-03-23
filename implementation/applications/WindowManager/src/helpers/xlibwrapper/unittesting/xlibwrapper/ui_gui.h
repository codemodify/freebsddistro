/********************************************************************************
** Form generated from reading ui file 'gui.ui'
**
** Created: Tue Mar 13 13:39:42 2007
**      by: Qt User Interface Compiler version 4.2.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_Form
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QPushButton *_setDisplayButton;
    QLineEdit *_display;
    QWidget *layoutWidget1;
    QHBoxLayout *hboxLayout1;
    QPushButton *_createdWindowButton;
    QLineEdit *_createdWindow;
    QWidget *layoutWidget_3;
    QHBoxLayout *hboxLayout2;
    QPushButton *_setTitleTextButton;
    QLineEdit *_windowToSetTitleText;
    QLineEdit *_titleTextToSet;
    QWidget *layoutWidget_2;
    QHBoxLayout *hboxLayout3;
    QPushButton *_showWindowButton;
    QLineEdit *_windowToShow;
    QWidget *layoutWidget_4;
    QHBoxLayout *hboxLayout4;
    QPushButton *_getTitleTextButton;
    QLineEdit *_windowToGetTitleText;
    QLineEdit *_titleText;
    QWidget *layoutWidget_5;
    QHBoxLayout *hboxLayout5;
    QPushButton *_hideWindowButton;
    QLineEdit *_windowToHide;
    QWidget *layoutWidget_6;
    QHBoxLayout *hboxLayout6;
    QPushButton *_focusWindowButton;
    QLineEdit *_windowToFocus;
    QWidget *layoutWidget_7;
    QHBoxLayout *hboxLayout7;
    QPushButton *_bringWindowOnTopButton;
    QLineEdit *_windowToBring;
    QWidget *layoutWidget_8;
    QHBoxLayout *hboxLayout8;
    QPushButton *_sendWindowToBackButton;
    QLineEdit *_windowToSend;

    void setupUi(QWidget *Form)
    {
    Form->setObjectName(QString::fromUtf8("Form"));
    layoutWidget = new QWidget(Form);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 10, 351, 27));
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    _setDisplayButton = new QPushButton(layoutWidget);
    _setDisplayButton->setObjectName(QString::fromUtf8("_setDisplayButton"));

    hboxLayout->addWidget(_setDisplayButton);

    _display = new QLineEdit(layoutWidget);
    _display->setObjectName(QString::fromUtf8("_display"));
    _display->setReadOnly(true);

    hboxLayout->addWidget(_display);

    layoutWidget1 = new QWidget(Form);
    layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
    layoutWidget1->setGeometry(QRect(10, 40, 351, 27));
    hboxLayout1 = new QHBoxLayout(layoutWidget1);
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    _createdWindowButton = new QPushButton(layoutWidget1);
    _createdWindowButton->setObjectName(QString::fromUtf8("_createdWindowButton"));

    hboxLayout1->addWidget(_createdWindowButton);

    _createdWindow = new QLineEdit(layoutWidget1);
    _createdWindow->setObjectName(QString::fromUtf8("_createdWindow"));
    _createdWindow->setReadOnly(true);

    hboxLayout1->addWidget(_createdWindow);

    layoutWidget_3 = new QWidget(Form);
    layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
    layoutWidget_3->setGeometry(QRect(10, 100, 351, 29));
    hboxLayout2 = new QHBoxLayout(layoutWidget_3);
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    _setTitleTextButton = new QPushButton(layoutWidget_3);
    _setTitleTextButton->setObjectName(QString::fromUtf8("_setTitleTextButton"));

    hboxLayout2->addWidget(_setTitleTextButton);

    _windowToSetTitleText = new QLineEdit(layoutWidget_3);
    _windowToSetTitleText->setObjectName(QString::fromUtf8("_windowToSetTitleText"));

    hboxLayout2->addWidget(_windowToSetTitleText);

    _titleTextToSet = new QLineEdit(layoutWidget_3);
    _titleTextToSet->setObjectName(QString::fromUtf8("_titleTextToSet"));
    _titleTextToSet->setReadOnly(false);

    hboxLayout2->addWidget(_titleTextToSet);

    layoutWidget_2 = new QWidget(Form);
    layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
    layoutWidget_2->setGeometry(QRect(10, 70, 351, 27));
    hboxLayout3 = new QHBoxLayout(layoutWidget_2);
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(0);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    _showWindowButton = new QPushButton(layoutWidget_2);
    _showWindowButton->setObjectName(QString::fromUtf8("_showWindowButton"));

    hboxLayout3->addWidget(_showWindowButton);

    _windowToShow = new QLineEdit(layoutWidget_2);
    _windowToShow->setObjectName(QString::fromUtf8("_windowToShow"));
    _windowToShow->setReadOnly(false);

    hboxLayout3->addWidget(_windowToShow);

    layoutWidget_4 = new QWidget(Form);
    layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
    layoutWidget_4->setGeometry(QRect(10, 130, 351, 27));
    hboxLayout4 = new QHBoxLayout(layoutWidget_4);
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(0);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    _getTitleTextButton = new QPushButton(layoutWidget_4);
    _getTitleTextButton->setObjectName(QString::fromUtf8("_getTitleTextButton"));

    hboxLayout4->addWidget(_getTitleTextButton);

    _windowToGetTitleText = new QLineEdit(layoutWidget_4);
    _windowToGetTitleText->setObjectName(QString::fromUtf8("_windowToGetTitleText"));

    hboxLayout4->addWidget(_windowToGetTitleText);

    _titleText = new QLineEdit(layoutWidget_4);
    _titleText->setObjectName(QString::fromUtf8("_titleText"));
    _titleText->setReadOnly(false);

    hboxLayout4->addWidget(_titleText);

    layoutWidget_5 = new QWidget(Form);
    layoutWidget_5->setObjectName(QString::fromUtf8("layoutWidget_5"));
    layoutWidget_5->setGeometry(QRect(10, 180, 351, 27));
    hboxLayout5 = new QHBoxLayout(layoutWidget_5);
    hboxLayout5->setSpacing(6);
    hboxLayout5->setMargin(0);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    _hideWindowButton = new QPushButton(layoutWidget_5);
    _hideWindowButton->setObjectName(QString::fromUtf8("_hideWindowButton"));

    hboxLayout5->addWidget(_hideWindowButton);

    _windowToHide = new QLineEdit(layoutWidget_5);
    _windowToHide->setObjectName(QString::fromUtf8("_windowToHide"));
    _windowToHide->setReadOnly(false);

    hboxLayout5->addWidget(_windowToHide);

    layoutWidget_6 = new QWidget(Form);
    layoutWidget_6->setObjectName(QString::fromUtf8("layoutWidget_6"));
    layoutWidget_6->setGeometry(QRect(10, 210, 351, 27));
    hboxLayout6 = new QHBoxLayout(layoutWidget_6);
    hboxLayout6->setSpacing(6);
    hboxLayout6->setMargin(0);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    _focusWindowButton = new QPushButton(layoutWidget_6);
    _focusWindowButton->setObjectName(QString::fromUtf8("_focusWindowButton"));

    hboxLayout6->addWidget(_focusWindowButton);

    _windowToFocus = new QLineEdit(layoutWidget_6);
    _windowToFocus->setObjectName(QString::fromUtf8("_windowToFocus"));
    _windowToFocus->setReadOnly(false);

    hboxLayout6->addWidget(_windowToFocus);

    layoutWidget_7 = new QWidget(Form);
    layoutWidget_7->setObjectName(QString::fromUtf8("layoutWidget_7"));
    layoutWidget_7->setGeometry(QRect(10, 240, 351, 27));
    hboxLayout7 = new QHBoxLayout(layoutWidget_7);
    hboxLayout7->setSpacing(6);
    hboxLayout7->setMargin(0);
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    _bringWindowOnTopButton = new QPushButton(layoutWidget_7);
    _bringWindowOnTopButton->setObjectName(QString::fromUtf8("_bringWindowOnTopButton"));

    hboxLayout7->addWidget(_bringWindowOnTopButton);

    _windowToBring = new QLineEdit(layoutWidget_7);
    _windowToBring->setObjectName(QString::fromUtf8("_windowToBring"));
    _windowToBring->setReadOnly(false);

    hboxLayout7->addWidget(_windowToBring);

    layoutWidget_8 = new QWidget(Form);
    layoutWidget_8->setObjectName(QString::fromUtf8("layoutWidget_8"));
    layoutWidget_8->setGeometry(QRect(10, 270, 351, 27));
    hboxLayout8 = new QHBoxLayout(layoutWidget_8);
    hboxLayout8->setSpacing(6);
    hboxLayout8->setMargin(0);
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    _sendWindowToBackButton = new QPushButton(layoutWidget_8);
    _sendWindowToBackButton->setObjectName(QString::fromUtf8("_sendWindowToBackButton"));

    hboxLayout8->addWidget(_sendWindowToBackButton);

    _windowToSend = new QLineEdit(layoutWidget_8);
    _windowToSend->setObjectName(QString::fromUtf8("_windowToSend"));
    _windowToSend->setReadOnly(false);

    hboxLayout8->addWidget(_windowToSend);


    retranslateUi(Form);

    QSize size(376, 309);
    size = size.expandedTo(Form->minimumSizeHint());
    Form->resize(size);


    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    _setDisplayButton->setText(QApplication::translate("Form", "setDisplay()", 0, QApplication::UnicodeUTF8));
    _createdWindowButton->setText(QApplication::translate("Form", "createWindow()", 0, QApplication::UnicodeUTF8));
    _setTitleTextButton->setText(QApplication::translate("Form", "setWindowTitleText()", 0, QApplication::UnicodeUTF8));
    _titleTextToSet->setText(QApplication::translate("Form", "some text", 0, QApplication::UnicodeUTF8));
    _showWindowButton->setText(QApplication::translate("Form", "showWindow()", 0, QApplication::UnicodeUTF8));
    _getTitleTextButton->setText(QApplication::translate("Form", "getTitleText()", 0, QApplication::UnicodeUTF8));
    _hideWindowButton->setText(QApplication::translate("Form", "hideWindow()", 0, QApplication::UnicodeUTF8));
    _focusWindowButton->setText(QApplication::translate("Form", "focusWindow()", 0, QApplication::UnicodeUTF8));
    _bringWindowOnTopButton->setText(QApplication::translate("Form", "bringWindowOnTop()", 0, QApplication::UnicodeUTF8));
    _sendWindowToBackButton->setText(QApplication::translate("Form", "sendWindowToBack()", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

#endif // UI_GUI_H
