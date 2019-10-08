/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "canvas.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label;
    QLineEdit *editFile;
    QPushButton *btnBrowse;
    QLabel *time;
    QPushButton *btnDetect;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    Canvas *source;
    Canvas *result;
    QLabel *message;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(802, 700);
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 61, 16));
        editFile = new QLineEdit(Dialog);
        editFile->setObjectName(QString::fromUtf8("editFile"));
        editFile->setGeometry(QRect(80, 30, 641, 31));
        btnBrowse = new QPushButton(Dialog);
        btnBrowse->setObjectName(QString::fromUtf8("btnBrowse"));
        btnBrowse->setGeometry(QRect(730, 30, 51, 31));
        time = new QLabel(Dialog);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(420, 660, 371, 31));
        btnDetect = new QPushButton(Dialog);
        btnDetect->setObjectName(QString::fromUtf8("btnDetect"));
        btnDetect->setGeometry(QRect(310, 660, 93, 28));
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 90, 801, 551));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        source = new Canvas(horizontalLayoutWidget);
        source->setObjectName(QString::fromUtf8("source"));

        horizontalLayout->addWidget(source);

        result = new Canvas(horizontalLayoutWidget);
        result->setObjectName(QString::fromUtf8("result"));

        horizontalLayout->addWidget(result);

        message = new QLabel(Dialog);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(0, 660, 301, 21));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog", "Image", nullptr));
        btnBrowse->setText(QApplication::translate("Dialog", "...", nullptr));
        time->setText(QApplication::translate("Dialog", "Time:", nullptr));
        btnDetect->setText(QApplication::translate("Dialog", "Detect", nullptr));
        source->setText(QApplication::translate("Dialog", "source", nullptr));
        result->setText(QApplication::translate("Dialog", "result", nullptr));
        message->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
