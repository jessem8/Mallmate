/********************************************************************************
** Form generated from reading UI file 'evacuation.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVACUATION_H
#define UI_EVACUATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Evacuation
{
public:
    QLabel *titleLabel;
    QLabel *pathSelectionLabel;
    QComboBox *pathComboBox;
    QLabel *labelStep3;
    QFrame *pathFrame;
    QLabel *labelStep2;
    QPushButton *btnvalider;
    QPushButton *addStepButton;
    QPushButton *undoStepButton;

    void setupUi(QDialog *Evacuation)
    {
        if (Evacuation->objectName().isEmpty())
            Evacuation->setObjectName("Evacuation");
        Evacuation->resize(550, 660);
        Evacuation->setStyleSheet(QString::fromUtf8("QDialog#Evacuation {\n"
"  background-color: #f8f9fa; /* Fond tr\303\250s l\303\251ger, proche du blanc */\n"
"  font-family: Segoe UI, Arial, sans-serif; /* Police moderne */\n"
"  color: #333333; /* Couleur de texte par d\303\251faut */\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"  border-color: #4A90E2; /* Bleu focus \303\251l\303\251gant */\n"
"  background-color: #F0F8FF; /* Fond focus doux */\n"
"}\n"
"\n"
"QLabel {\n"
"  background-color: transparent;\n"
"  border: none;\n"
"  padding: 2px;\n"
"  font-size: 13px; /* Taille de base */\n"
"  color: #333333; /* Couleur de texte */\n"
"}\n"
"\n"
"QLabel#titleLabel {\n"
"  font-size: 19pt; /* Texte plus grand pour titre */\n"
"  font-weight: bold;\n"
"  color: #d35400; /* Couleur orange/alerte */\n"
"  padding-bottom: 10px;\n"
"  text-align: center; /* Centrage texte */\n"
"}\n"
"\n"
"QLabel#pathSelectionLabel {\n"
"  font-size: 14px; /* Taille l\303\251g\303\250rement plus grande */\n"
"  font-weight: bold;\n"
"  color: #555;\n"
"  margin-top: 8px;\n"
"  padding-b"
                        "ottom: 4px;\n"
"}\n"
"\n"
"QLabel.InstructionLabel {\n"
"  font-size: 16pt; /* Texte lisible et large */\n"
"  color: #2c3e50; /* Couleur s\303\251rieuse et sombre */\n"
"  padding: 5px; /* Espacement uniforme */\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"QPushButton#speakButton {\n"
"  border-radius: 5px; \n"
"  padding: 10px 20px;\n"
"  font-weight: bold;\n"
"  font-size: 14px; \n"
"  min-height: 26px;\n"
"  background-color: #3498db; \n"
"  color: white;\n"
"  border: none; \n"
"  margin-top: 20px; \n"
"}\n"
"\n"
"QPushButton#speakButton:hover {\n"
"  background-color: #2980b9; \n"
"}\n"
"\n"
"QPushButton#speakButton:pressed {\n"
"  background-color: #1f618d; \n"
"}\n"
"\n"
"QPushButton#speakButton:disabled {\n"
"  background-color: #aed6f1; \n"
"  color: #f0f0f0;\n"
"}\n"
"\n"
"QFrame.SeparatorLine {\n"
"  border: none;\n"
"  border-top: 1px solid #dddddd;\n"
"  margin-top: 15px;\n"
"  margin-bottom: 15px;\n"
"}\n"
""));
        titleLabel = new QLabel(Evacuation);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(20, 20, 410, 52));
        titleLabel->setStyleSheet(QString::fromUtf8("font-family: Segoe UI, Arial, sans-serif;\n"
"font-size: 30px; /* Slightly smaller */\n"
"font-weight: bold;\n"
"color: #2c3e50; /* Darker blue/gray */"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pathSelectionLabel = new QLabel(Evacuation);
        pathSelectionLabel->setObjectName("pathSelectionLabel");
        pathSelectionLabel->setGeometry(QRect(20, 116, 192, 33));
        pathComboBox = new QComboBox(Evacuation);
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->addItem(QString());
        pathComboBox->setObjectName("pathComboBox");
        pathComboBox->setGeometry(QRect(20, 160, 281, 41));
        pathComboBox->setStyleSheet(QString::fromUtf8("\n"
"    QComboBox {\n"
"        background-color: #f9f9f9;\n"
"        color: #000000; /* Texte noir */\n"
"        border: 2px solid black;\n"
"        border-radius: 5px;\n"
"        padding: 5px;\n"
"        font-size: 14px;\n"
"    }\n"
"\n"
"    QComboBox:hover {\n"
"        border: 2px solid #999999;\n"
"    }\n"
"\n"
"    QComboBox::drop-down {\n"
"        subcontrol-origin: padding;\n"
"        subcontrol-position: top right;\n"
"        width: 25px;\n"
"        border-left: 1px solid #cccccc;\n"
"        background-color: #f0f0f0;\n"
"    }\n"
"\n"
"    QComboBox::down-arrow {\n"
"        image: url(:/images/images/arrow_down.png); /* ic\303\264ne sombre ou remplace si tu veux */\n"
"        width: 12px;\n"
"        height: 12px;\n"
"    }\n"
"\n"
"    QComboBox QAbstractItemView {\n"
"        background-color: white;\n"
"        color: black;\n"
"        selection-background-color: #e0e0e0;\n"
"        selection-color: black;\n"
"        border: 1px solid #cccccc;\n"
"    }"));
        labelStep3 = new QLabel(Evacuation);
        labelStep3->setObjectName("labelStep3");
        labelStep3->setGeometry(QRect(20, 489, 22, 46));
        labelStep3->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        labelStep3->setWordWrap(true);
        pathFrame = new QFrame(Evacuation);
        pathFrame->setObjectName("pathFrame");
        pathFrame->setGeometry(QRect(10, 220, 521, 321));
        pathFrame->setFrameShape(QFrame::Shape::HLine);
        pathFrame->setFrameShadow(QFrame::Shadow::Sunken);
        labelStep2 = new QLabel(Evacuation);
        labelStep2->setObjectName("labelStep2");
        labelStep2->setGeometry(QRect(20, 393, 22, 46));
        labelStep2->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        labelStep2->setWordWrap(true);
        btnvalider = new QPushButton(Evacuation);
        btnvalider->setObjectName("btnvalider");
        btnvalider->setGeometry(QRect(450, 600, 83, 41));
        btnvalider->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #e74c3c; color: white; border: none; } QPushButton:hover { background-color: #c0392b; } QPushButton:pressed { background-color: #a93226; } QPushButton:disabled { background-color: #f5b7b1; }"));
        addStepButton = new QPushButton(Evacuation);
        addStepButton->setObjectName("addStepButton");
        addStepButton->setGeometry(QRect(320, 160, 83, 41));
        addStepButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #e74c3c; color: white; border: none; } QPushButton:hover { background-color: #c0392b; } QPushButton:pressed { background-color: #a93226; } QPushButton:disabled { background-color: #f5b7b1; }"));
        undoStepButton = new QPushButton(Evacuation);
        undoStepButton->setObjectName("undoStepButton");
        undoStepButton->setGeometry(QRect(410, 160, 83, 41));
        undoStepButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #95a5a6; color: white; border: none; } QPushButton:hover { background-color: #7f8c8d; } QPushButton:pressed { background-color: #616A6B; }"));

        retranslateUi(Evacuation);

        QMetaObject::connectSlotsByName(Evacuation);
    } // setupUi

    void retranslateUi(QDialog *Evacuation)
    {
        Evacuation->setWindowTitle(QCoreApplication::translate("Evacuation", "\342\232\240\357\270\217 Instructions d'\303\211vacuation Personnalis\303\251es", nullptr));
        titleLabel->setText(QCoreApplication::translate("Evacuation", "\360\237\232\250 Instructions d'\303\211vacuation", nullptr));
        pathSelectionLabel->setText(QCoreApplication::translate("Evacuation", "Choisissez votre itin\303\251raire :", nullptr));
        pathComboBox->setItemText(0, QCoreApplication::translate("Evacuation", "-- Choisir un chemin --", nullptr));
        pathComboBox->setItemText(1, QCoreApplication::translate("Evacuation", "\360\237\221\211 Avancez tout droit sans vous arr\303\252ter.", nullptr));
        pathComboBox->setItemText(2, QCoreApplication::translate("Evacuation", "\342\254\205\357\270\217 Prenez \303\240 gauche au prochain couloir.", nullptr));
        pathComboBox->setItemText(3, QCoreApplication::translate("Evacuation", "\342\236\241\357\270\217 Allez \303\240 droite \303\240 l'intersection.", nullptr));
        pathComboBox->setItemText(4, QCoreApplication::translate("Evacuation", "\342\244\265\357\270\217 Tournez \303\240 droite apr\303\250s cet escalier.", nullptr));
        pathComboBox->setItemText(5, QCoreApplication::translate("Evacuation", "\342\254\207\357\270\217 Descendez les escaliers avec pr\303\251caution", nullptr));
        pathComboBox->setItemText(6, QCoreApplication::translate("Evacuation", "\342\254\206\357\270\217 Montez au premier \303\251tage, la sortie est l\303\240-bas", nullptr));
        pathComboBox->setItemText(7, QCoreApplication::translate("Evacuation", "\342\234\213 Vous \303\252tes en train de vous tromper\342\200\257; revenez en arri\303\250re.", nullptr));
        pathComboBox->setItemText(8, QCoreApplication::translate("Evacuation", "\360\237\224\204 Faites demi-tour et reprenez par ici.", nullptr));
        pathComboBox->setItemText(9, QCoreApplication::translate("Evacuation", "\360\237\232\252 Voici la sortie, avancez tout droit.", nullptr));
        pathComboBox->setItemText(10, QCoreApplication::translate("Evacuation", "Other", nullptr));
        pathComboBox->setItemText(11, QString());

#if QT_CONFIG(tooltip)
        pathComboBox->setToolTip(QCoreApplication::translate("Evacuation", "S\303\251lectionnez le chemin d'\303\251vacuation \303\240 suivre", nullptr));
#endif // QT_CONFIG(tooltip)
        labelStep3->setText(QString());
        labelStep3->setProperty("class", QVariant(QCoreApplication::translate("Evacuation", "InstructionLabel", nullptr)));
        pathFrame->setProperty("class", QVariant(QCoreApplication::translate("Evacuation", "SeparatorLine", nullptr)));
        labelStep2->setText(QString());
        labelStep2->setProperty("class", QVariant(QCoreApplication::translate("Evacuation", "InstructionLabel", nullptr)));
        btnvalider->setText(QCoreApplication::translate("Evacuation", "Valider", nullptr));
        addStepButton->setText(QCoreApplication::translate("Evacuation", "ADD", nullptr));
        undoStepButton->setText(QCoreApplication::translate("Evacuation", "Annuler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Evacuation: public Ui_Evacuation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVACUATION_H
