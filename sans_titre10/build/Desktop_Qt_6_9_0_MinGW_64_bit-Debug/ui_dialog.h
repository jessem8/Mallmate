/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_25;
    QLineEdit *telephoneLineEdit;
    QLineEdit *adresseLineEdit;
    QLineEdit *nomLineEdit;
    QLabel *label_3;
    QLineEdit *prenomLineEdit;
    QLineEdit *cinLineEdit;
    QLabel *label_27;
    QLabel *label_24;
    QLineEdit *emailLineEdit;
    QPushButton *ajouterButton_AddForm;
    QPushButton *cancelButton_AddForm;
    QLabel *label_AddClientTitle;
    QComboBox *paysComboBox;
    QLabel *label_26;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(693, 579);
        Dialog->setStyleSheet(QString::fromUtf8(""));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(220, 70, 42, 42));
        label_8 = new QLabel(Dialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(212, 170, 71, 42));
        label_25 = new QLabel(Dialog);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(212, 320, 88, 42));
        telephoneLineEdit = new QLineEdit(Dialog);
        telephoneLineEdit->setObjectName("telephoneLineEdit");
        telephoneLineEdit->setGeometry(QRect(310, 320, 171, 42));
        adresseLineEdit = new QLineEdit(Dialog);
        adresseLineEdit->setObjectName("adresseLineEdit");
        adresseLineEdit->setGeometry(QRect(310, 220, 171, 42));
        nomLineEdit = new QLineEdit(Dialog);
        nomLineEdit->setObjectName("nomLineEdit");
        nomLineEdit->setGeometry(QRect(310, 120, 171, 42));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(212, 120, 51, 42));
        prenomLineEdit = new QLineEdit(Dialog);
        prenomLineEdit->setObjectName("prenomLineEdit");
        prenomLineEdit->setGeometry(QRect(310, 170, 171, 42));
        cinLineEdit = new QLineEdit(Dialog);
        cinLineEdit->setObjectName("cinLineEdit");
        cinLineEdit->setGeometry(QRect(310, 70, 171, 42));
        cinLineEdit->setReadOnly(true);
        label_27 = new QLabel(Dialog);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(212, 270, 54, 42));
        label_24 = new QLabel(Dialog);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(212, 220, 72, 42));
        emailLineEdit = new QLineEdit(Dialog);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setGeometry(QRect(310, 270, 171, 42));
        ajouterButton_AddForm = new QPushButton(Dialog);
        ajouterButton_AddForm->setObjectName("ajouterButton_AddForm");
        ajouterButton_AddForm->setGeometry(QRect(350, 460, 181, 40));
        ajouterButton_AddForm->setMinimumSize(QSize(150, 40));
        ajouterButton_AddForm->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"                      background-color: #2ecc71; /* Green */\n"
"                      color: white;\n"
"                      border: none; border-radius: 4px; padding: 8px 16px;\n"
"                      font-weight: bold;\n"
"                  }\n"
"                  QPushButton:hover { background-color: #28b463; }\n"
"                  QPushButton:pressed { background-color: #239b56; }"));
        ajouterButton_AddForm->setIconSize(QSize(16, 16));
        cancelButton_AddForm = new QPushButton(Dialog);
        cancelButton_AddForm->setObjectName("cancelButton_AddForm");
        cancelButton_AddForm->setGeometry(QRect(210, 460, 120, 40));
        cancelButton_AddForm->setMinimumSize(QSize(120, 40));
        cancelButton_AddForm->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 4px; padding: 8px 16px; font-weight: bold; } QPushButton:hover { background-color: #7f8c8d; } QPushButton:pressed { background-color: #616A6B; }"));
        label_AddClientTitle = new QLabel(Dialog);
        label_AddClientTitle->setObjectName("label_AddClientTitle");
        label_AddClientTitle->setGeometry(QRect(70, 20, 550, 29));
        label_AddClientTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #3498db; border: none; background: transparent;"));
        label_AddClientTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        paysComboBox = new QComboBox(Dialog);
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->setObjectName("paysComboBox");
        paysComboBox->setGeometry(QRect(310, 380, 171, 36));
        paysComboBox->setMinimumSize(QSize(0, 36));
        label_26 = new QLabel(Dialog);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(200, 370, 88, 42));

        retranslateUi(Dialog);

        ajouterButton_AddForm->setDefault(true);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "CIN :", nullptr));
        label_8->setText(QCoreApplication::translate("Dialog", "Pr\303\251nom :", nullptr));
        label_25->setText(QCoreApplication::translate("Dialog", "T\303\251l\303\251phone :", nullptr));
        telephoneLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "Num\303\251ro de t\303\251l\303\251phone", nullptr));
        adresseLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "Adresse postale", nullptr));
        nomLineEdit->setText(QString());
        nomLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "Nom de famille", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Nom :", nullptr));
        prenomLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "Pr\303\251nom", nullptr));
        cinLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "Num\303\251ro CIN", nullptr));
        label_27->setText(QCoreApplication::translate("Dialog", "Email :", nullptr));
        label_24->setText(QCoreApplication::translate("Dialog", "Adresse :", nullptr));
        emailLineEdit->setPlaceholderText(QCoreApplication::translate("Dialog", "adresse@example.com", nullptr));
        ajouterButton_AddForm->setText(QCoreApplication::translate("Dialog", "\342\234\224\357\270\217 Enregistrer Client", nullptr));
        cancelButton_AddForm->setText(QCoreApplication::translate("Dialog", "Annuler", nullptr));
        label_AddClientTitle->setText(QCoreApplication::translate("Dialog", "Modifier un  Client", nullptr));
        paysComboBox->setItemText(0, QCoreApplication::translate("Dialog", "Tunisie", nullptr));
        paysComboBox->setItemText(1, QCoreApplication::translate("Dialog", "Palestine", nullptr));
        paysComboBox->setItemText(2, QCoreApplication::translate("Dialog", "France", nullptr));
        paysComboBox->setItemText(3, QCoreApplication::translate("Dialog", "UA", nullptr));

        label_26->setText(QCoreApplication::translate("Dialog", "Pays:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
