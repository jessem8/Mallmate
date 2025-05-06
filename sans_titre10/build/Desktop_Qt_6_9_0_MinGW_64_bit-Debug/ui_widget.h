/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QAction *actionExport;
    QAction *actionExportPDF;
    QAction *actionPrint;
    QAction *actionSettings;
    QListWidget *sidebarListWidget;
    QWidget *mainContentWidget;
    QVBoxLayout *verticalLayout_MainContent;
    QWidget *headerWidget;
    QHBoxLayout *horizontalLayout_Header;
    QLabel *label_28;
    QLabel *label_23;
    QStackedWidget *stackedWidget;
    QWidget *welcomePage;
    QVBoxLayout *verticalLayout_Welcome;
    QLabel *label_WelcomeTitle;
    QLabel *label_WelcomeDesc;
    QSpacerItem *verticalSpacer_Welcome;
    QWidget *clientListPage;
    QVBoxLayout *verticalLayout_ClientList;
    QWidget *clientListContainer;
    QTableWidget *clientTableWidget;
    QPushButton *addButton_List;
    QPushButton *deleteButton;
    QPushButton *modifyButton;
    QLineEdit *searchLineEdit;
    QComboBox *comboBox_SortBy;
    QToolButton *functionalityMenuButton;
    QPushButton *consulterbtn;
    QWidget *addClientPage;
    QWidget *addClientFormContainer;
    QLabel *label_WelcomeDesc_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_AddFormActions;
    QSpacerItem *horizontalSpacer_AddForm;
    QPushButton *ajouterButton_AddForm;
    QPushButton *cancelButton_AddForm;
    QLabel *label_27;
    QLineEdit *cinLineEdit;
    QLabel *label_4;
    QComboBox *paysComboBox;
    QLabel *label_24;
    QLineEdit *emailLineEdit;
    QLineEdit *prenomLineEdit;
    QLineEdit *adresseLineEdit;
    QLabel *label_3;
    QLineEdit *telephoneLineEdit;
    QLineEdit *nomLineEdit;
    QLabel *label_Zone;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_25;
    QComboBox *zoneComboBox;
    QLabel *label_AddClientTitle;
    QLabel *label_StatsTitle_2;
    QWidget *statisticsPage;
    QVBoxLayout *verticalLayout_Stats;
    QLabel *label_StatsTitle;
    QWidget *statsControlWidget;
    QHBoxLayout *horizontalLayout_StatsControls;
    QLabel *label_TimeRange;
    QComboBox *comboBoxchoisi;
    QSpacerItem *horizontalSpacer_StatsControls;
    QFrame *chartFrame;
    QVBoxLayout *verticalLayout_ChartContainer;
    QLabel *chartInfoLabel;
    QWidget *riskManagementPage;
    QVBoxLayout *verticalLayout_RiskMgmt;
    QLabel *label_RiskMgmtTitle;
    QWidget *riskInputContainer;
    QHBoxLayout *horizontalLayout_RiskInput;
    QLabel *label_RiskType;
    QComboBox *comboBox_RiskType;
    QLabel *label_RiskZone;
    QComboBox *comboBox_RiskZone;
    QLabel *label_RiskDate;
    QDateEdit *dateEdit_RiskDate;
    QSpacerItem *horizontalSpacer_RiskInput;
    QPushButton *button_ReportRisk;
    QTableWidget *tableWidget_RiskClients;
    QWidget *notificationsPage;
    QVBoxLayout *verticalLayout_Notifications;
    QLabel *label_NotificationsTitle;
    QListWidget *notificationsListWidget;
    QHBoxLayout *horizontalLayout_NotificationActions;
    QSpacerItem *horizontalSpacer_Notifications;
    QPushButton *button_ClearNotifications;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(1209, 737);
        Widget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: white;\n"
"    font-family: Segoe UI, Arial, sans-serif;\n"
"    color: #333333;\n"
"}\n"
"/* Global focus style for input widgets for consistency */\n"
"QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QTextEdit:focus, QDateEdit:focus { /* Added QDateEdit */\n"
"    border-color: #4A90E2;\n"
"    background-color: #F0F8FF;\n"
"}\n"
"\n"
"/* Basic Button Style (can be overridden) */\n"
"QPushButton {\n"
"    border-radius: 4px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    min-height: 20px; /* Ensure minimum height */\n"
"}\n"
"QPushButton:disabled {\n"
"    color: #f0f0f0; /* Lighter text color */\n"
"    background-color: #dcdcdc; /* Lighter background */\n"
"    border-color: #c0c0c0; /* Lighter border */\n"
"}\n"
"\n"
"\n"
"/* Basic Label Style (can be overridden) */\n"
"QLabel {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    padding: 2px; /* Add minimal padding */\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"/* "
                        "Basic Input Field Style (QLineEdit, QComboBox, QDateEdit - can be overridden) */\n"
"QLineEdit, QComboBox, QDateEdit { /* Added QDateEdit */\n"
"    border: 1px solid #D0D0D0;\n"
"    border-radius: 4px;\n"
"    padding: 7px 10px; /* Adjusted padding */\n"
"    font-size: 13px;\n"
"    color: #333333;\n"
"    background-color: white;\n"
"    min-height: 20px; /* Consistent height */\n"
"}\n"
"\n"
"/* Specific ComboBox Dropdown Arrow */\n"
"QComboBox::drop-down {\n"
"    subcontrol-origin: padding; subcontrol-position: top right; width: 20px;\n"
"    border-left: 1px solid #D5D5D5; border-top-right-radius: 4px; border-bottom-right-radius: 4px; background-color: white;\n"
"}\n"
"QComboBox::down-arrow { image: url(:/images/images/arrow_down.png); width: 12px; height: 12px; } /* Ensure resource */\n"
"QComboBox QAbstractItemView { /* Dropdown list style */\n"
"   border: 1px solid #BDBDBD; background-color: white; padding: 2px; outline: none;\n"
"   selection-background-color: #D6EAF8; selection-color: #154360; fo"
                        "nt-size: 13px;\n"
"}\n"
"QComboBox QAbstractItemView::item { padding: 5px 10px; min-height: 24px; }\n"
"QComboBox QAbstractItemView::item:hover { background-color: #E8F0FE; color: #1A237E; }\n"
"\n"
"/* Specific DateEdit Dropdown Arrow (optional consistency) */\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding; subcontrol-position: top right; width: 20px;\n"
"    border-left: 1px solid #D5D5D5; border-top-right-radius: 4px; border-bottom-right-radius: 4px; background-color: white;\n"
"}\n"
"QDateEdit::down-arrow { image: url(:/images/images/arrow_down.png); width: 12px; height: 12px; }\n"
""));
        actionExport = new QAction(Widget);
        actionExport->setObjectName("actionExport");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/export.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionExport->setIcon(icon);
        actionExportPDF = new QAction(Widget);
        actionExportPDF->setObjectName("actionExportPDF");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/pdf.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionExportPDF->setIcon(icon1);
        actionPrint = new QAction(Widget);
        actionPrint->setObjectName("actionPrint");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/print.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionPrint->setIcon(icon2);
        actionSettings = new QAction(Widget);
        actionSettings->setObjectName("actionSettings");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/settings.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionSettings->setIcon(icon3);
        sidebarListWidget = new QListWidget(Widget);
        new QListWidgetItem(sidebarListWidget);
        new QListWidgetItem(sidebarListWidget);
        new QListWidgetItem(sidebarListWidget);
        new QListWidgetItem(sidebarListWidget);
        new QListWidgetItem(sidebarListWidget);
        sidebarListWidget->setObjectName("sidebarListWidget");
        sidebarListWidget->setGeometry(QRect(0, 0, 190, 707));
        sidebarListWidget->setMinimumSize(QSize(190, 0));
        sidebarListWidget->setMaximumSize(QSize(220, 16777215));
        sidebarListWidget->setStyleSheet(QString::fromUtf8("/* Sidebar List Widget */\n"
"#sidebarListWidget {\n"
"    background-color: #F5F5F5;\n"
"    border: none; /* Remove default border */\n"
"    border-right: 1px solid #E0E0E0; /* Separator line */\n"
"    padding: 10px 5px; /* Internal padding */\n"
"    outline: none; /* Remove focus outline */\n"
"}\n"
"\n"
"/* List Items */\n"
"#sidebarListWidget::item {\n"
"    padding: 12px 15px; /* Adjusted padding */\n"
"    color: #555555;\n"
"    min-height: 30px;\n"
"    border-radius: 6px;\n"
"    margin: 2px 0px; /* Spacing between items */\n"
"    border-left: 4px solid transparent; /* Reserve space for selection indicator */\n"
"}\n"
"\n"
"/* Selected Item */\n"
"#sidebarListWidget::item:selected {\n"
"    background-color: #E3F2FD;\n"
"    color: #0D47A1;\n"
"    border-left: 4px solid #0D47A1;\n"
"    font-weight: bold;\n"
"}\n"
"#sidebarListWidget::item:selected:!active { /* Style when widget is inactive */\n"
"     background-color: #E8E8E8;\n"
"     color: #444;\n"
"     border-left-color: #BBB;\n"
"}\n"
"\n"
""
                        "\n"
"/* Hover (non-s\303\251lectionn\303\251) */\n"
"#sidebarListWidget::item:hover:!selected {\n"
"    background-color: #ECEFF1;\n"
"    border-left: 4px solid #B0BEC5;\n"
"    color: #333;\n"
"}\n"
""));
        mainContentWidget = new QWidget(Widget);
        mainContentWidget->setObjectName("mainContentWidget");
        mainContentWidget->setGeometry(QRect(176, 0, 1027, 711));
        mainContentWidget->setStyleSheet(QString::fromUtf8("/* Keep background white, remove any borders */\n"
"#mainContentWidget {\n"
"    background-color: white;\n"
"    border: none;\n"
"}"));
        verticalLayout_MainContent = new QVBoxLayout(mainContentWidget);
        verticalLayout_MainContent->setSpacing(10);
        verticalLayout_MainContent->setObjectName("verticalLayout_MainContent");
        verticalLayout_MainContent->setContentsMargins(15, 15, 15, 15);
        headerWidget = new QWidget(mainContentWidget);
        headerWidget->setObjectName("headerWidget");
        horizontalLayout_Header = new QHBoxLayout(headerWidget);
        horizontalLayout_Header->setObjectName("horizontalLayout_Header");
        horizontalLayout_Header->setContentsMargins(0, 0, 0, 0);
        label_28 = new QLabel(headerWidget);
        label_28->setObjectName("label_28");
        label_28->setStyleSheet(QString::fromUtf8("font-family: Segoe UI, Arial, sans-serif;\n"
"font-size: 26px; /* Slightly smaller */\n"
"font-weight: bold;\n"
"color: #2c3e50; /* Darker blue/gray */"));

        horizontalLayout_Header->addWidget(label_28);

        label_23 = new QLabel(headerWidget);
        label_23->setObjectName("label_23");
        label_23->setMinimumSize(QSize(0, 0));
        label_23->setMaximumSize(QSize(154, 90));
        label_23->setStyleSheet(QString::fromUtf8("/* Removed conflicting width/height rules, rely on max size */\n"
"    object-fit: cover;   /* Permet \303\240 l'image de remplir l'espace sans d\303\251former les proportions */\n"
"    max-width: 150%;     /* Corrected: limit to container width */\n"
"    max-height: 86px;    /* Hauteur maximale de l'image */"));
        label_23->setPixmap(QPixmap(QString::fromUtf8(":/images/images/image2.jpg")));
        label_23->setScaledContents(true);

        horizontalLayout_Header->addWidget(label_23);


        verticalLayout_MainContent->addWidget(headerWidget);

        stackedWidget = new QStackedWidget(mainContentWidget);
        stackedWidget->setObjectName("stackedWidget");
        welcomePage = new QWidget();
        welcomePage->setObjectName("welcomePage");
        verticalLayout_Welcome = new QVBoxLayout(welcomePage);
        verticalLayout_Welcome->setObjectName("verticalLayout_Welcome");
        label_WelcomeTitle = new QLabel(welcomePage);
        label_WelcomeTitle->setObjectName("label_WelcomeTitle");
        label_WelcomeTitle->setStyleSheet(QString::fromUtf8("font-size: 20px; font-weight: bold; color: #3498db;"));

        verticalLayout_Welcome->addWidget(label_WelcomeTitle);

        label_WelcomeDesc = new QLabel(welcomePage);
        label_WelcomeDesc->setObjectName("label_WelcomeDesc");
        label_WelcomeDesc->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7f8c8d;"));
        label_WelcomeDesc->setWordWrap(true);

        verticalLayout_Welcome->addWidget(label_WelcomeDesc);

        verticalSpacer_Welcome = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Welcome->addItem(verticalSpacer_Welcome);

        stackedWidget->addWidget(welcomePage);
        clientListPage = new QWidget();
        clientListPage->setObjectName("clientListPage");
        verticalLayout_ClientList = new QVBoxLayout(clientListPage);
        verticalLayout_ClientList->setObjectName("verticalLayout_ClientList");
        clientListContainer = new QWidget(clientListPage);
        clientListContainer->setObjectName("clientListContainer");
        clientListContainer->setStyleSheet(QString::fromUtf8("#clientListContainer {\n"
"                 border: 1px solid #E0E0E0; /* Lighter border */\n"
"                 border-radius: 5px; /* Rounded corners */\n"
"                 background-color: #FFFFFF; /* Ensure white background */\n"
"              }"));
        clientTableWidget = new QTableWidget(clientListContainer);
        if (clientTableWidget->columnCount() < 8)
            clientTableWidget->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        clientTableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        clientTableWidget->setObjectName("clientTableWidget");
        clientTableWidget->setGeometry(QRect(15, 60, 901, 411));
        clientTableWidget->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    border: 1px solid #E0E0E0; gridline-color: #F0F0F0; background-color: #FFFFFF;\n"
"    alternate-background-color: #F9F9F9; selection-background-color: #AED6F1; selection-color: #222222;\n"
"    outline: none; font-size: 13px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 8px 12px; border-bottom: 1px solid #F0F0F0; border-right: 1px solid #F0F0F0;\n"
"}\n"
"QTableWidget::item:last-child { border-right: none; }\n"
"QTableWidget::item:selected { background-color: #AED6F1; color: #1A5276; border-bottom: 1px solid #9ACEEB; border-right-color: #9ACEEB; }\n"
"QTableWidget::item:selected:last-child { border-right: none; }\n"
"QTableWidget::item:hover { background-color: #EAF2F8; }\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #F1F3F4; padding: 8px; border: none; border-bottom: 2px solid #D5D8DC;\n"
"    font-weight: bold; color: #424949; text-align: left; padding-left: 12px; min-height: 26px;\n"
"}\n"
""));
        clientTableWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        clientTableWidget->setAlternatingRowColors(true);
        clientTableWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        clientTableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        clientTableWidget->setShowGrid(true);
        clientTableWidget->setGridStyle(Qt::PenStyle::SolidLine);
        clientTableWidget->setSortingEnabled(true);
        clientTableWidget->horizontalHeader()->setStretchLastSection(true);
        addButton_List = new QPushButton(clientListContainer);
        addButton_List->setObjectName("addButton_List");
        addButton_List->setGeometry(QRect(800, 510, 120, 36));
        addButton_List->setMinimumSize(QSize(120, 36));
        addButton_List->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #5dade2; color: white; border: none; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #2874a6; } QPushButton:disabled { background-color: #aed6f1; }"));
        addButton_List->setIconSize(QSize(16, 16));
        deleteButton = new QPushButton(clientListContainer);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setGeometry(QRect(143, 503, 120, 36));
        deleteButton->setMinimumSize(QSize(120, 36));
        deleteButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #5dade2; color: white; border: none; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #2874a6; } QPushButton:disabled { background-color: #aed6f1; }"));
        deleteButton->setIconSize(QSize(16, 16));
        modifyButton = new QPushButton(clientListContainer);
        modifyButton->setObjectName("modifyButton");
        modifyButton->setGeometry(QRect(16, 503, 120, 36));
        modifyButton->setMinimumSize(QSize(120, 36));
        modifyButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #5dade2; color: white; border: none; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #2874a6; } QPushButton:disabled { background-color: #aed6f1; }"));
        modifyButton->setIconSize(QSize(16, 16));
        searchLineEdit = new QLineEdit(clientListContainer);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setGeometry(QRect(206, 14, 250, 36));
        searchLineEdit->setMinimumSize(QSize(250, 36));
        searchLineEdit->setStyleSheet(QString::fromUtf8("#searchLineEdit {\n"
"    padding-left: 28px; /* Padding for potential icon */\n"
"    /* background-image: url(:/images/images/search_icon.png); /* Example: add via code */\n"
"    background-repeat: no-repeat;\n"
"    background-position: left 7px center;\n"
"    background-origin: padding-box;\n"
"}\n"
"/* Add search icon using QAction in code instead of CSS background */\n"
""));
        searchLineEdit->setClearButtonEnabled(true);
        comboBox_SortBy = new QComboBox(clientListContainer);
        comboBox_SortBy->addItem(QString());
        comboBox_SortBy->addItem(QString());
        comboBox_SortBy->addItem(QString());
        comboBox_SortBy->addItem(QString());
        comboBox_SortBy->setObjectName("comboBox_SortBy");
        comboBox_SortBy->setGeometry(QRect(16, 14, 183, 36));
        comboBox_SortBy->setMinimumSize(QSize(120, 36));
        comboBox_SortBy->setStyleSheet(QString::fromUtf8("/* Reusing general ComboBox style defined globally */"));
        functionalityMenuButton = new QToolButton(clientListContainer);
        functionalityMenuButton->setObjectName("functionalityMenuButton");
        functionalityMenuButton->setGeometry(QRect(870, 10, 45, 41));
        functionalityMenuButton->setMinimumSize(QSize(42, 34));
        functionalityMenuButton->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"    border: 1px solid #C5C5C5;\n"
"    padding: 5px;\n"
"    background-color: white;\n"
"    color: #333;\n"
"    border-radius: 4px;\n"
"    min-width: 30px;\n"
"}\n"
"QToolButton:hover { background-color: #F0F8FF; border-color: #A0A0A0; }\n"
"QToolButton:focus { border-color: #4A90E2; background-color: #E3F2FD; }\n"
"QToolButton:pressed { background-color: #E3F2FD; }\n"
"QToolButton[popupMode=\"1\"] { /* Optional: space for indicator */ }\n"
"QToolButton::menu-indicator { width: 16px; height: 16px; }\n"
"QToolButton:!menu-indicator { image: none; }\n"
"QToolButton::menu { background-color: white; border: 1px solid #C0C0C0; border-radius: 4px; min-width: 160px; padding: 5px; }\n"
"QToolButton QMenu::item { padding: 8px 15px; background-color: transparent; }\n"
"QToolButton QMenu::item:selected { background-color: #E8F0FE; color: #1A237E; }\n"
"QToolButton QMenu::separator { height: 1px; background-color: #E0E0E0; margin: 5px 0; }\n"
""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/menu_dots.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        functionalityMenuButton->setIcon(icon4);
        functionalityMenuButton->setIconSize(QSize(16, 16));
        functionalityMenuButton->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
        functionalityMenuButton->setAutoRaise(false);
        consulterbtn = new QPushButton(clientListContainer);
        consulterbtn->setObjectName("consulterbtn");
        consulterbtn->setGeometry(QRect(670, 510, 120, 36));
        consulterbtn->setMinimumSize(QSize(120, 36));
        consulterbtn->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #5dade2; color: white; border: none; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #2874a6; } QPushButton:disabled { background-color: #aed6f1; }"));
        consulterbtn->setIconSize(QSize(16, 16));

        verticalLayout_ClientList->addWidget(clientListContainer);

        stackedWidget->addWidget(clientListPage);
        addClientPage = new QWidget();
        addClientPage->setObjectName("addClientPage");
        addClientFormContainer = new QWidget(addClientPage);
        addClientFormContainer->setObjectName("addClientFormContainer");
        addClientFormContainer->setGeometry(QRect(200, 30, 491, 541));
        addClientFormContainer->setStyleSheet(QString::fromUtf8("#addClientFormContainer {\n"
"                border: 1px solid #E0E0E0;\n"
"                border-radius: 5px;\n"
"                background-color: #FFFFFF;\n"
"                /* Removed max-width/auto margins to allow full expansion */\n"
"                /* Added padding */\n"
"                padding: 20px;\n"
"             }\n"
"             #addClientFormContainer QLabel { /* Style labels *only* within this form */\n"
"                 font-weight: bold;\n"
"                 color: #555;\n"
"                 padding-top: 8px; /* Align with line edit center */\n"
"                 min-width: 80px; /* Ensure labels have enough space */\n"
"                 /* Removed alignment */\n"
"             }\n"
"             /* Input field styling is inherited or defined globally */\n"
"             "));
        label_WelcomeDesc_2 = new QLabel(addClientFormContainer);
        label_WelcomeDesc_2->setObjectName("label_WelcomeDesc_2");
        label_WelcomeDesc_2->setGeometry(QRect(50, 20, 421, 41));
        label_WelcomeDesc_2->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #7f8c8d;"));
        label_WelcomeDesc_2->setWordWrap(true);
        layoutWidget = new QWidget(addClientFormContainer);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(70, 480, 344, 38));
        horizontalLayout_AddFormActions = new QHBoxLayout(layoutWidget);
        horizontalLayout_AddFormActions->setSpacing(10);
        horizontalLayout_AddFormActions->setObjectName("horizontalLayout_AddFormActions");
        horizontalLayout_AddFormActions->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_AddForm = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_AddFormActions->addItem(horizontalSpacer_AddForm);

        ajouterButton_AddForm = new QPushButton(layoutWidget);
        ajouterButton_AddForm->setObjectName("ajouterButton_AddForm");
        ajouterButton_AddForm->setMinimumSize(QSize(150, 36));
        ajouterButton_AddForm->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #5dade2; color: white; border: none; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #2874a6; } QPushButton:disabled { background-color: #aed6f1; }"));
        ajouterButton_AddForm->setIconSize(QSize(16, 16));

        horizontalLayout_AddFormActions->addWidget(ajouterButton_AddForm);

        cancelButton_AddForm = new QPushButton(layoutWidget);
        cancelButton_AddForm->setObjectName("cancelButton_AddForm");
        cancelButton_AddForm->setMinimumSize(QSize(120, 36));
        cancelButton_AddForm->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #95a5a6; color: white; border: none; } QPushButton:hover { background-color: #7f8c8d; } QPushButton:pressed { background-color: #616A6B; }"));

        horizontalLayout_AddFormActions->addWidget(cancelButton_AddForm);

        label_27 = new QLabel(addClientFormContainer);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(40, 272, 84, 27));
        cinLineEdit = new QLineEdit(addClientFormContainer);
        cinLineEdit->setObjectName("cinLineEdit");
        cinLineEdit->setGeometry(QRect(139, 80, 281, 36));
        cinLineEdit->setMinimumSize(QSize(0, 36));
        label_4 = new QLabel(addClientFormContainer);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 80, 84, 27));
        paysComboBox = new QComboBox(addClientFormContainer);
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->addItem(QString());
        paysComboBox->setObjectName("paysComboBox");
        paysComboBox->setGeometry(QRect(139, 368, 281, 36));
        paysComboBox->setMinimumSize(QSize(0, 36));
        label_24 = new QLabel(addClientFormContainer);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(40, 224, 84, 27));
        emailLineEdit = new QLineEdit(addClientFormContainer);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setGeometry(QRect(139, 272, 281, 36));
        emailLineEdit->setMinimumSize(QSize(0, 36));
        prenomLineEdit = new QLineEdit(addClientFormContainer);
        prenomLineEdit->setObjectName("prenomLineEdit");
        prenomLineEdit->setGeometry(QRect(139, 176, 281, 36));
        prenomLineEdit->setMinimumSize(QSize(0, 36));
        adresseLineEdit = new QLineEdit(addClientFormContainer);
        adresseLineEdit->setObjectName("adresseLineEdit");
        adresseLineEdit->setGeometry(QRect(139, 224, 281, 36));
        adresseLineEdit->setMinimumSize(QSize(0, 36));
        label_3 = new QLabel(addClientFormContainer);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 128, 84, 27));
        telephoneLineEdit = new QLineEdit(addClientFormContainer);
        telephoneLineEdit->setObjectName("telephoneLineEdit");
        telephoneLineEdit->setGeometry(QRect(139, 320, 281, 36));
        telephoneLineEdit->setMinimumSize(QSize(0, 36));
        nomLineEdit = new QLineEdit(addClientFormContainer);
        nomLineEdit->setObjectName("nomLineEdit");
        nomLineEdit->setGeometry(QRect(139, 128, 281, 36));
        nomLineEdit->setMinimumSize(QSize(0, 36));
        label_Zone = new QLabel(addClientFormContainer);
        label_Zone->setObjectName("label_Zone");
        label_Zone->setGeometry(QRect(40, 416, 84, 27));
        label = new QLabel(addClientFormContainer);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 368, 84, 27));
        label_8 = new QLabel(addClientFormContainer);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 176, 84, 27));
        label_25 = new QLabel(addClientFormContainer);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(40, 320, 84, 27));
        zoneComboBox = new QComboBox(addClientFormContainer);
        zoneComboBox->addItem(QString());
        zoneComboBox->addItem(QString());
        zoneComboBox->addItem(QString());
        zoneComboBox->addItem(QString());
        zoneComboBox->addItem(QString());
        zoneComboBox->setObjectName("zoneComboBox");
        zoneComboBox->setGeometry(QRect(140, 410, 281, 36));
        zoneComboBox->setMinimumSize(QSize(0, 36));
        label_AddClientTitle = new QLabel(addClientPage);
        label_AddClientTitle->setObjectName("label_AddClientTitle");
        label_AddClientTitle->setGeometry(QRect(0, 0, 341, 47));
        label_AddClientTitle->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; color: #3498db; padding-bottom: 15px;"));
        label_AddClientTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_StatsTitle_2 = new QLabel(addClientPage);
        label_StatsTitle_2->setObjectName("label_StatsTitle_2");
        label_StatsTitle_2->setGeometry(QRect(30, -10, 935, 37));
        label_StatsTitle_2->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: 600; color: #2980b9; padding-bottom: 5px;"));
        stackedWidget->addWidget(addClientPage);
        statisticsPage = new QWidget();
        statisticsPage->setObjectName("statisticsPage");
        verticalLayout_Stats = new QVBoxLayout(statisticsPage);
        verticalLayout_Stats->setSpacing(15);
        verticalLayout_Stats->setObjectName("verticalLayout_Stats");
        label_StatsTitle = new QLabel(statisticsPage);
        label_StatsTitle->setObjectName("label_StatsTitle");
        label_StatsTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: 600; color: #2980b9; padding-bottom: 5px;"));

        verticalLayout_Stats->addWidget(label_StatsTitle);

        statsControlWidget = new QWidget(statisticsPage);
        statsControlWidget->setObjectName("statsControlWidget");
        statsControlWidget->setStyleSheet(QString::fromUtf8("#statsControlWidget { background-color: #f8f9fa; border: 1px solid #e9ecef; border-radius: 5px; padding: 8px; }\n"
"#statsControlWidget QLabel { font-weight: bold; color: #555; font-size: 13px; }\n"
"/* ComboBox style inherited */\n"
""));
        horizontalLayout_StatsControls = new QHBoxLayout(statsControlWidget);
        horizontalLayout_StatsControls->setSpacing(10);
        horizontalLayout_StatsControls->setObjectName("horizontalLayout_StatsControls");
        horizontalLayout_StatsControls->setContentsMargins(5, 5, 5, 5);
        label_TimeRange = new QLabel(statsControlWidget);
        label_TimeRange->setObjectName("label_TimeRange");

        horizontalLayout_StatsControls->addWidget(label_TimeRange);

        comboBoxchoisi = new QComboBox(statsControlWidget);
        comboBoxchoisi->addItem(QString());
        comboBoxchoisi->addItem(QString());
        comboBoxchoisi->addItem(QString());
        comboBoxchoisi->addItem(QString());
        comboBoxchoisi->setObjectName("comboBoxchoisi");
        comboBoxchoisi->setMinimumSize(QSize(160, 36));
        comboBoxchoisi->setStyleSheet(QString::fromUtf8("/* Using general ComboBox style */"));

        horizontalLayout_StatsControls->addWidget(comboBoxchoisi);

        horizontalSpacer_StatsControls = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_StatsControls->addItem(horizontalSpacer_StatsControls);


        verticalLayout_Stats->addWidget(statsControlWidget);

        chartFrame = new QFrame(statisticsPage);
        chartFrame->setObjectName("chartFrame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(chartFrame->sizePolicy().hasHeightForWidth());
        chartFrame->setSizePolicy(sizePolicy);
        chartFrame->setMinimumSize(QSize(0, 300));
        chartFrame->setStyleSheet(QString::fromUtf8("#chartFrame {\n"
"    border: 1px solid #dee2e6; background-color: #fdfefe; border-radius: 4px;\n"
"}\n"
"QLabel#chartInfoLabel { /* Target specifically if needed */\n"
"   color: #6c757d; font-size: 14px; font-style: italic;\n"
"}\n"
""));
        chartFrame->setFrameShape(QFrame::Shape::StyledPanel);
        chartFrame->setFrameShadow(QFrame::Shadow::Plain);
        verticalLayout_ChartContainer = new QVBoxLayout(chartFrame);
        verticalLayout_ChartContainer->setObjectName("verticalLayout_ChartContainer");
        verticalLayout_ChartContainer->setContentsMargins(5, 5, 5, 5);
        chartInfoLabel = new QLabel(chartFrame);
        chartInfoLabel->setObjectName("chartInfoLabel");
        sizePolicy.setHeightForWidth(chartInfoLabel->sizePolicy().hasHeightForWidth());
        chartInfoLabel->setSizePolicy(sizePolicy);
        chartInfoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_ChartContainer->addWidget(chartInfoLabel);


        verticalLayout_Stats->addWidget(chartFrame);

        verticalLayout_Stats->setStretch(2, 1);
        stackedWidget->addWidget(statisticsPage);
        riskManagementPage = new QWidget();
        riskManagementPage->setObjectName("riskManagementPage");
        verticalLayout_RiskMgmt = new QVBoxLayout(riskManagementPage);
        verticalLayout_RiskMgmt->setSpacing(15);
        verticalLayout_RiskMgmt->setObjectName("verticalLayout_RiskMgmt");
        label_RiskMgmtTitle = new QLabel(riskManagementPage);
        label_RiskMgmtTitle->setObjectName("label_RiskMgmtTitle");
        label_RiskMgmtTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: 600; color: #e67e22; /* Orange for risk? */ padding-bottom: 5px;"));

        verticalLayout_RiskMgmt->addWidget(label_RiskMgmtTitle);

        riskInputContainer = new QWidget(riskManagementPage);
        riskInputContainer->setObjectName("riskInputContainer");
        riskInputContainer->setStyleSheet(QString::fromUtf8("#riskInputContainer { background-color: #fdf6e3; border: 1px solid #f9e79f; border-radius: 5px; padding: 10px; }\n"
"#riskInputContainer QLabel { font-weight: bold; color: #555; }\n"
"/* Button/Input styles inherited or overridden below */\n"
""));
        horizontalLayout_RiskInput = new QHBoxLayout(riskInputContainer);
        horizontalLayout_RiskInput->setSpacing(10);
        horizontalLayout_RiskInput->setObjectName("horizontalLayout_RiskInput");
        label_RiskType = new QLabel(riskInputContainer);
        label_RiskType->setObjectName("label_RiskType");

        horizontalLayout_RiskInput->addWidget(label_RiskType);

        comboBox_RiskType = new QComboBox(riskInputContainer);
        comboBox_RiskType->addItem(QString());
        comboBox_RiskType->addItem(QString());
        comboBox_RiskType->addItem(QString());
        comboBox_RiskType->addItem(QString());
        comboBox_RiskType->setObjectName("comboBox_RiskType");
        comboBox_RiskType->setMinimumSize(QSize(180, 36));

        horizontalLayout_RiskInput->addWidget(comboBox_RiskType);

        label_RiskZone = new QLabel(riskInputContainer);
        label_RiskZone->setObjectName("label_RiskZone");

        horizontalLayout_RiskInput->addWidget(label_RiskZone);

        comboBox_RiskZone = new QComboBox(riskInputContainer);
        comboBox_RiskZone->addItem(QString());
        comboBox_RiskZone->addItem(QString());
        comboBox_RiskZone->addItem(QString());
        comboBox_RiskZone->addItem(QString());
        comboBox_RiskZone->setObjectName("comboBox_RiskZone");
        comboBox_RiskZone->setMinimumSize(QSize(180, 36));

        horizontalLayout_RiskInput->addWidget(comboBox_RiskZone);

        label_RiskDate = new QLabel(riskInputContainer);
        label_RiskDate->setObjectName("label_RiskDate");

        horizontalLayout_RiskInput->addWidget(label_RiskDate);

        dateEdit_RiskDate = new QDateEdit(riskInputContainer);
        dateEdit_RiskDate->setObjectName("dateEdit_RiskDate");
        dateEdit_RiskDate->setMinimumSize(QSize(120, 36));
        dateEdit_RiskDate->setCalendarPopup(true);
        dateEdit_RiskDate->setDate(QDate(2024, 1, 1));

        horizontalLayout_RiskInput->addWidget(dateEdit_RiskDate);

        horizontalSpacer_RiskInput = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_RiskInput->addItem(horizontalSpacer_RiskInput);

        button_ReportRisk = new QPushButton(riskInputContainer);
        button_ReportRisk->setObjectName("button_ReportRisk");
        button_ReportRisk->setMinimumSize(QSize(120, 36));
        button_ReportRisk->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #e74c3c; color: white; border: none; } QPushButton:hover { background-color: #c0392b; } QPushButton:pressed { background-color: #a93226; } QPushButton:disabled { background-color: #f5b7b1; }"));
        button_ReportRisk->setIconSize(QSize(16, 16));

        horizontalLayout_RiskInput->addWidget(button_ReportRisk);


        verticalLayout_RiskMgmt->addWidget(riskInputContainer);

        tableWidget_RiskClients = new QTableWidget(riskManagementPage);
        if (tableWidget_RiskClients->columnCount() < 4)
            tableWidget_RiskClients->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_RiskClients->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_RiskClients->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_RiskClients->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_RiskClients->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        tableWidget_RiskClients->setObjectName("tableWidget_RiskClients");
        tableWidget_RiskClients->setStyleSheet(QString::fromUtf8("/* Reusing clientTableWidget style */\n"
"QTableWidget {\n"
"    border: 1px solid #E0E0E0; gridline-color: #F0F0F0; background-color: #FFFFFF;\n"
"    alternate-background-color: #F9F9F9; selection-background-color: #AED6F1; selection-color: #222222;\n"
"    outline: none; font-size: 13px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 8px 12px; border-bottom: 1px solid #F0F0F0; border-right: 1px solid #F0F0F0;\n"
"}\n"
"QTableWidget::item:last-child { border-right: none; }\n"
"/* Specific selection style for risk table if needed */\n"
"/* QTableWidget::item:selected { background-color: #FADBD8; color: #78281F; } */\n"
"QTableWidget::item:hover { background-color: #EAF2F8; }\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #F1F3F4; padding: 8px; border: none; border-bottom: 2px solid #D5D8DC;\n"
"    font-weight: bold; color: #424949; text-align: left; padding-left: 12px; min-height: 26px;\n"
"}\n"
"/* Placeholder styles for status (will be set dynamically in code) */\n"
"/* QTableWidget::item[statu"
                        "s=\"danger\"] { background-color: #F5B7B1; color: #78281F; } */\n"
"/* QTableWidget::item[status=\"ok\"] { background-color: #A9DFBF; color: #145A32; } */\n"
""));
        tableWidget_RiskClients->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableWidget_RiskClients->setAlternatingRowColors(true);
        tableWidget_RiskClients->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tableWidget_RiskClients->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableWidget_RiskClients->setShowGrid(true);
        tableWidget_RiskClients->setGridStyle(Qt::PenStyle::SolidLine);
        tableWidget_RiskClients->setSortingEnabled(false);
        tableWidget_RiskClients->horizontalHeader()->setStretchLastSection(false);
        tableWidget_RiskClients->verticalHeader()->setVisible(false);

        verticalLayout_RiskMgmt->addWidget(tableWidget_RiskClients);

        verticalLayout_RiskMgmt->setStretch(2, 1);
        stackedWidget->addWidget(riskManagementPage);
        notificationsPage = new QWidget();
        notificationsPage->setObjectName("notificationsPage");
        verticalLayout_Notifications = new QVBoxLayout(notificationsPage);
        verticalLayout_Notifications->setSpacing(15);
        verticalLayout_Notifications->setObjectName("verticalLayout_Notifications");
        label_NotificationsTitle = new QLabel(notificationsPage);
        label_NotificationsTitle->setObjectName("label_NotificationsTitle");
        label_NotificationsTitle->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: 600; color: #3498db; /* Blue like others */ padding-bottom: 5px;"));

        verticalLayout_Notifications->addWidget(label_NotificationsTitle);

        notificationsListWidget = new QListWidget(notificationsPage);
        notificationsListWidget->setObjectName("notificationsListWidget");
        notificationsListWidget->setStyleSheet(QString::fromUtf8("QListWidget {\n"
"               border: 1px solid #E0E0E0;\n"
"               background-color: #FFFFFF;\n"
"               alternate-background-color: #F9F9F9;\n"
"               outline: none;\n"
"             }\n"
"             QListWidget::item {\n"
"                 padding: 8px 12px;\n"
"                 border-bottom: 1px solid #F0F0F0; /* Separator between items */\n"
"                 min-height: 25px; /* Ensure items aren't too small */\n"
"                 color: #444; /* Default text color */\n"
"             }\n"
"             /* Remove default selection highlight if needed */\n"
"             QListWidget::item:selected {\n"
"                 background-color: #EAF2F8; /* Light blue selection */\n"
"                 color: #1A5276;\n"
"                 /* border-bottom-color: #9ACEEB; */ /* Optional: Change border on selection */\n"
"             }\n"
"             QListWidget::item:hover {\n"
"                 background-color: #F5F5F5; /* Subtle hover */\n"
"             }\n"
"             /* E"
                        "xample for different notification types (set text color in code) */\n"
"             /* QListWidget::item[type=\"error\"] { color: red; } */\n"
"             /* QListWidget::item[type=\"warning\"] { color: orange; } */\n"
"             /* QListWidget::item[type=\"info\"] { color: blue; } */\n"
"             "));
        notificationsListWidget->setAlternatingRowColors(true);

        verticalLayout_Notifications->addWidget(notificationsListWidget);

        horizontalLayout_NotificationActions = new QHBoxLayout();
        horizontalLayout_NotificationActions->setObjectName("horizontalLayout_NotificationActions");
        horizontalSpacer_Notifications = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_NotificationActions->addItem(horizontalSpacer_Notifications);

        button_ClearNotifications = new QPushButton(notificationsPage);
        button_ClearNotifications->setObjectName("button_ClearNotifications");
        button_ClearNotifications->setMinimumSize(QSize(120, 36));
        button_ClearNotifications->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #95a5a6; color: white; border: none; } QPushButton:hover { background-color: #7f8c8d; } QPushButton:pressed { background-color: #616A6B; }"));

        horizontalLayout_NotificationActions->addWidget(button_ClearNotifications);


        verticalLayout_Notifications->addLayout(horizontalLayout_NotificationActions);

        verticalLayout_Notifications->setStretch(1, 1);
        stackedWidget->addWidget(notificationsPage);

        verticalLayout_MainContent->addWidget(stackedWidget);


        retranslateUi(Widget);

        sidebarListWidget->setCurrentRow(-1);
        stackedWidget->setCurrentIndex(4);
        ajouterButton_AddForm->setDefault(true);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Gestionnaire Clients", nullptr));
        actionExport->setText(QCoreApplication::translate("Widget", "Exporter CSV...", nullptr));
#if QT_CONFIG(tooltip)
        actionExport->setToolTip(QCoreApplication::translate("Widget", "Exporter la liste des clients au format CSV", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExportPDF->setText(QCoreApplication::translate("Widget", "Exporter PDF...", nullptr));
#if QT_CONFIG(tooltip)
        actionExportPDF->setToolTip(QCoreApplication::translate("Widget", "Exporter la liste des clients au format PDF", nullptr));
#endif // QT_CONFIG(tooltip)
        actionPrint->setText(QCoreApplication::translate("Widget", "Imprimer la liste...", nullptr));
#if QT_CONFIG(tooltip)
        actionPrint->setToolTip(QCoreApplication::translate("Widget", "Imprimer la table des clients actuelle", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSettings->setText(QCoreApplication::translate("Widget", "Param\303\250tres...", nullptr));
#if QT_CONFIG(tooltip)
        actionSettings->setToolTip(QCoreApplication::translate("Widget", "Ouvrir les param\303\250tres de l'application", nullptr));
#endif // QT_CONFIG(tooltip)

        const bool __sortingEnabled = sidebarListWidget->isSortingEnabled();
        sidebarListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = sidebarListWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("Widget", "\360\237\223\212 Ajout", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = sidebarListWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("Widget", "\360\237\221\245 Liste Clients", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = sidebarListWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("Widget", "\360\237\223\210 Statistiques", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = sidebarListWidget->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("Widget", "\342\232\240\357\270\217 Gestion Risques", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = sidebarListWidget->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("Widget", "\360\237\224\224 Notifications", nullptr));
        sidebarListWidget->setSortingEnabled(__sortingEnabled);

        label_28->setText(QCoreApplication::translate("Widget", "Gestion de Clients", nullptr));
        label_23->setText(QString());
        label_WelcomeTitle->setText(QCoreApplication::translate("Widget", "\360\237\233\215\357\270\217\360\237\221\213 Bienvenue au Gestionnaire Clients MallMate", nullptr));
        label_WelcomeDesc->setText(QCoreApplication::translate("Widget", "S\303\251lectionnez une option dans le menu de gauche pour commencer.", nullptr));
        QTableWidgetItem *___qtablewidgetitem = clientTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Widget", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = clientTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Widget", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = clientTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Widget", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = clientTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Widget", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = clientTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Widget", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = clientTableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Widget", "T\303\251l\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = clientTableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Widget", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = clientTableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Widget", "Pays", nullptr));
        addButton_List->setText(QCoreApplication::translate("Widget", "\342\236\225 Ajouter", nullptr));
        deleteButton->setText(QCoreApplication::translate("Widget", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        modifyButton->setText(QCoreApplication::translate("Widget", "\342\234\217\357\270\217 Modifier", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Rechercher un client...", nullptr));
        comboBox_SortBy->setItemText(0, QCoreApplication::translate("Widget", "Recherche par: Nom", nullptr));
        comboBox_SortBy->setItemText(1, QCoreApplication::translate("Widget", "Recherche par: Pr\303\251nom", nullptr));
        comboBox_SortBy->setItemText(2, QCoreApplication::translate("Widget", "Recherche par: CIN", nullptr));
        comboBox_SortBy->setItemText(3, QCoreApplication::translate("Widget", "Recherche par: Zone", nullptr));

#if QT_CONFIG(tooltip)
        functionalityMenuButton->setToolTip(QCoreApplication::translate("Widget", "Fonctionnalit\303\251s", nullptr));
#endif // QT_CONFIG(tooltip)
        functionalityMenuButton->setText(QCoreApplication::translate("Widget", "...", nullptr));
        consulterbtn->setText(QCoreApplication::translate("Widget", "\360\237\227\202\357\270\217Consulter", nullptr));
        label_WelcomeDesc_2->setText(QCoreApplication::translate("Widget", " Veuillez remplir les informations du  client avec pr\303\251cision.", nullptr));
        ajouterButton_AddForm->setText(QCoreApplication::translate("Widget", "\342\234\224\357\270\217 Enregistrer Client", nullptr));
        cancelButton_AddForm->setText(QCoreApplication::translate("Widget", "Annuler", nullptr));
        label_27->setText(QCoreApplication::translate("Widget", "Email :", nullptr));
        cinLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Num\303\251ro CIN", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "CIN :", nullptr));
        paysComboBox->setItemText(0, QCoreApplication::translate("Widget", "Tunisie", nullptr));
        paysComboBox->setItemText(1, QCoreApplication::translate("Widget", "Palestine", nullptr));
        paysComboBox->setItemText(2, QCoreApplication::translate("Widget", "Inde", nullptr));
        paysComboBox->setItemText(3, QCoreApplication::translate("Widget", "Alg\303\251rie", nullptr));
        paysComboBox->setItemText(4, QCoreApplication::translate("Widget", "Espagne", nullptr));
        paysComboBox->setItemText(5, QCoreApplication::translate("Widget", "Maroc", nullptr));
        paysComboBox->setItemText(6, QCoreApplication::translate("Widget", "Allemagne", nullptr));
        paysComboBox->setItemText(7, QCoreApplication::translate("Widget", "Chine", nullptr));
        paysComboBox->setItemText(8, QCoreApplication::translate("Widget", "France", nullptr));

        label_24->setText(QCoreApplication::translate("Widget", "Adresse :", nullptr));
        emailLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "adresse@example.com", nullptr));
        prenomLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Pr\303\251nom", nullptr));
        adresseLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Adresse postale", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "Nom :", nullptr));
        telephoneLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Num\303\251ro de t\303\251l\303\251phone", nullptr));
        nomLineEdit->setPlaceholderText(QCoreApplication::translate("Widget", "Nom de famille", nullptr));
        label_Zone->setText(QCoreApplication::translate("Widget", "Zone :", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Pays :", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "Pr\303\251nom :", nullptr));
        label_25->setText(QCoreApplication::translate("Widget", "T\303\251l\303\251phone :", nullptr));
        zoneComboBox->setItemText(0, QString());
        zoneComboBox->setItemText(1, QCoreApplication::translate("Widget", "zone A", nullptr));
        zoneComboBox->setItemText(2, QCoreApplication::translate("Widget", "zone B", nullptr));
        zoneComboBox->setItemText(3, QCoreApplication::translate("Widget", "zone C", nullptr));
        zoneComboBox->setItemText(4, QCoreApplication::translate("Widget", "zone D", nullptr));

        label_AddClientTitle->setText(QString());
        label_StatsTitle_2->setText(QCoreApplication::translate("Widget", "\360\237\223\213 Ajouter un Nouveau Client", nullptr));
        label_StatsTitle->setText(QCoreApplication::translate("Widget", "\360\237\223\212 Vue Chronologique des Inscriptions", nullptr));
        label_TimeRange->setText(QCoreApplication::translate("Widget", "P\303\251riode :", nullptr));
        comboBoxchoisi->setItemText(0, QCoreApplication::translate("Widget", "All", nullptr));
        comboBoxchoisi->setItemText(1, QCoreApplication::translate("Widget", "Asie", nullptr));
        comboBoxchoisi->setItemText(2, QCoreApplication::translate("Widget", "Afrique", nullptr));
        comboBoxchoisi->setItemText(3, QCoreApplication::translate("Widget", "Europe", nullptr));

        chartInfoLabel->setText(QCoreApplication::translate("Widget", "(Le graphique chronologique sera affich\303\251 ici)", nullptr));
        label_RiskMgmtTitle->setText(QCoreApplication::translate("Widget", "\342\232\240\357\270\217 Gestion des Risques", nullptr));
        label_RiskType->setText(QCoreApplication::translate("Widget", "Type de risque :", nullptr));
        comboBox_RiskType->setItemText(0, QCoreApplication::translate("Widget", "\360\237\224\245 Incendie", nullptr));
        comboBox_RiskType->setItemText(1, QCoreApplication::translate("Widget", "\342\232\241 Panne de courant", nullptr));
        comboBox_RiskType->setItemText(2, QCoreApplication::translate("Widget", "\360\237\232\252 Foule", nullptr));
        comboBox_RiskType->setItemText(3, QCoreApplication::translate("Widget", "\360\237\230\250 Malaise", nullptr));

        label_RiskZone->setText(QCoreApplication::translate("Widget", "Zone concern\303\251e :", nullptr));
        comboBox_RiskZone->setItemText(0, QCoreApplication::translate("Widget", "zone A", nullptr));
        comboBox_RiskZone->setItemText(1, QCoreApplication::translate("Widget", "zone B", nullptr));
        comboBox_RiskZone->setItemText(2, QCoreApplication::translate("Widget", "zone C", nullptr));
        comboBox_RiskZone->setItemText(3, QCoreApplication::translate("Widget", "zone D", nullptr));

        label_RiskDate->setText(QCoreApplication::translate("Widget", "Date :", nullptr));
#if QT_CONFIG(tooltip)
        dateEdit_RiskDate->setToolTip(QCoreApplication::translate("Widget", "Date \303\240 laquelle le risque est signal\303\251 ou pertinent", nullptr));
#endif // QT_CONFIG(tooltip)
        dateEdit_RiskDate->setDisplayFormat(QCoreApplication::translate("Widget", "dd/MM/yyyy", nullptr));
#if QT_CONFIG(tooltip)
        button_ReportRisk->setToolTip(QCoreApplication::translate("Widget", "Signaler le risque et afficher les clients concern\303\251s", nullptr));
#endif // QT_CONFIG(tooltip)
        button_ReportRisk->setText(QCoreApplication::translate("Widget", "\360\237\232\250 Signaler", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_RiskClients->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("Widget", "Client", nullptr));
#if QT_CONFIG(tooltip)
        ___qtablewidgetitem8->setToolTip(QCoreApplication::translate("Widget", "Nom et pr\303\251nom du client", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_RiskClients->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("Widget", "Zone", nullptr));
#if QT_CONFIG(tooltip)
        ___qtablewidgetitem9->setToolTip(QCoreApplication::translate("Widget", "Zone enregistr\303\251e pour le client", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_RiskClients->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("Widget", "Statut", nullptr));
#if QT_CONFIG(tooltip)
        ___qtablewidgetitem10->setToolTip(QCoreApplication::translate("Widget", "Indique si le client est dans la zone \303\240 risque", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_RiskClients->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("Widget", "Action", nullptr));
#if QT_CONFIG(tooltip)
        ___qtablewidgetitem11->setToolTip(QCoreApplication::translate("Widget", "Actions possibles (ex: Notifier)", nullptr));
#endif // QT_CONFIG(tooltip)
        label_NotificationsTitle->setText(QCoreApplication::translate("Widget", "\360\237\224\224 Notifications R\303\251centes", nullptr));
#if QT_CONFIG(tooltip)
        button_ClearNotifications->setToolTip(QCoreApplication::translate("Widget", "Effacer toutes les notifications", nullptr));
#endif // QT_CONFIG(tooltip)
        button_ClearNotifications->setText(QCoreApplication::translate("Widget", "\360\237\227\221\357\270\217 Effacer Tout", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
