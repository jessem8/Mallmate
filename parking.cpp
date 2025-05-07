#include "parking.h"
#include <QFont>
#include <QSpacerItem>
#include <QPixmap>
#include <QLabel>
#include "connection.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QFormLayout>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // Pour gérer les NULL potentiels
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QPageSize>    // Needed for QPageSize::A4
#include <QPageLayout>  // Needed for QPageLayout::Point
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QButtonGroup>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QSqlError>
#include <QVariant>  // Pour gérer NULL si besoin plus tard
#include <QScrollArea>
#include <QFrame>
#include <QSpacerItem>
#include <QGridLayout> // Need this for the grid layout
#include <QMap>       // Useful for mapping spot number to status
#include <QRegularExpression> // For potentially parsing spot numbers
#include <QJsonParseError>
#include <QJsonArray>
#include <QRandomGenerator>


const int ACTIONS_COLUMN_INDEX = 5; //  0:PLACEID, 1:NUMERO, 2:STATUT, 3:LOCALISATION, 4:DATE RESERVATION, 5:Actions
void ParkingWindow::goToHomePage() {
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(0);
    }
}
ParkingWindow::ParkingWindow(QWidget *parent)
    : QWidget(parent), arduino_is_available(false)
{
    arduino = new QSerialPort(this);
    stackedWidget = new QStackedWidget(this);
    Connection dbConnection; // Crée un objet de votre classe Connection
    bool connectionSuccess = dbConnection.createconnect(); // Appelle votre fonction de connexion
    readSerialData();
    if (!connectionSuccess) {
        // La connexion a échoué - Affiche un message d'erreur critique
        qCritical() << "ERREUR CRITIQUE: Impossible de se connecter à la base de données 'Mallmate' via la classe Connection.";
        QMessageBox::critical(this,
                              "Erreur de Connexion Base de Données",
                              "Impossible de se connecter à la base de données.\n"
                              "Veuillez vérifier la configuration de la source de données ODBC nommée 'Mallmate'.\n"
                              "L'application risque de ne pas fonctionner correctement.");
        // Optionnel: Vous pourriez vouloir désactiver les parties de l'UI
        // qui dépendent de la base de données ici.
    } else {
        // La connexion a réussi
        qDebug() << "Base de données 'Mallmate' connectée avec succès via la classe Connection!";
    }
    setupSerialConnection();

    // ===========================================
    // =========      PAGE PRINCIPALE      =========
    // ===========================================
    QWidget *mainPage = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);

    // --- Sidebar (Menu latéral gauche) ---
    QVBoxLayout *sidebarLayout = new QVBoxLayout;
    QPushButton *dashboardButton = new QPushButton("🏠 Accueil");
    QPushButton *statsButton = new QPushButton("📈 Statistiques");
    QPushButton *historyButton2 = new QPushButton("📚 Historique");
    QPushButton *settingsButton = new QPushButton("⚙ Paramètres");

    connect(dashboardButton, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentIndex(0); });
    connect(statsButton, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentIndex(1); });
    connect(historyButton2, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentIndex(2); });
    connect(settingsButton, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentIndex(3); });

    QString sidebarStyle = R"(
    QPushButton {
        text-align: left;
        padding-left: 20px;
        background-color: transparent;
        border: none;
        color: #444444;
        min-height: 35px;
        font-size: 11pt;
    }
    QPushButton:hover {
        background-color: #f0f0f0;
        color: #111111;
    }
    QPushButton:pressed {
        background-color: #e0e0e0;
    }
    QPushButton:checked {
        background-color: #E3F2FD;
        color: #0D47A1;
        border-left: 3px solid #0D47A1;
        font-weight: bold;
        padding-left: 17px;
    }
)";

    dashboardButton->setStyleSheet(sidebarStyle);
    statsButton->setStyleSheet(sidebarStyle);
    historyButton2->setStyleSheet(sidebarStyle);
    settingsButton->setStyleSheet(sidebarStyle);

    // Pour donner un style de sélection similaire à QListWidget
    dashboardButton->setCheckable(true);
    statsButton->setCheckable(true);
    historyButton2->setCheckable(true);
    settingsButton->setCheckable(true);

    // Connecter les boutons pour désélectionner les autres
    auto buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(dashboardButton, 0);
    buttonGroup->addButton(statsButton, 1);
    buttonGroup->addButton(historyButton2, 2);
    buttonGroup->addButton(settingsButton, 3);
    buttonGroup->setExclusive(true);

    sidebarLayout->addWidget(dashboardButton);
    sidebarLayout->addWidget(statsButton);
    sidebarLayout->addWidget(historyButton2);
    sidebarLayout->addWidget(settingsButton);
    sidebarLayout->addStretch();

    // Style du conteneur de la sidebar
    sidebarLayout->setContentsMargins(0, 10, 0, 10);
    sidebarLayout->setSpacing(0);

    // --- Header (En-tête en haut) ---
    QHBoxLayout *headerLayout = new QHBoxLayout;
    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap("C:/Users/betbo/Desktop/intqt/parking/logo.png");
    if (logoPixmap.isNull()) {
        qWarning() << "Impossible de charger le logo";
        logoLabel->setText("Logo");
    } else {
        logoLabel->setPixmap(logoPixmap.scaled(100, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    titleLabel = new QLabel("Gestion du Parking");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #1565C0;");
    titleLabel->setAlignment(Qt::AlignCenter);

    notificationButton = new QPushButton();

    QIcon bellIcon("C:/path/to_your_icons/bell.png");
    if (bellIcon.isNull()) {
        notificationButton->setText("🔔");
        notificationButton->setMinimumWidth(40);
        notificationButton->setStyleSheet("QPushButton { font-size: 18px; border: none; background: transparent; padding: 5px; } QPushButton:hover { background-color: #E0E0E0; border-radius: 15px; }");
    } else {
        notificationButton->setIcon(bellIcon);
        notificationButton->setIconSize(QSize(24, 24));
        notificationButton->setFixedSize(40, 40);
        notificationButton->setStyleSheet(R"(
            QPushButton { background-color: transparent; border: none; border-radius: 20px; padding: 5px; }
            QPushButton:hover { background-color: #E0E0E0; }
            QPushButton:pressed { background-color: #D0D0D0; }
        )");
    }
    notificationButton->setToolTip("Afficher les notifications");
    notificationButton->setCursor(Qt::PointingHandCursor);


    connect(notificationButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Notifications", "Aucune nouvelle alerte pour le moment.");
    });
    // Initialiser le système de notifications
    setupNotificationSystem();

    // Connecter le bouton de notification
    connect(notificationButton, &QPushButton::clicked, this, [this]() {
        // Afficher les dernières notifications
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = 'occupee'");
        if (query.exec() && query.next()) {
            int occupied = query.value(0).toInt();
            int total = 8;

            QMessageBox::information(this, "État du Parking",
                                     QString("Places occupeees: %1\nPlaces libres: %2")
                                         .arg(occupied).arg(total - occupied));
        }
    });


    headerLayout->addWidget(logoLabel);
    headerLayout->addStretch(1);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(1);
    headerLayout->addWidget(notificationButton);

    mainLayout->addLayout(headerLayout);

    // --- Formulaire d'ajout ---
    QGroupBox *formBox = new QGroupBox("Ajouter une place");
    formBox->setStyleSheet(R"(
    QGroupBox {
        font-weight: bold; margin-top: 10px; border: 1px solid #D3D3D3;
        border-radius: 5px; padding-top: 18px; padding-bottom: 10px;
        padding-left: 10px; padding-right: 10px;
    }
    QGroupBox::title {
        subcontrol-origin: margin; subcontrol-position: top left; padding-left: 8px;
        padding-right: 8px; padding-top: 1px; padding-bottom: 1px;
    }
    )");
    formBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    QVBoxLayout *formLayout = new QVBoxLayout(formBox);
    formLayout->setSpacing(15);

    plateInput = new QLineEdit();
    plateInput->setPlaceholderText("Numéro de place (ex: A12)");
    nomInput = new QLineEdit();
    nomInput->setPlaceholderText("ID Réservation Utilisateur");
    prenomInput = new QLineEdit();
    prenomInput->setPlaceholderText("Localisation (ex: Niveau -1)");
    numeroInput = new QLineEdit();
    numeroInput->setPlaceholderText("ID Place (numérique)");
    entryTimeInput = new QLineEdit();
    entryTimeInput->setPlaceholderText("Date et heure réservation (JJ/MM/AAAA HH:MM)");
    vehicleTypeCombo = new QComboBox();
    vehicleTypeCombo->addItems({"libre", "reservee", "occupee"});
    addButton = new QPushButton("Ajouter");
    addButton->setFixedSize(100, 35);
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setStyleSheet(R"(
        QPushButton { background-color: #5dade2; color: white; font-size: 13px; border: none; border-radius: 6px; padding: 5px; }
        QPushButton:hover { background-color: #3; }
        QPushButton:pressed { background-color: #555555; }
    )");
    connect(addButton, &QPushButton::clicked, this, &ParkingWindow::onAddButtonClicked);
    formLayout->addWidget(plateInput);
    formLayout->addWidget(nomInput);
    formLayout->addWidget(prenomInput);
    formLayout->addWidget(numeroInput);
    formLayout->addWidget(entryTimeInput);
    formLayout->addWidget(vehicleTypeCombo);
    formLayout->addWidget(addButton, 0, Qt::AlignRight);
    mainLayout->addWidget(formBox);

    // --- Boutons d'action ---
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QVBoxLayout *rightButtons = new QVBoxLayout;
    rightButtons->setSpacing(10);

    //removeButton = new QPushButton("🚗 Retirer un véhicule");
    //historyButton = new QPushButton("📊 Afficher historique");
    ticketButton = new QPushButton("🧾 Générer un ticket");

    QString actionButtonStyle = R"(
        QPushButton {
            background-color: white; color: black; font-weight: bold;
            padding: 10px 15px; border: 1px solid #A0A0A0; border-radius: 8px;
            min-width: 180px;
        }
        QPushButton:hover { background-color: #F0F0F0; border-color: #555; }
        QPushButton:pressed { background-color: #E0E0E0; }
    )";
    //removeButton->setStyleSheet(actionButtonStyle + "QPushButton { border-color: #C0392B; }");
    //historyButton->setStyleSheet(actionButtonStyle);
    ticketButton->setStyleSheet(actionButtonStyle);

    //removeButton->setCursor(Qt::PointingHandCursor);
    //historyButton->setCursor(Qt::PointingHandCursor);
    ticketButton->setCursor(Qt::PointingHandCursor);

    //connect(historyButton, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentIndex(2); });
    // Dans ParkingWindow::ParkingWindow()
    connect(ticketButton, &QPushButton::clicked, this, &ParkingWindow::generateParkingTicket);

    //rightButtons->addWidget(removeButton);
    //rightButtons->addWidget(historyButton);
    rightButtons->addWidget(ticketButton);
    rightButtons->addStretch();

    bottomLayout->addStretch();
    bottomLayout->addLayout(rightButtons);

    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();

    // =========================================
    // =========      PAGE STATS       =========
    // =========================================
    // Dans le constructeur, remplacer la partie statsPage par :
    // Dans le constructeur, remplacer la partie statsPage par :
    QWidget *statsPage = new QWidget;
    QVBoxLayout *statsPageLayout = new QVBoxLayout(statsPage);

    // --- Graphique ---
    QHBoxLayout *statsContentLayout = new QHBoxLayout();
    setupCharts(); // Initialise les graphiques

    // Configuration de la vue pour un meilleur affichage
    chartView->setMinimumSize(600, 500);
    statsContentLayout->addWidget(chartView, 1, Qt::AlignCenter);

    // --- Bouton Actualiser ---
    QHBoxLayout *statsButtonLayout = new QHBoxLayout();
    QPushButton *refreshStatsBtn = new QPushButton("Actualiser");
    refreshStatsBtn->setFixedHeight(40);
    refreshStatsBtn->setCursor(Qt::PointingHandCursor);
    refreshStatsBtn->setStyleSheet("QPushButton { padding: 10px 20px; background-color: #5dade2; color: #2980B9; }");
    connect(refreshStatsBtn, &QPushButton::clicked, this, &ParkingWindow::updateStats);
    QPushButton *backFromStatsBtn = new QPushButton("Retour");
    backFromStatsBtn->setFixedHeight(40);
    backFromStatsBtn->setCursor(Qt::PointingHandCursor);
    backFromStatsBtn->setStyleSheet("QPushButton { padding: 10px 20px; }");
    connect(backFromStatsBtn, &QPushButton::clicked, this, &ParkingWindow::goToHomePage);

    statsButtonLayout->addStretch();
    statsButtonLayout->addWidget(refreshStatsBtn);
    statsButtonLayout->addWidget(backFromStatsBtn);
    statsButtonLayout->addStretch();

    statsPageLayout->addLayout(statsContentLayout, 1);
    statsPageLayout->addLayout(statsButtonLayout);

    // ===========================================
    // =========    PAGE HISTORIQUE      =========
    // ===========================================
    QWidget *historyPage = new QWidget;
    QVBoxLayout *historyLayout = new QVBoxLayout(historyPage);
    historyLayout->setContentsMargins(20, 20, 20, 20);
    historyLayout->setSpacing(15);

    QLabel *historyTitle = new QLabel("📚 Historique des places");
    historyTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #333; margin-bottom: 10px;");
    historyTitle->setAlignment(Qt::AlignCenter);
    historyLayout->addWidget(historyTitle);
    historyTable = new QTableWidget();
    historyTable->setColumnCount(6);
    historyTable->setHorizontalHeaderLabels({"PLACEID", "NUMERO", "STATUT", "LOCALISATION", "Actions"});

    // Configuration des dimensions et du style
    historyTable->verticalHeader()->setDefaultSectionSize(40); // Hauteur fixe des lignes
    historyTable->verticalHeader()->setVisible(false);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    historyTable->setAlternatingRowColors(true);

    // Largeurs des colonnes (en pixels)
    historyTable->setColumnWidth(0, 70);   // PLACEID
    historyTable->setColumnWidth(1, 80);   // NUMERO
    historyTable->setColumnWidth(2, 90);   // STATUT
    historyTable->setColumnWidth(3, 90);  // LOCALISATION
    historyTable->setColumnWidth(4, 150);  // DATE RESERVATION
    historyTable->setColumnWidth(5, 200);  // Actions
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);

    QLabel *searchLabel = new QLabel("Rechercher:");
    searchLabel->setStyleSheet("font-weight: bold;");

    searchLineEdit = new QLineEdit();
    searchLineEdit->setPlaceholderText("PLACEID, Numéro, Statut ou Localisation...");
    searchLineEdit->setClearButtonEnabled(true);

    searchFieldCombo = new QComboBox();
    searchFieldCombo->addItem("Tous les champs", "all");
    searchFieldCombo->addItem("PLACEID", "PLACEID");
    searchFieldCombo->addItem("Numéro", "NUMERO");
    searchFieldCombo->addItem("Statut", "STATUT");
    searchFieldCombo->addItem("Localisation", "LOCALISATION");
    searchFieldCombo->addItem("Date Réservation", "DATE_RESERVATION");
    QPushButton *searchButton = new QPushButton("Rechercher");
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setStyleSheet("QPushButton { padding: 5px 15px; background-color: #5dade2; color: white; border: none; border-radius: 4px; }"
                                "QPushButton:hover { background-color: #2980B9; }");

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit, 1);
    searchLayout->addWidget(searchFieldCombo);
    searchLayout->addWidget(searchButton);

    historyLayout->addLayout(searchLayout);

    // Connecter les signaux de recherche
    connect(searchButton, &QPushButton::clicked, this, &ParkingWindow::filterHistory);
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &ParkingWindow::filterHistory);



    // --- Barre de tri ---
    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->setSpacing(10);

    QLabel *sortLabel = new QLabel("Trier par:");
    sortLabel->setStyleSheet("font-weight: bold;");

    sortComboBox = new QComboBox();
    sortComboBox->addItem("Date (récent)", "date_desc");
    sortComboBox->addItem("Date (ancien)", "date_asc");
    sortComboBox->addItem("Numéro de place", "numero");
    sortComboBox->addItem("Statut", "statut");
    sortComboBox->setCursor(Qt::PointingHandCursor);

    sortButton = new QPushButton("Appliquer");
    sortButton->setCursor(Qt::PointingHandCursor);
    sortButton->setStyleSheet("QPushButton { padding: 5px 15px; background-color: #5dade2; color: white; border: none; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #2980B9; }");

    sortLayout->addWidget(sortLabel);
    sortLayout->addWidget(sortComboBox);
    sortLayout->addWidget(sortButton);
    sortLayout->addStretch();

    historyLayout->addLayout(sortLayout);

    // Connecter le bouton de tri
    connect(sortButton, &QPushButton::clicked, this, [this]() {
        this->sortHistory(sortComboBox->currentData().toString());
    });
    // --- Bouton Export PDF ---
    QPushButton *exportPdfButton = new QPushButton("Exporter en PDF");
    connect(exportPdfButton, &QPushButton::clicked, this, &ParkingWindow::exportToPDF);

    exportPdfButton->setCursor(Qt::PointingHandCursor);
    exportPdfButton->setStyleSheet("QPushButton { padding: 5px 15px; background-color: #e74c3c; color: white; border: none; border-radius: 4px; }"
                                   "QPushButton:hover { background-color: #c0392b; }");
    // Dans ParkingWindow::ParkingWindow(), après la création des autres boutons
    QPushButton *parkingPlanButton = new QPushButton("🗺 Plan du parking");
    parkingPlanButton->setCursor(Qt::PointingHandCursor);
    parkingPlanButton->setStyleSheet(actionButtonStyle);
    connect(parkingPlanButton, &QPushButton::clicked, this, &ParkingWindow::showParkingSpotStatus);

    // Ajouter ce bouton à votre layout existant
    rightButtons->addWidget(parkingPlanButton);
    // Ajoutez le bouton au layout existant
    sortLayout->addWidget(exportPdfButton);
    // Style du tableau
    historyTable->setStyleSheet(R"(
QTableWidget {
    border: 1px solid #E0E0E0; gridline-color: #F0F0F0; background-color: #FFFFFF;
    alternate-background-color: #F9F9F9; selection-background-color: #AED6F1; selection-color: #222222;
    outline: none; font-size: 13px;
}
QTableWidget::item {
    padding: 8px 12px; border-bottom: 1px solid #F0F0F0; border-right: 1px solid #F0F0F0;
}
QTableWidget::item:last-child { border-right: none; }
QTableWidget::item:selected { background-color: #AED6F1; color: #1A5276; border-bottom: 1px solid #9ACEEB; border-right-color: #9ACEEB; }
QTableWidget::item:selected:last-child { border-right: none; }
QTableWidget::item:hover { background-color: #EAF2F8; }

QHeaderView::section {
    background-color: #F1F3F4; padding: 8px; border: none; border-bottom: 2px solid #D5D8DC;
    font-weight: bold; color: #424949; text-align: left; padding-left: 12px; min-height: 26px;
}
)");
    historyTable->horizontalHeader()->setSectionResizeMode(ACTIONS_COLUMN_INDEX, QHeaderView::Fixed);
    historyTable->setColumnWidth(ACTIONS_COLUMN_INDEX, 180);

    historyLayout->addWidget(historyTable, 1);
    afficherHistorique();
    // --- Bouton Retour ---
    QHBoxLayout *historyButtonLayout = new QHBoxLayout();
    QPushButton *backFromHistoryBtn = new QPushButton("Retour");
    backFromHistoryBtn->setFixedHeight(40);
    backFromHistoryBtn->setCursor(Qt::PointingHandCursor);
    backFromHistoryBtn->setStyleSheet("QPushButton { padding: 10px 20px; }");
    connect(backFromHistoryBtn, &QPushButton::clicked, this, &ParkingWindow::goToHomePage);
    historyButtonLayout->addStretch();
    historyButtonLayout->addWidget(backFromHistoryBtn);
    historyButtonLayout->addStretch();
    historyLayout->addLayout(historyButtonLayout);

    // ===========================================
    // =========    PAGE PARAMÈTRES      =========
    // ===========================================
    QWidget *settingsPage = new QWidget;
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setSpacing(15);
    settingsLayout->setContentsMargins(25, 25, 25, 25);

    QLabel *settingsTitle = new QLabel("⚙ Paramètres de l'application");
    settingsTitle->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 20px; color: #333;");
    settingsTitle->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(settingsTitle);

    QString settingsGroupBoxStyle = R"(
        QGroupBox {
            font-weight: bold; margin-top: 8px; border: 1px solid #D3D3D3;
            border-radius: 5px; padding-top: 18px; padding-bottom: 10px;
            padding-left: 10px; padding-right: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin; subcontrol-position: top left; padding-left: 8px;
            padding-right: 8px; padding-top: 1px; padding-bottom: 1px;
        }
    )";

    QGroupBox *notificationsBox = new QGroupBox("Notifications");
    notificationsBox->setStyleSheet(settingsGroupBoxStyle);
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsBox);
    QCheckBox *soundNotifCheck = new QCheckBox("Activer les notifications sonores à l'entrée/sortie");
    QCheckBox *fullNotifCheck = new QCheckBox("Notifier quand le parking est plein");
    soundNotifCheck->setCursor(Qt::PointingHandCursor);
    fullNotifCheck->setCursor(Qt::PointingHandCursor);
    notificationsLayout->addWidget(soundNotifCheck);
    notificationsLayout->addWidget(fullNotifCheck);
    settingsLayout->addWidget(notificationsBox);

    QGroupBox *appearanceBox = new QGroupBox("Apparence");
    appearanceBox->setStyleSheet(settingsGroupBoxStyle);
    QFormLayout *appearanceLayout = new QFormLayout(appearanceBox);
    QComboBox *themeCombo = new QComboBox();
    themeCombo->addItems({"Clair (Défaut)", "Sombre"});
    QComboBox *languageCombo = new QComboBox();
    languageCombo->addItems({"Français", "English", "Arabe"});
    themeCombo->setCursor(Qt::PointingHandCursor);
    languageCombo->setCursor(Qt::PointingHandCursor);
    appearanceLayout->addRow(new QLabel("Thème visuel:"), themeCombo);
    appearanceLayout->addRow(new QLabel("Langue:"), languageCombo);
    settingsLayout->addWidget(appearanceBox);

    QGroupBox *dataBox = new QGroupBox("Gestion des données");
    dataBox->setStyleSheet(settingsGroupBoxStyle);
    QVBoxLayout *dataLayout = new QVBoxLayout(dataBox);
    QPushButton *exportHistoryButton = new QPushButton("Exporter l'historique (CSV)");
    QPushButton *clearHistoryButton = new QPushButton("Effacer TOUT l'historique");
    connect(exportHistoryButton, &QPushButton::clicked, this, &ParkingWindow::exportHistoryToCSV);

    exportHistoryButton->setCursor(Qt::PointingHandCursor);
    clearHistoryButton->setCursor(Qt::PointingHandCursor);
    exportHistoryButton->setStyleSheet("QPushButton { background-color: #5dade2; color: white; padding: 8px 12px; border: none; border-radius: 4px; } QPushButton:hover { background-color: #2980B9; }");
    clearHistoryButton->setStyleSheet("QPushButton { background-color: #5dade2; color: white; padding: 8px 12px; border: none; border-radius: 4px; } QPushButton:hover { background-color: #2980B9; }");
    dataLayout->addWidget(exportHistoryButton);
    dataLayout->addWidget(clearHistoryButton);
    settingsLayout->addWidget(dataBox);

    settingsLayout->addStretch();

    QHBoxLayout *settingsActionsLayout = new QHBoxLayout();
    settingsActionsLayout->setSpacing(15);
    QPushButton *backFromSettingsBtn = new QPushButton("Retour");

    backFromSettingsBtn->setCursor(Qt::PointingHandCursor);
    backFromSettingsBtn->setFixedHeight(40);
    backFromSettingsBtn->setStyleSheet("QPushButton { padding: 10px 20px; }");

    connect(backFromSettingsBtn, &QPushButton::clicked, this, &ParkingWindow::goToHomePage);

    settingsActionsLayout->addStretch(1);
    settingsActionsLayout->addWidget(backFromSettingsBtn);
    settingsActionsLayout->addStretch(1);
    settingsLayout->addLayout(settingsActionsLayout);


    // ==================================================
    // ========= ASSEMBLAGE FINAL DES PAGES     =========
    // ==================================================

    stackedWidget->addWidget(mainPage);       // Index 0
    stackedWidget->addWidget(statsPage);      // Index 1
    stackedWidget->addWidget(historyPage);    // Index 2
    stackedWidget->addWidget(settingsPage);   // Index 3


    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setLayout(sidebarLayout);
    sidebarWidget->setFixedWidth(200);
    sidebarWidget->setStyleSheet("background-color: #F5F5F5;");

    QHBoxLayout *globalLayout = new QHBoxLayout(this);
    globalLayout->setContentsMargins(0, 0, 0, 0);
    globalLayout->setSpacing(0);
    globalLayout->addWidget(sidebarWidget);
    globalLayout->addWidget(stackedWidget, 1);

    this->setStyleSheet("background-color: white; color: black;");
    setWindowTitle("Système de Gestion de Parking");
    resize(1150, 750);
}
void ParkingWindow::afficherHistorique() {
    // Vider le tableau actuel
    historyTable->setRowCount(0);
    // Modifier les en-têtes pour inclure la date de réservation
    historyTable->setHorizontalHeaderLabels({"PLACEID", "NUMERO", "STATUT", "LOCALISATION", "DATE RESERVATION", "Actions"});

    QSqlQuery query;
    // Modifier la requête pour inclure la date de réservation
    if (!query.exec("SELECT PLACEID, NUMERO, STATUT, LOCALISATION, TO_CHAR(DATERESERVATION, 'DD/MM/YYYY HH24:MI') as DATE_RESERVATION FROM PlacesParking ORDER BY PLACEID DESC")) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger l'historique: " + query.lastError().text());
        return;
    }

    while (query.next()) {
        QString placeId = query.value("PLACEID").toString();
        QString numero = query.value("NUMERO").toString();
        QString statut = query.value("STATUT").toString();
        QString localisation = query.value("LOCALISATION").toString();
        QString dateReservation = query.value("DATE_RESERVATION").toString();

        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        // Ajouter les données dans les colonnes
        historyTable->setItem(row, 0, new QTableWidgetItem(placeId));
        historyTable->setItem(row, 1, new QTableWidgetItem(numero));
        historyTable->setItem(row, 2, new QTableWidgetItem(statut));
        historyTable->setItem(row, 3, new QTableWidgetItem(localisation));
        historyTable->setItem(row, 4, new QTableWidgetItem(dateReservation));

        // Ajouter les boutons d'action dans la colonne 5 (Actions)
        QPushButton *viewBtn = new QPushButton("Consulter");
        QPushButton *editBtn = new QPushButton("Modifier");
        QPushButton *deleteBtn = new QPushButton("Supprimer");

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addWidget(viewBtn);
        btnLayout->addWidget(editBtn);
        btnLayout->addWidget(deleteBtn);
        btnLayout->setContentsMargins(5, 2, 5, 2);
        btnLayout->setSpacing(5);

        QWidget *btnWidget = new QWidget();
        btnWidget->setLayout(btnLayout);
        historyTable->setCellWidget(row, 5, btnWidget); // Colonne 5 pour les actions

        // Connecter les boutons
        connect(viewBtn, &QPushButton::clicked, [this, row]() { onViewButtonClicked(row); });
        connect(editBtn, &QPushButton::clicked, this, [this, row]() { onModifyButtonClicked(row); });
        connect(deleteBtn, &QPushButton::clicked, [this, row]() { onDeleteButtonClicked(row); });
    }

    // Ajuster la largeur des colonnes
    historyTable->setColumnWidth(0, 70);   // PLACEID
    historyTable->setColumnWidth(1, 80);   // NUMERO
    historyTable->setColumnWidth(2, 90);   // STATUT
    historyTable->setColumnWidth(3, 150);  // LOCALISATION
    historyTable->setColumnWidth(4, 150);  // DATE RESERVATION
    historyTable->setColumnWidth(5, 180);  // Actions
}
void ParkingWindow::showModifyForm(int row) {
    if (!historyTable || row < 0 || row >= historyTable->rowCount()) {
        return;
    }

    // Récupérer les données de la ligne sélectionnée
    QString placeId = historyTable->item(row, 0)->text();
    QString numero = historyTable->item(row, 1)->text();
    QString statut = historyTable->item(row, 2)->text();
    QString localisation = historyTable->item(row, 3)->text();

    // Créer une boîte de dialogue de modification
    QDialog *modifyDialog = new QDialog(this);
    modifyDialog->setWindowTitle("Modifier la place de parking");
    modifyDialog->setFixedSize(400, 300);

    QFormLayout *formLayout = new QFormLayout(modifyDialog);

    // Champs de formulaire
    QLineEdit *numeroEdit = new QLineEdit(numero);
    QLineEdit *localisationEdit = new QLineEdit(localisation);
    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"libre", "reservee", "occupee"});
    statutCombo->setCurrentText(statut.toLower());

    formLayout->addRow("Numéro:", numeroEdit);
    formLayout->addRow("Localisation:", localisationEdit);
    formLayout->addRow("Statut:", statutCombo);

    // Boutons
    QPushButton *saveButton = new QPushButton("Enregistrer");
    QPushButton *cancelButton = new QPushButton("Annuler");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    formLayout->addRow(buttonLayout);

    // Connexions
    connect(cancelButton, &QPushButton::clicked, modifyDialog, &QDialog::reject);
    connect(saveButton, &QPushButton::clicked, [=]() {
        // Mettre à jour la base de données
        QSqlQuery query;
        query.prepare("UPDATE PlacesParking SET NUMERO = :numero, LOCALISATION = :localisation, STATUT = :statut WHERE PLACEID = :id");
        query.bindValue(":numero", numeroEdit->text());
        query.bindValue(":localisation", localisationEdit->text());
        query.bindValue(":statut", statutCombo->currentText());
        query.bindValue(":id", placeId);

        if (query.exec()) {
            // Mettre à jour l'affichage
            historyTable->item(row, 1)->setText(numeroEdit->text());
            historyTable->item(row, 2)->setText(statutCombo->currentText());
            historyTable->item(row, 3)->setText(localisationEdit->text());

            QMessageBox::information(this, "Succès", "Modification enregistrée !");
            modifyDialog->accept();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour: " + query.lastError().text());
        }
    });

    modifyDialog->exec();
}
bool ParkingWindow::isParkingFull() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = 'occupee' OR Statut = 'reservee'");
    if (query.exec() && query.next()) {
        int occupied = query.value(0).toInt();
        int totalSpots = 8; // Remplacez par votre nombre total de places
        return (occupied >= totalSpots);
    }
    return false; // En cas d'erreur, on suppose que le parking n'est pas plein
}
void ParkingWindow::onAddButtonClicked()
{
    if (isParkingFull()) {
        QMessageBox::warning(this, "Parking Complet",
                             "Le parking est complet. Impossible d'ajouter une nouvelle place.");
        return;
    }
    qDebug() << "Slot onAddButtonClicked() appelé.";

    // Récupération des données
    QString numeroPlace = plateInput->text().trimmed();
    QString localisation = prenomInput->text().trimmed();
    QString dateReservation = entryTimeInput->text().trimmed();
    QString statut = vehicleTypeCombo->currentText().toLower();

    // Validation
    if (numeroPlace.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de place est obligatoire.");
        return;
    }

    // Valeurs par défaut
    if (localisation.isEmpty()) localisation = "Niveau -1";
    if (dateReservation.isEmpty()) dateReservation = QDateTime::currentDateTime().toString("HH:mm");

    // Insertion dans la base
    QSqlQuery query;
    query.prepare("INSERT INTO PlacesParking (Numero, Statut, Localisation, DateReservation) "
                  "VALUES (:numero, :statut, :localisation, TO_TIMESTAMP(:dateReservation, 'HH24:MI'))");

    query.bindValue(":numero", numeroPlace);
    query.bindValue(":statut", statut);
    query.bindValue(":localisation", localisation);
    query.bindValue(":dateReservation", dateReservation);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur BD: " + query.lastError().text());
        return;
    }

    // UNIQUEMENT rafraîchir l'affichage
    afficherHistorique();

    // Réinitialiser le formulaire
    plateInput->clear();
    prenomInput->clear();
    entryTimeInput->clear();
    vehicleTypeCombo->setCurrentIndex(0);

    QMessageBox::information(this, "Succès", "Place ajoutée à l'historique !");
    updateStats();
}

void ParkingWindow::setupHistoryRowWidgets(int row, const QString &placeId, const QString &numero, const QString &statut, const QString &localisation)
{
    if (!historyTable) return;

    if (row == historyTable->rowCount()) {
        historyTable->insertRow(row);
    }

    historyTable->setItem(row, 0, new QTableWidgetItem(placeId));
    historyTable->setItem(row, 1, new QTableWidgetItem(numero));
    historyTable->setItem(row, 2, new QTableWidgetItem(statut));
    historyTable->setItem(row, 3, new QTableWidgetItem(localisation));

    // Boutons compacts avec icônes
    QPushButton *viewButton = new QPushButton("consulter");
    QPushButton *modifyButton = new QPushButton("modifier");
    QPushButton *deleteButton = new QPushButton("supprimer");

    // Style minimaliste
    QString buttonStyle = R"(
    QPushButton {
        padding: 2px;
        margin: 1px;
        border: none;
        background: transparent;
        min-width: 24px;
        max-height: 24px;
    }
    QPushButton:hover {
        background: #f0f0f0;
        border-radius: 3px;
    }
)";
    viewButton->setStyleSheet(buttonStyle);
    modifyButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle + "color: #d32f2f;");

    // Tooltips explicites
    viewButton->setToolTip("Voir détails");
    modifyButton->setToolTip("Modifier");
    deleteButton->setToolTip("Supprimer");

    // Layout compact
    QHBoxLayout *actionsLayout = new QHBoxLayout();
    actionsLayout->setContentsMargins(3, 1, 3, 1);
    actionsLayout->setSpacing(2);
    actionsLayout->addWidget(viewButton);
    actionsLayout->addWidget(modifyButton);
    actionsLayout->addWidget(deleteButton);

    QWidget *actionsWidget = new QWidget();
    actionsWidget->setLayout(actionsLayout);
    historyTable->setCellWidget(row, ACTIONS_COLUMN_INDEX, actionsWidget);

    // Connexion des signaux
    connect(viewButton, &QPushButton::clicked, this, [this, row]() { onViewButtonClicked(row); });
    connect(modifyButton, &QPushButton::clicked, this, [this, row]() { onModifyButtonClicked(row); });
    connect(deleteButton, &QPushButton::clicked, this, [this, row]() { onDeleteButtonClicked(row); });
}
void ParkingWindow::onViewButtonClicked(int row)
{
    if (!historyTable || row < 0 || row >= historyTable->rowCount()) {
        qWarning() << "onViewButtonClicked: Invalid row index" << row;
        return;
    }

    QString placeId = historyTable->item(row, 0) ? historyTable->item(row, 0)->text() : "N/D";
    QString numero = historyTable->item(row, 1) ? historyTable->item(row, 1)->text() : "N/D";
    QString statut = historyTable->item(row, 2) ? historyTable->item(row, 2)->text() : "N/D";
    QString localisation = historyTable->item(row, 3) ? historyTable->item(row, 3)->text() : "N/D";
    QString dateReservation = historyTable->item(row, 4) ? historyTable->item(row, 4)->text() : "N/D";

    QMessageBox::information(this, "Détails de la place",
                             QString("<b>Place ID:</b> %1<br>"
                                     "<b>Numéro:</b> %2<br>"
                                     "<b>Statut:</b> %3<br>"
                                     "<b>Localisation:</b> %4<br>"
                                     "<b>Date Réservation:</b> %5")
                                 .arg(placeId, numero, statut, localisation, dateReservation));
}
void ParkingWindow::onModifyButtonClicked(int row) {
    showModifyForm(row);
}

void ParkingWindow::onDeleteButtonClicked(int row)
{
    if (!historyTable || row < 0 || row >= historyTable->rowCount()) {
        qWarning() << "onDeleteButtonClicked: Invalid row index" << row;
        return;
    }

    QString placeId = historyTable->item(row, 0)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                  QString("Voulez-vous vraiment supprimer cette place de parking ?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM PlacesParking WHERE PLACEID = :id");
        query.bindValue(":id", placeId);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression: " + query.lastError().text());
            return;
        }

        // Rafraîchir l'affichage
        afficherHistorique();
        QMessageBox::information(this, "Succès", "Place supprimée avec succès !");
        updateStats();

    }
}
void ParkingWindow::generateParkingTicket()
{
    // [1] Vérifier la sélection
    if (historyTable->currentRow() < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une place dans le tableau");
        return;
    }

    // [2] Récupérer les données
    int row = historyTable->currentRow();
    QString numero = historyTable->item(row, 1)->text();
    QString localisation = historyTable->item(row, 3)->text();
    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy - HH:mm");

    // [3] Création du PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A6)); // Format ticket
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le ticket",
                                                    QDir::homePath() + "/Ticket_Parking_" + numero + ".pdf",
                                                    "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF");
        return;
    }

    // [4] Paramètres de dessin
    int margin = 20;
    int y = 30;
    int contentWidth = printer.width() - 2 * margin;

    QFont fontHeader("Arial", 18, QFont::Bold);
    QFont fontBody("Arial", 12);
    QFont fontFooter("Arial", 10, QFont::StyleItalic);

    // [5] Logo (en-tête)
    QPixmap logo(":/images/logo.png");
    if (!logo.isNull()) {
        int logoSize = 50;
        painter.drawPixmap(printer.width() - margin - logoSize, y,
                           logoSize, logoSize,
                           logo.scaled(logoSize, logoSize, Qt::KeepAspectRatio));
    }

    // [6] Titre principal
    painter.setFont(fontHeader);
    painter.drawText(margin, y + 40, "TICKET DE PARKING");
    y += 70;

    // [7] Informations principales
    painter.setFont(fontBody);
    painter.drawText(margin, y, "Numéro: " + numero); y += 25;
    painter.drawText(margin, y, "Localisation: " + localisation); y += 25;
    painter.drawText(margin, y, "Date: " + date); y += 40;



    // [9] Pied de page
    painter.setFont(fontFooter);
    painter.drawText(margin, y, "Merci pour votre visite !");
    painter.drawText(margin, y + 20, "Présenter ce ticket à la sortie");

    painter.end();

    // [10] Confirmation
    QMessageBox::information(this, "Succès",
                             "Le ticket a été généré avec succès:\n" + fileName);
}

QWidget* ParkingWindow::createStatBox(const QString &icon, const QString &value, const QString &text)
{
    QWidget *box = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(box);
    QLabel *valueLabel = new QLabel(icon + " " + value);
    QLabel *textLabel = new QLabel(text);
    valueLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    textLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(valueLabel, 0, Qt::AlignCenter);
    layout->addWidget(textLabel, 0, Qt::AlignCenter);
    box->setStyleSheet("background-color: #2196F3; color: white; border-radius: 10px; padding: 10px;");
    box->setFixedSize(180, 100);
    return box;
}
void ParkingWindow::sortHistory(const QString &sortOption)
{
    QString orderByClause;

    if (sortOption == "date_desc") {
        orderByClause = "ORDER BY PLACEID DESC";
    }
    else if (sortOption == "date_asc") {
        orderByClause = "ORDER BY PLACEID ASC";
    }
    else if (sortOption == "numero") {
        orderByClause = "ORDER BY NUMERO";
    }
    else if (sortOption == "statut") {
        orderByClause = "ORDER BY STATUT";
    }
    else {
        orderByClause = "ORDER BY PLACEID DESC";
    }

    // Vider le tableau actuel
    historyTable->setRowCount(0);

    QSqlQuery query;
    QString queryStr = QString("SELECT PLACEID, NUMERO, STATUT, LOCALISATION, TO_CHAR(DATERESERVATION, 'DD/MM/YYYY HH24:MI') as DATE_RESERVATION FROM PlacesParking %1").arg(orderByClause);
    if (!query.exec(queryStr)) {
        QMessageBox::critical(this, "Erreur", "Impossible de trier l'historique: " + query.lastError().text());
        return;
    }

    // Remplir le tableau avec les résultats triés
    while (query.next()) {
        QString placeId = query.value("PLACEID").toString();
        QString numero = query.value("NUMERO").toString();
        QString statut = query.value("STATUT").toString();
        QString localisation = query.value("LOCALISATION").toString();
        QString dateReservation = query.value("DATE_RESERVATION").toString();

        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        // Ajouter les données pour les 5 colonnes
        historyTable->setItem(row, 0, new QTableWidgetItem(placeId));
        historyTable->setItem(row, 1, new QTableWidgetItem(numero));
        historyTable->setItem(row, 2, new QTableWidgetItem(statut));
        historyTable->setItem(row, 3, new QTableWidgetItem(localisation));
        historyTable->setItem(row, 4, new QTableWidgetItem(dateReservation));

        // Utiliser setupHistoryRowWidgets pour ajouter les boutons
        setupHistoryRowWidgets(row, placeId, numero, statut, localisation);
    }
}
void ParkingWindow::exportToPDF() {
    // Vérifier s'il y a des données à exporter
    if (historyTable->rowCount() == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", QDir::homePath() + "/historique_parking.pdf", "Fichiers PDF (*.pdf)"); // Added default path/name
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution); // HighResolution is often better for PDF
    printer.setOutputFormat(QPrinter::PdfFormat);
    // ***** FIX 1: Use QPageSize *****
    printer.setPageSize(QPageSize(QPageSize::A4)); // Correct way to set standard page size in Qt 6
    printer.setOutputFileName(fileName);
    // Optionally set margins (in points)
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Point);
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Erreur", "Impossible de démarrer l'écriture PDF."); // LIGNE CORRIGÉE        return;
    }
    // Configuration du style
    painter.setFont(QFont("Arial", 10));
    // Use pageRect with units to get dimensions correctly
    QRectF pageRectPoints = printer.pageRect(QPrinter::Point); // LIGNE CORRIGÉE (Bonne unité)    int rowHeight = 25; // Adjust as needed
    int rowHeight = 25; // <-- AJOUTEZ CETTE LIGNE (Hauteur de ligne en points, ajustez si besoin)
    int margin = 30; // Use a smaller margin, page margins handled by printer now
    int currentX = margin;
    int currentY = margin;

    // Titre
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(currentX, currentY, pageRectPoints.width() - 2 * margin, 30, Qt::AlignHCenter, "Historique des places de parking");
    currentY += 40;

    // En-têtes de colonnes
    QStringList headers = {"ID", "Numéro", "Statut", "Localisation"};
    // Define column widths more dynamically or as percentages if needed
    // Using fixed points here for simplicity
    QList<int> columnWidthsPoints = { 60, 100, 120, 200 };
    int totalHeaderWidth = 0;
    for(int w : columnWidthsPoints) totalHeaderWidth += w;


    // Dessiner les en-têtes
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.setBrush(Qt::lightGray); // Background for header
    painter.drawRect(currentX, currentY, totalHeaderWidth, rowHeight); // Draw header background rect
    painter.setBrush(Qt::NoBrush); // Reset brush

    int headerX = currentX;
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(headerX + 5, currentY, // Add padding
                         columnWidthsPoints[i] - 10, rowHeight, // Adjust width for padding
                         Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
        // Draw vertical line
        if (i < headers.size() -1) {
            painter.drawLine(headerX + columnWidthsPoints[i], currentY, headerX + columnWidthsPoints[i], currentY + rowHeight);
        }
        headerX += columnWidthsPoints[i];
    }
    // Draw bottom line for header
    painter.drawLine(currentX, currentY + rowHeight, currentX + totalHeaderWidth, currentY + rowHeight);

    currentY += rowHeight;


    // Contenu du tableau
    painter.setFont(QFont("Arial", 10)); // Reset font for content
    for (int row = 0; row < historyTable->rowCount(); ++row) {
        // ***** FIX 2: Use pageRect with units for page break check *****
        // Check if adding the next row exceeds the page height (considering bottom margin)
        if (currentY + rowHeight > pageRectPoints.height() - margin) {
            if (!printer.newPage()) {
                QMessageBox::warning(this, "Erreur", "Impossible de créer une nouvelle page PDF.");
                painter.end(); // Abort painting
                return;
            }
            currentY = margin; // Reset Y to top margin on new page
            // Optionally redraw headers on new page (add header drawing logic here if needed)
        }

        int cellX = currentX;
        for (int col = 0; col < headers.size(); ++col) { // Only iterate through columns you want to print
            QTableWidgetItem *item = historyTable->item(row, col);
            painter.drawText(cellX + 5, currentY, // Add padding
                             columnWidthsPoints[col] - 10, rowHeight, // Adjust width for padding
                             Qt::AlignLeft | Qt::AlignVCenter,
                             item ? item->text() : "");
            // Draw vertical line
            if (col < headers.size() -1) {
                painter.drawLine(cellX + columnWidthsPoints[col], currentY, cellX + columnWidthsPoints[col], currentY + rowHeight);
            }
            cellX += columnWidthsPoints[col];
        }
        // Draw horizontal line below row
        painter.drawLine(currentX, currentY + rowHeight, currentX + totalHeaderWidth, currentY + rowHeight);

        currentY += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Export réussi", "PDF généré avec succès :\n" + fileName);
}
void ParkingWindow::exportHistoryToCSV()
{
    // Vérifier s'il y a des données à exporter
    if (historyTable->rowCount() == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }

    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter l'historique en CSV",
                                                    QDir::homePath() + "/historique_parking.csv",
                                                    "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Ouvrir le fichier en écriture
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier pour écriture: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); // Encodage UTF-8

    // Écrire l'en-tête
    out << "ID Place;Numéro;Statut;Localisation\n";

    // Écrire les données
    for (int row = 0; row < historyTable->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < 4; ++col) { // Nous exportons seulement les 4 premières colonnes
            QTableWidgetItem* item = historyTable->item(row, col);
            rowData << (item ? "\"" + item->text().replace("\"", "\"\"") + "\"" : "");
        }
        out << rowData.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Export réussi", "L'historique a été exporté avec succès vers:\n" + fileName);
}

void ParkingWindow::showParkingSpotStatus()
{
    // Create a boîte de dialogue
    QDialog dialog(this);
    dialog.setWindowTitle("🗺 Plan du Parking");
    // Set a reasonable initial size, scrolling will handle larger content
    dialog.setMinimumSize(1000, 600);
    dialog.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // --- Main Layout for the Dialog ---
    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

    // --- Title ---
    QLabel *titleLabel = new QLabel("Plan Visuel du Parking");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    dialogLayout->addWidget(titleLabel);

    // --- Scroll Area for the Parking Plan ---
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true); // Important!
    scrollArea->setStyleSheet("background-color: #E0E0E0; border: none;"); // Light grey background for the area
    dialogLayout->addWidget(scrollArea, 1); // Allow scroll area to expand

    // --- Widget inside Scroll Area ---
    QWidget *parkingPlanContainer = new QWidget();
    parkingPlanContainer->setStyleSheet("background-color: #BDBDBD;"); // Medium grey background for the plan itself
    scrollArea->setWidget(parkingPlanContainer);

    // --- Layout for the Parking Plan Container (Main structure) ---
    QHBoxLayout *planLayout = new QHBoxLayout(parkingPlanContainer);
    planLayout->setSpacing(15);
    planLayout->setContentsMargins(20, 20, 20, 20);

    // --- 1. Left Structure (Stairs/Building) Placeholder ---
    QWidget *leftStructure = new QWidget();
    leftStructure->setFixedSize(80, 400); // Adjust size as needed
    leftStructure->setStyleSheet("background-color: #A0A0A0; border: 1px solid #666;");
    // Optional: Add details like a label or image
    QVBoxLayout* leftLayout = new QVBoxLayout(leftStructure);
    QLabel* stairsLabel = new QLabel("Stairs\nElevator");
    stairsLabel->setAlignment(Qt::AlignCenter);
    stairsLabel->setStyleSheet("color: white; font-weight: bold;");
    leftLayout->addWidget(stairsLabel);
    planLayout->addWidget(leftStructure, 0, Qt::AlignVCenter); // Add to the main horizontal layout

    // --- 2. Central Parking Area (Top Row, Lane, Bottom Row) ---
    QWidget *centralParkingWidget = new QWidget();
    QVBoxLayout *centralParkingLayout = new QVBoxLayout(centralParkingWidget);
    centralParkingLayout->setSpacing(10); // Space between rows and lane
    centralParkingLayout->setContentsMargins(0, 0, 0, 0);

    // --- Fetch Parking Spot Data ---
    QList<QPair<QString, QString>> spots; // List of (Numero, Statut)
    QSqlQuery query;
    // *** IMPORTANT: Order by NUMERO to have some predictable order ***
    // You might need a more sophisticated ORDER BY clause based on your numbering scheme
    query.prepare("SELECT NUMERO, STATUT FROM PlacesParking ORDER BY NUMERO");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les places: " + query.lastError().text());
        return;
    }
    while (query.next()) {
        spots.append(qMakePair(query.value("NUMERO").toString(), query.value("STATUT").toString()));
    }

    // --- Create Top Row Container ---
    QWidget *topRowWidget = new QWidget();
    QHBoxLayout *topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setSpacing(5); // Space between spots
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->addStretch(1); // Add stretch before spots

    // --- Create Bottom Row Container ---
    QWidget *bottomRowWidget = new QWidget();
    QHBoxLayout *bottomRowLayout = new QHBoxLayout(bottomRowWidget);
    bottomRowLayout->setSpacing(5);
    bottomRowLayout->setContentsMargins(0, 0, 0, 0);
    bottomRowLayout->addStretch(1); // Add stretch before spots

    // --- Distribute Spots (Simple Split - **ADAPT THIS LOGIC**) ---
    int totalSpots = spots.size();
    int spotsPerRow = (totalSpots + 1) / 2; // Roughly half, adjust as needed
    const int spotWidth = 70; // Adjust size
    const int spotHeight = 100; // Adjust size (depth)

    for (int i = 0; i < totalSpots; ++i) {
        QString numero = spots[i].first;
        QString statut = spots[i].second;

        QPushButton *spotButton = new QPushButton(numero);
        spotButton->setFixedSize(spotWidth, spotHeight);
        spotButton->setProperty("spotNumber", numero);
        spotButton->setProperty("spotStatus", statut);
        spotButton->setToolTip(QString("Place %1 - Statut: %2").arg(numero).arg(statut));
        spotButton->setCursor(Qt::PointingHandCursor);


        // Color styling (similar to your original code, adjust colors if needed)
        QString color_occupied = "#E0BBE4"; // Violet clair
        QString color_reserved = "#D291BC"; // Rose
        QString color_free = "#957DAD";     // Violet foncé
        QString color_border = "#5D3FD3";   // Darker purple border
        QString bgColor;

        if (statut.compare("occupee", Qt::CaseInsensitive) == 0) bgColor = color_occupied;
        else if (statut.compare("reservee", Qt::CaseInsensitive) == 0) bgColor = color_reserved;
        else bgColor = color_free; // Default to free

        spotButton->setStyleSheet(QString(
                                      "QPushButton {"
                                      "   background-color: %1;"
                                      "   color: white;" // Text color
                                      "   border: 1px solid %2;" // Border color
                                      "   border-radius: 4px;"
                                      "   font-weight: bold;"
                                      "   padding: 5px;" // Add some padding inside
                                      "}"
                                      "QPushButton:hover {"
                                      "   border: 2px solid %2;" // Thicker border on hover
                                      "   background-color: %3;" // Slightly darker on hover maybe?
                                      "}"
                                      ).arg(bgColor).arg(color_border).arg(bgColor.left(bgColor.length()-2)+"AA")); // Simple hover effect example


        connect(spotButton, &QPushButton::clicked, this, &ParkingWindow::onParkingSpotClicked);

        // Add to the correct row layout
        if (i < spotsPerRow) {
            topRowLayout->addWidget(spotButton);
        } else {
            bottomRowLayout->addWidget(spotButton);
        }
    }

    topRowLayout->addStretch(1); // Add stretch after spots
    bottomRowLayout->addStretch(1); // Add stretch after spots

    // --- Create Driving Lane ---
    QWidget *laneWidget = new QWidget();
    laneWidget->setFixedHeight(60); // Adjust height
    laneWidget->setStyleSheet("background-color: #757575; border-radius: 5px;"); // Dark grey for lane
    QHBoxLayout *laneLayout = new QHBoxLayout(laneWidget);
    laneLayout->addStretch();
    // Add arrow (optional)
    QLabel* arrowLabel = new QLabel();
    QPixmap arrowPixmap(":/path/to/your/up_arrow.png"); // *** PROVIDE CORRECT PATH to an arrow image ***
    if (!arrowPixmap.isNull()) {
        arrowLabel->setPixmap(arrowPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        laneLayout->addWidget(arrowLabel); // Add arrow somewhere in the lane
    } else {
        arrowLabel->setText("-->"); // Fallback text arrow
        arrowLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
        laneLayout->addWidget(arrowLabel);
    }
    laneLayout->addStretch();


    // --- Assemble Central Parking Area ---
    centralParkingLayout->addWidget(topRowWidget);
    centralParkingLayout->addWidget(laneWidget);
    centralParkingLayout->addWidget(bottomRowWidget);

    planLayout->addWidget(centralParkingWidget, 1); // Add central area to main horizontal layout, allow stretching

    // --- 3. Right Structure (Exit) Placeholder ---
    QWidget *rightStructure = new QWidget();
    rightStructure->setFixedSize(30, 100); // Adjust size
    rightStructure->setStyleSheet("background-color: #A0A0A0; border: 1px solid #666;");
    planLayout->addWidget(rightStructure, 0, Qt::AlignVCenter);

    // --- Legend (Below Scroll Area) ---
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->setContentsMargins(20, 10, 20, 0); // Add some margin
    legendLayout->addWidget(createLegendItem("Libre", "#957DAD")); // Use the same colors as spots
    legendLayout->addWidget(createLegendItem("Réservée", "#D291BC"));
    legendLayout->addWidget(createLegendItem("Occupée", "#E0BBE4"));
    legendLayout->addStretch();
    dialogLayout->addLayout(legendLayout);

    // --- Close Button ---
    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   padding: 8px 16px; background-color: #5dade2; color: white;"
        "   border: none; border-radius: 4px; margin: 10px;"
        "}"
        "QPushButton:hover { background-color: #2980B9; }"
        );
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();
    dialogLayout->addLayout(buttonLayout);

    // --- Execute Dialog ---
    dialog.exec();
}

QLabel* ParkingWindow::createLegendItem(const QString &text, const QString &color)
{
    // Your existing implementation should be fine
    QLabel *label = new QLabel();
    QHBoxLayout *layout = new QHBoxLayout(label); // Use QHBoxLayout for horizontal arrangement

    QLabel *colorLabel = new QLabel();
    colorLabel->setFixedSize(20, 20);
    colorLabel->setStyleSheet(QString("background-color: %1; border: 1px solid #7f8c8d; border-radius: 3px;").arg(color));

    QLabel *textLabel = new QLabel(text);
    textLabel->setStyleSheet("margin-left: 5px;"); // Add space between color and text

    layout->addWidget(colorLabel);
    layout->addWidget(textLabel);
    layout->setContentsMargins(0, 0, 10, 0); // Right margin for spacing between legend items
    layout->addStretch(); // Optional: Pushes items to the left if the container is large

    label->setLayout(layout); // Set the layout on the parent label
    return label;
}

void ParkingWindow::onParkingSpotClicked()
{
    // Your existing implementation should be fine
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString spotNumber = button->property("spotNumber").toString();
    QString spotStatus = button->property("spotStatus").toString();

    // Récupérer les détails complets depuis la base (optional, could show basic info first)
    QSqlQuery query;
    // Make sure to select all necessary details you want to show
    query.prepare("SELECT NUMERO, STATUT, LOCALISATION, DATERESERVATION FROM PlacesParking WHERE NUMERO = :numero");
    query.bindValue(":numero", spotNumber);

    QString details = QString("<b>Numéro:</b> %1<br><b>Statut:</b> %2<br>").arg(spotNumber).arg(spotStatus);

    if (query.exec() && query.next()) {
        details += QString("<b>Localisation:</b> %1<br>").arg(query.value("LOCALISATION").toString());
        // Format the date/time nicely if it exists
        QVariant dateVal = query.value("DATERESERVATION");
        if (!dateVal.isNull()) {
            details += QString("<b>Date Réservation/Entrée:</b> %1").arg(dateVal.toDateTime().toString("dd/MM/yyyy HH:mm:ss"));
        } else {
            details += QString("<b>Date Réservation/Entrée:</b> N/A");
        }
    } else {
        qWarning() << "Could not retrieve full details for spot" << spotNumber << ":" << query.lastError().text();
        details += "<i>(Détails complets non disponibles)</i>";
    }


    QMessageBox::information(this, "Détails de la place " + spotNumber, details);
}
void ParkingWindow::setupCharts()
{
    // Création du graphique camembert
    chart = new QChart();
    chart->setTitle("Répartition des places de parking");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Création de la série en camembert
    QPieSeries *pieSeries = new QPieSeries();

    // Ajout des tranches (elles seront mises à jour dans updateStats())
    pieSeries->append("occupeees", 0);
    pieSeries->append("Libres", 0);
    pieSeries->append("reserveees", 0);

    // Configuration du style des tranches
    for (QPieSlice *slice : pieSeries->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setExploded(false);
        slice->setPen(QPen(Qt::white, 2));
    }

    // Couleurs personnalisées
    pieSeries->slices().at(0)->setColor(QColor("#E0BBE4")); // pour occupeees
    pieSeries->slices().at(1)->setColor(QColor("#957DAD")); // pour libres
    pieSeries->slices().at(2)->setColor(QColor("#D291BC")); // Orange pour reserveees

    chart->addSeries(pieSeries);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Mise à jour initiale
    updateStats();
}

void ParkingWindow::updateStats()
{
    // Récupération des données depuis la base
    int occupied = getCurrentCount("occupee");
    int free = getCurrentCount("libre");
    int reserved = getCurrentCount("reservee");
    int total = 100; // Remplacez par votre nombre total de places

    // Calcul des pourcentages
    double occupiedPercent = (occupied * 100.0) / total;
    double freePercent = (free * 100.0) / total;
    double reservedPercent = (reserved * 100.0) / total;

    // Récupération de la série camembert
    QPieSeries *pieSeries = qobject_cast<QPieSeries*>(chart->series().first());
    if (!pieSeries) return;

    // Mise à jour des valeurs
    pieSeries->slices().at(0)->setValue(occupied);
    pieSeries->slices().at(0)->setLabel(QString("occupeees %1%").arg(occupiedPercent, 0, 'f', 1));

    pieSeries->slices().at(1)->setValue(free);
    pieSeries->slices().at(1)->setLabel(QString("Libres %1%").arg(freePercent, 0, 'f', 1));

    pieSeries->slices().at(2)->setValue(reserved);
    pieSeries->slices().at(2)->setLabel(QString("reserveees %1%").arg(reservedPercent, 0, 'f', 1));

    // Actualiser l'affichage
    chart->update();
}

QVector<QPointF> ParkingWindow::getHourlyData(const QString &status)
{
    QVector<QPointF> data;
    int currentHour = QTime::currentTime().hour();

    QSqlQuery query;
    if (status == "électrique") {
        // Requête pour les véhicules électriques (ajuster selon votre schéma de base)
        query.prepare("SELECT EXTRACT(HOUR FROM DateReservation) as hour, COUNT(*) "
                      "FROM PlacesParking WHERE TypeVehicule = 'électrique' "
                      "GROUP BY EXTRACT(HOUR FROM DateReservation) "
                      "ORDER BY hour");
    } else {
        query.prepare("SELECT EXTRACT(HOUR FROM DateReservation) as hour, COUNT(*) "
                      "FROM PlacesParking WHERE Statut = :status "
                      "GROUP BY EXTRACT(HOUR FROM DateReservation) "
                      "ORDER BY hour");
        query.bindValue(":status", status);
    }

    if (!query.exec()) {
        qWarning() << "Erreur lors de la récupération des données horaires:" << query.lastError().text();
        return data;
    }

    // Remplir les données manquantes avec 0
    for (int hour = 0; hour <= currentHour; ++hour) {
        data.append(QPointF(hour, 0));
    }

    while (query.next()) {
        int hour = query.value(0).toInt();
        int count = query.value(1).toInt();
        if (hour >= 0 && hour <= currentHour) {
            data[hour].setY(count);
        }
    }

    return data;
}

int ParkingWindow::getCurrentCount(const QString &status)
{
    QSqlQuery query;
    if (status == "électrique") {
        query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE TypeVehicule = 'électrique'");
    } else {
        query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = :status");
        query.bindValue(":status", status);
    }

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
void ParkingWindow::setupNotificationSystem()
{
    // Créer l'icône de la zone de notification
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/notification.png")); // Remplacez par le chemin de votre icône
    trayIcon->setToolTip("Parking Notification System");
    trayIcon->show();

    // Configurer le timer pour vérifier périodiquement l'état du parking
    notificationTimer = new QTimer(this);
    connect(notificationTimer, &QTimer::timeout, this, &ParkingWindow::checkParkingStatus);
    notificationTimer->start(30000); // Vérifie toutes les 30 secondes

    lastOccupiedCount = 0;
    isParkingFullNotified = false;
}

void ParkingWindow::checkParkingStatus()
{
    QSqlQuery query;

    // Vérifier si le parking est plein
    query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = 'libre'");
    query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = 'libre'");
    if (query.exec() && query.next()) {
        int freeSpots = query.value(0).toInt();
        int totalSpots = 8; // Remplacez par votre nombre total de places tay

        if (freeSpots == 0 && !isParkingFullNotified) {
            showNotification("Parking Complet", "Attention : Le parking est maintenant complet !");
            isParkingFullNotified = true;

            // Ajouter une notification visuelle dans l'interface
            notificationButton->setStyleSheet("QPushButton { background-color: #FF5733; border-radius: 20px; }");
            QTimer::singleShot(5000, [this]() {
                notificationButton->setStyleSheet("QPushButton { background-color: transparent; border-radius: 20px; }");
            });
        } else if (freeSpots > 0) {
            isParkingFullNotified = false;
        }
    }

    // Vérifier les nouvelles entrées/sorties
    query.prepare("SELECT COUNT(*) FROM PlacesParking WHERE Statut = 'occupee'");
    if (query.exec() && query.next()) {
        int currentOccupied = query.value(0).toInt();

        if (currentOccupied > lastOccupiedCount) {
            showNotification("Nouvelle Entrée", QString("Un nouveau véhicule est entré (%1 places occupeees)").arg(currentOccupied));
        } else if (currentOccupied < lastOccupiedCount) {
            showNotification("Nouvelle Sortie", QString("Un véhicule est sorti (%1 places occupeees)").arg(currentOccupied));
        }

        lastOccupiedCount = currentOccupied;
    }
}

void ParkingWindow::showNotification(const QString &title, const QString &message)
{
    // Afficher une notification système
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 5000);

    // Afficher également une notification dans l'application
    QMessageBox::information(this, title, message);

    // Vous pouvez aussi ajouter une notification visuelle dans l'interface
    notificationButton->setStyleSheet("QPushButton { background-color: #FF5733; border-radius: 20px; }");
    QTimer::singleShot(3000, [this]() {
        notificationButton->setStyleSheet("QPushButton { background-color: transparent; border-radius: 20px; }");
    });
}
void ParkingWindow::filterHistory()
{
    QString searchText = searchLineEdit->text().trimmed();
    QString searchField = searchFieldCombo->currentData().toString();

    // Vider le tableau actuel
    historyTable->setRowCount(0);

    QSqlQuery query;
    QString queryStr = "SELECT PLACEID, NUMERO, STATUT, LOCALISATION, TO_CHAR(DATERESERVATION, 'DD/MM/YYYY HH24:MI') as DATE_RESERVATION FROM PlacesParking ";
    if (!searchText.isEmpty()) {
        if (searchField == "all") {
            queryStr += "WHERE PLACEID LIKE :search OR NUMERO LIKE :search OR STATUT LIKE :search OR LOCALISATION LIKE :search OR TO_CHAR(DATERESERVATION, 'DD/MM/YYYY HH24:MI') LIKE :search ";
        } else if (searchField == "DATE_RESERVATION") {
            queryStr += "WHERE TO_CHAR(DATERESERVATION, 'DD/MM/YYYY HH24:MI') LIKE :search ";
        } else {
            queryStr += "WHERE " + searchField + " LIKE :search ";
        }
    }
    queryStr += "ORDER BY PLACEID DESC";

    query.prepare(queryStr);
    query.bindValue(":search", "%" + searchText + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        return;
    }

    // Remplir le tableau avec les résultats filtrés
    while (query.next()) {
        QString placeId = query.value("PLACEID").toString();
        QString numero = query.value("NUMERO").toString();
        QString statut = query.value("STATUT").toString();
        QString localisation = query.value("LOCALISATION").toString();
        QString dateReservation = query.value("DATE_RESERVATION").toString();

        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        // Ajouter les données pour les 5 colonnes
        historyTable->setItem(row, 0, new QTableWidgetItem(placeId));
        historyTable->setItem(row, 1, new QTableWidgetItem(numero));
        historyTable->setItem(row, 2, new QTableWidgetItem(statut));
        historyTable->setItem(row, 3, new QTableWidgetItem(localisation));
        historyTable->setItem(row, 4, new QTableWidgetItem(dateReservation));

        // Utiliser setupHistoryRowWidgets pour ajouter les boutons
        setupHistoryRowWidgets(row, placeId, numero, statut, localisation);
    }
}

void ParkingWindow::setupSerialConnection()
{
    arduino_is_available = false; // Assume not available initially
    arduino_port_name = "";

    qDebug() << "Recherche de l'Arduino...";
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : ports) {
        qDebug() << "Port trouvé:" << portInfo.portName()
                 << "VID:" << (portInfo.hasVendorIdentifier() ? QString::number(portInfo.vendorIdentifier(), 16) : "N/A")
                 << "PID:" << (portInfo.hasProductIdentifier() ? QString::number(portInfo.productIdentifier(), 16) : "N/A");

        if (portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier()) {
            // <-- VERIFY THESE IDs ARE CORRECT FOR YOUR BOARD -->
            if (portInfo.vendorIdentifier() == arduino_vendor_id &&
                portInfo.productIdentifier() == arduino_product_id) {
                arduino_port_name = portInfo.portName();
                arduino_is_available = true;
                qDebug() << "Arduino trouvé sur le port:" << arduino_port_name;
                break; // Found it, stop searching
            }
        }
    }

    if (arduino_is_available) {
        if (arduino->isOpen()) {
            qDebug() << "Port série déjà ouvert. Fermeture avant réouverture.";
            arduino->close();
        }
        arduino->setPortName(arduino_port_name);

        // <-- VERIFY BAUD RATE MATCHES ARDUINO SKETCH -->
        if (arduino->open(QSerialPort::ReadWrite)) { // Open in ReadWrite mode
            if (arduino->setBaudRate(QSerialPort::Baud9600) &&
                arduino->setDataBits(QSerialPort::Data8) &&
                arduino->setParity(QSerialPort::NoParity) &&
                arduino->setStopBits(QSerialPort::OneStop) &&
                arduino->setFlowControl(QSerialPort::NoFlowControl))
            {
                qDebug() << "Port série" << arduino_port_name << "ouvert et configuré avec succès.";
                // Connect signals AFTER successful open and configuration
                connect(arduino, &QSerialPort::readyRead, this, &ParkingWindow::readSerialData);
                connect(arduino, &QSerialPort::errorOccurred, this, &ParkingWindow::handleSerialError);
                serialBuffer.clear(); // Clear any old data in the buffer on reconnect
            } else {
                qWarning() << "Échec de la configuration du port série:" << arduino->errorString();
                arduino->close(); // Close if configuration failed
                arduino_is_available = false; // Mark as unavailable
            }
        } else {
            qWarning() << "Impossible d'ouvrir le port série" << arduino_port_name << ":" << arduino->errorString();
            arduino_is_available = false; // Mark as unavailable
            // Optionally show a user message here
            // QMessageBox::warning(this, "Erreur Arduino", "Impossible de se connecter à l'Arduino sur le port " + arduino_port_name);
        }
    } else {
        qDebug() << "Aucun Arduino correspondant trouvé (VID:" << QString::number(arduino_vendor_id, 16)
                 << ", PID:" << QString::number(arduino_product_id, 16) << "). Vérifiez la connexion et les IDs.";
        // Optionally show a user message here
        // QMessageBox::information(this, "Info Arduino", "Aucun Arduino compatible détecté.");
    }
}

void ParkingWindow::readSerialData()
{
    // <-- CHANGE: Use the member variable serialBuffer, not a static one -->
    if (!arduino || !arduino->isOpen() || !arduino->bytesAvailable()) {
        return; // Nothing to read or port not ready
    }

    // Append new data to the member buffer
    serialBuffer.append(arduino->readAll());
    // qDebug() << "Serial Buffer content:" << serialBuffer; // Debug: See raw buffer content

    // Process complete lines (terminated by newline)
    // <-- VERIFY ARDUINO SENDS '\n' AT THE END OF EACH JSON MESSAGE -->
    while (serialBuffer.contains('\n')) {
        int newlineIndex = serialBuffer.indexOf('\n');
        if (newlineIndex >= 0) {
            // Extract the complete line (excluding the newline)
            QByteArray completeLine = serialBuffer.left(newlineIndex).trimmed(); // trimmed() removes whitespace

            // Remove the processed line (including the newline) from the buffer
            serialBuffer = serialBuffer.mid(newlineIndex + 1);

            if (!completeLine.isEmpty()) {
                // qDebug() << "Processing line:" << completeLine; // Debug: Show line being processed
                processSerialData(completeLine);
            }
        } else {
            // Should not happen if contains('\n') is true, but break just in case
            break;
        }
    }
    // If buffer gets too large without a newline, consider clearing it or logging an error
    if (serialBuffer.size() > 4096) { // Example limit: 4KB
        qWarning() << "Serial buffer grew very large without newline, clearing buffer.";
        serialBuffer.clear();
    }
}
void ParkingWindow::processSerialData(const QByteArray &data)
{
    // <-- CHANGE: Added more debugging -->
    // qDebug() << "Attempting to parse JSON:" << data;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Échec de l'analyse JSON:" << error.errorString();
        qWarning() << "Données brutes reçues:" << data; // Log the problematic data
        return; // Cannot process invalid JSON
    }

    if (!doc.isObject()) {
        qWarning() << "Le document JSON reçu n'est pas un objet:" << data;
        return;
    }

    QJsonObject obj = doc.object();

    // <-- CHANGE: Added checks for key existence and type -->
    int available = -1; // Default/invalid value
    if (obj.contains("parkingAvailable") && obj["parkingAvailable"].isDouble()) { // JSON numbers are often doubles
        available = obj["parkingAvailable"].toInt();
    } else {
        qWarning() << "Clé JSON 'parkingAvailable' manquante ou type incorrect dans:" << data;
    }

    QJsonArray sensors;
    if (obj.contains("sensors") && obj["sensors"].isArray()) {
        sensors = obj["sensors"].toArray();
    } else {
        qWarning() << "Clé JSON 'sensors' manquante ou n'est pas un tableau dans:" << data;
    }

    int barrierState = 0; // Default state
    if (obj.contains("barrierState") && obj["barrierState"].isDouble()) {
        barrierState = obj["barrierState"].toInt();
    } else {
        qWarning() << "Clé JSON 'barrierState' manquante ou type incorrect dans:" << data;
    }

    // <-- CHANGE: Safer sensor value extraction -->
    double s1 = 0.0, s2 = 0.0, s3 = 0.0;
    if (sensors.size() >= 1 && sensors[0].isDouble()) s1 = sensors[0].toDouble();
    if (sensors.size() >= 2 && sensors[1].isDouble()) s2 = sensors[1].toDouble();
    if (sensors.size() >= 3 && sensors[2].isDouble()) s3 = sensors[2].toDouble();
    if (sensors.size() < 3) {
        qWarning() << "Le tableau JSON 'sensors' contient moins de 3 éléments dans:" << data;
    }
    updateParkingDisplay(available, s1, s2, s3); // Supprimez le 5ème paramètre
    if (barrierState == -1) {
        qDebug() << "État barrière -1 détecté, déclenchement de l'ajout à la base de données.";
        addParkingEntryToDatabase(); // Call the function to add entry
    } else {
    }

    updateDatabase(available); // Call placeholder update function
}

void ParkingWindow::addParkingEntryToDatabase()
{
    if (isParkingFull()) {
        qDebug() << "Tentative d'ajout automatique alors que le parking est plein - bloqué.";
        showNotification("Parking Complet", "Tentative d'entrée bloquée - parking complet!");
        return;
    }
    qDebug() << "Début de addParkingEntryToDatabase() [Déclenché par Arduino]...";
    QString zone = QRandomGenerator::global()->bounded(2) ? "A" : "B";
    int spotNumber = QRandomGenerator::global()->bounded(20) + 1;
    QString numeroPlace = QString("%1%2").arg(zone).arg(spotNumber);
    qDebug() << "Place générée aléatoirement (ATTENTION: Non fiable) :" << numeroPlace;
    // --- End Warning ---
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentTimeString = currentDateTime.toString("HH:mm"); // Format for TO_TIMESTAMP('HH24:MI') assumed

    qDebug() << "Heure actuelle (pour BDD) :" << currentTimeString;

    QSqlQuery query;
    QString sql = "INSERT INTO PlacesParking (Numero, Statut, Localisation, DateReservation) "
                  "VALUES (:numero, :statut, :localisation, TO_TIMESTAMP(:dateReservation, 'HH24:MI'))";

    qDebug() << "Préparation de la requête SQL d'ajout automatique :" << sql;
    if (!query.prepare(sql)) {
        qCritical() << "ÉCHEC de la préparation de la requête d'ajout automatique:" << query.lastError().text();
        // Show notification or log, but don't necessarily stop the whole app
        showNotification("Erreur Interne", "Échec préparation requête ajout auto.");
        return;
    }

    query.bindValue(":numero", numeroPlace);
    query.bindValue(":statut", "occupee"); // <-- Fixed status for Arduino entry
    query.bindValue(":localisation", "Entrée Auto (Arduino)"); // <-- More specific location
    query.bindValue(":dateReservation", currentTimeString);

    qDebug() << "Exécution de la requête d'ajout automatique...";
    if (!query.exec()) {
        qCritical() << "ERREUR lors de l'ajout automatique:" << query.lastError().databaseText()
        << "| Driver Text:" << query.lastError().driverText();
        showNotification("Erreur Base de Données", "Échec ajout auto. Vérifiez logs.");
    } else {
        qInfo() << "SUCCÈS: Entrée automatique ajoutée pour la place (fictive)" << numeroPlace;
        showNotification("Nouvelle Entrée (Auto)", QString("Place %1 occupeee").arg(numeroPlace));

        // Refresh UI only on successful insert
        qDebug() << "Rafraîchissement de l'historique et des statistiques après ajout auto...";
        afficherHistorique();
        updateStats(); // Make sure updateStats itself doesn't cause errors
    }
    qDebug() << "Fin de addParkingEntryToDatabase().";
}


void ParkingWindow::updateParkingDisplay(int available, float sensor1, float sensor2, float sensor3)
{
    // Affiche les informations reçues de l'Arduino dans la console de débogage
    qDebug() << "Arduino Update - Disponible:" << available
             << "| S1:" << QString::number(sensor1, 'f', 1)
             << "| S2:" << QString::number(sensor2, 'f', 1)
             << "| S3:" << QString::number(sensor3, 'f', 1);

}

void ParkingWindow::updateDatabase(int availableSpots)
{

}


void ParkingWindow::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return; // No error occurred.
    }

    // Prevent recursive errors if closing the port itself causes an error signal
    static bool handlingError = false;
    if (handlingError) return;
    handlingError = true;


    QString errorMsg = "Erreur de communication série : ";
    bool shouldClosePort = false;
    bool isCritical = false; // Flag for showing user message

    switch (error) {
    case QSerialPort::DeviceNotFoundError:
        errorMsg += "Périphérique non trouvé.";
        arduino_is_available = false;
        shouldClosePort = true; // Port is likely gone
        isCritical = true;
        break;
    case QSerialPort::PermissionError:
        errorMsg += "Erreur de permission.";
        arduino_is_available = false;
        shouldClosePort = true; // Can't operate on it
        isCritical = true;
        break;
    case QSerialPort::OpenError:
        errorMsg += "Erreur lors de l'ouverture du port (déjà géré dans setup?).";
            // arduino_is_available should already be false from setupSerialConnection failure
        break; // Open error handled in setupSerialConnection usually
    case QSerialPort::NotOpenError:
        errorMsg += "Opération sur port non ouvert.";
        // Indicates a logic error elsewhere, port might be closed already
        break;
    case QSerialPort::WriteError:
        errorMsg += "Erreur d'écriture.";
            // Might be temporary or indicate disconnection
        isCritical = true; // Often implies connection issue
        break;
    case QSerialPort::ReadError:
        errorMsg += "Erreur de lecture.";
            // Might be temporary or indicate disconnection
        isCritical = true; // Often implies connection issue
        break;
    case QSerialPort::ResourceError:
        errorMsg += "Erreur de ressource (Périphérique déconnecté?).";
        arduino_is_available = false;
        shouldClosePort = true; // Device is gone
        isCritical = true;
        break;
    case QSerialPort::UnsupportedOperationError:
        errorMsg += "Opération non supportée.";
        break;
    case QSerialPort::TimeoutError:
        errorMsg += "Timeout."; // Might be expected depending on config
        break;
    case QSerialPort::UnknownError:
    default:
        errorMsg += "Erreur inconnue.";
        arduino_is_available = false; // Assume critical on unknown
        shouldClosePort = true;
        isCritical = true;
        break;
    }

    qWarning() << errorMsg << "(Code:" << error << ")" << arduino->errorString(); // Include system error string

    if (shouldClosePort && arduino && arduino->isOpen()) {
        qDebug() << "Fermeture du port série en raison d'une erreur critique.";
        // Disconnect signals before closing to prevent further error signals during close
        disconnect(arduino, &QSerialPort::readyRead, this, &ParkingWindow::readSerialData);
        disconnect(arduino, &QSerialPort::errorOccurred, this, &ParkingWindow::handleSerialError);
        arduino->close();
    }

    if (isCritical) {
        QMessageBox::warning(this, "Erreur Arduino", errorMsg + "\nLa communication est peut-être interrompue.");
        // TODO: Consider disabling UI elements that depend on Arduino here
    }

    handlingError = false; // Allow handling future errors
}
// --- End Arduino Communication Section ---
// ... (destructor, other methods like setupCharts, updateStats, etc.) ...
ParkingWindow::~ParkingWindow()
{
    qDebug() << "ParkingWindow destructor called.";
    // QSerialPort 'arduino' will be deleted automatically due to parent 'this'
    // No need to explicitly delete arduino here unless it was created with `new QSerialPort()` (without parent)
    if (arduino && arduino->isOpen()) {
        qDebug() << "Closing serial port in destructor.";
        arduino->close();
    }
}
