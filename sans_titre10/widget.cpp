#include "widget.h"
#include "ui_widget.h"
#include "risque.h"
#include "client.h"
#include "dialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QtCharts> // Inclure QtCharts pour accéder aux fonctionnalités de graphique
#include <QListWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "evacuation.h"
#include <QSqlError>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); // ✅
    ui->dateEdit_RiskDate->setDateTime(QDateTime::currentDateTime());
    ui->notificationsListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    // --- System Tray Icon Setup --- <--- INSÉREZ CE BLOC ICI
    // Check if system tray is available
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        trayIcon = new QSystemTrayIcon(this); // 'this' comme parent
        // **IMPORTANT**: Remplacez par le chemin de VOTRE icône dans resources.qrc
        QIcon icon(":/images/images/notification_bell.png"); // Mettez votre chemin d'icône ici
        if (icon.isNull()) {
            qWarning() << "Icône de notification non trouvée ou invalide. Chemin utilisé: :/images/images/notification_bell.png";
            trayIcon->setIcon(QIcon()); // Utilise une icône par défaut
        } else {
            trayIcon->setIcon(icon);
        }
        trayIcon->setToolTip("Gestion App - Notifications");

        // Connexion optionnelle pour le clic sur l'icône
        connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger) {
                this->showNormal();
                this->activateWindow();
                ui->stackedWidget->setCurrentWidget(ui->notificationsPage);
            }
        });

        trayIcon->show(); // Afficher l'icône
        qDebug() << "Icône de la barre d'état système initialisée.";
    } else {
        qWarning() << "Barre d'état système non disponible. Les notifications pop-up ne seront pas affichées.";
    }
    // --- End System Tray Setup ---

    chargerNotifications();  // Charger les notifications APRÈS la config du trayIcon






    // Connexions
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &Widget::on_searchLineEdit_textChanged);

    // Charger l'image et la transformer en icône
    QPixmap pixmap(":/images/images/search.jpg"); // Chemin de votre image (dans resources.qrc)
    QIcon searchIcon(pixmap); // Transformer l'image en icône

    // Ajouter l'icône au QLineEdit
    ui->searchLineEdit->addAction(searchIcon, QLineEdit::LeadingPosition); // Icône à gauche
    connect(ui->sidebarListWidget, &QListWidget::itemClicked, this, &Widget::gererClicSidebar);
    connect(ui->comboBoxchoisi, SIGNAL(currentIndexChanged(int)), this, SLOT(afficherTimelineClients()));

    afficherClients();
}


Widget::~Widget()
{
    delete ui;
}
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QJsonObject>
// In widget.cpp

void Widget::sauvegarderNotifications()
{
    QJsonArray notificationsArray;

    // Parcourir toutes les notifications dans la liste
    for (int i = 0; i < ui->notificationsListWidget->count(); ++i) {
        QListWidgetItem *item = ui->notificationsListWidget->item(i);
        QJsonObject notificationObj;
        // Save only the text, not the check state or prefixes unless needed
        // Let's adjust how we store/retrieve the core message
        QString itemText = item->text();
        // Basic way to remove potential prefixes if they exist
        if (itemText.startsWith("📢 ") || itemText.startsWith("🛑 [")) {
            // Find the actual message start if prefixes are consistent
            // This might need adjustment based on your exact prefixes
            int firstSpace = itemText.indexOf(' ');
            if (firstSpace != -1) {
                itemText = itemText.mid(firstSpace + 1);
                // If it had the date prefix, remove that too
                if (itemText.startsWith("[")) {
                    int closingBracket = itemText.indexOf("] ");
                    if (closingBracket != -1) {
                        itemText = itemText.mid(closingBracket + 2);
                    }
                }
            }
        }
        notificationObj["message"] = itemText; // Store the core message
        notificationObj["date"] = QDateTime::currentDateTime().toString(); // Or use original date if stored

        notificationsArray.append(notificationObj);
    }

    // Sauvegarder dans un fichier JSON
    QFile file("notifications.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(notificationsArray);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Erreur lors de l'ouverture du fichier pour sauvegarder les notifications.";
    }
}


void Widget::chargerNotifications()
{
    QFile file("notifications.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isArray()) {
            QJsonArray notificationsArray = doc.array();
            ui->notificationsListWidget->clear(); // Clear before loading

            for (const QJsonValue &value : notificationsArray) {
                if (value.isObject()) {
                    QJsonObject notificationObj = value.toObject();
                    QString message = notificationObj["message"].toString();
                    // You might want to add the date back here if you stored it per notification
                    // QString dateStr = notificationObj["date"].toString();
                    // QString displayText = "📢 [" + dateStr + "] " + message; // Example

                    // --- Add Checkbox Functionality ---
                    QListWidgetItem *newItem = new QListWidgetItem("📢 " + message); // Or your preferred format
                    newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable); // Make it checkable
                    newItem->setCheckState(Qt::Unchecked); // Start as unchecked
                    // ---------------------------------

                    ui->notificationsListWidget->addItem(newItem);
                }
            }
        }
        file.close();
    } else {
        qDebug() << "Erreur lors de l'ouverture du fichier pour charger les notifications.";
    }
}


void Widget::afficherNotification(const QString &message)
{
    QString dateTime = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    QString fullMessage = "🛑 [" + dateTime + "] " + message;

    // --- Add Item to List Widget ---
    QListWidgetItem *newItem = new QListWidgetItem(fullMessage);
    newItem->setForeground(Qt::red);
    newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
    newItem->setCheckState(Qt::Unchecked);
    ui->notificationsListWidget->addItem(newItem);

    // --- Update Sidebar Counter and Style ---
    nombreNotificationsNonLues++;
    for (int i = 0; i < ui->sidebarListWidget->count(); ++i) {
        QListWidgetItem *item = ui->sidebarListWidget->item(i);
        if (item != nullptr && item->text().contains("Notifications")) { // Vérif null
            item->setText("🔔 Notifications (" + QString::number(nombreNotificationsNonLues) + ")");
            item->setForeground(Qt::red);
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            break;
        }
    }

    // --- Show System Tray Notification Pop-up --- <--- INSÉREZ CE BLOC ICI
    if (trayIcon && trayIcon->isVisible()) {
        // Utiliser le message principal (sans date/heure) pour la concision
        trayIcon->showMessage(
            "Nouvelle Alerte Critique !",     // Titre de la pop-up
            message,                          // Contenu principal
            QSystemTrayIcon::Warning,         // Icône de type Avertissement
            5000                              // Durée d'affichage (ms)
            );
        qDebug() << "Message système affiché pour: " << message;
    } else {
        qDebug() << "trayIcon non disponible/visible. Pop-up non affiché pour: " << message;
    }
    // --------------------------------------------

    // Sauvegarder les notifications
    sauvegarderNotifications();
}
void Widget::gererClicSidebar(QListWidgetItem *item)
{
    if (item->text().contains("Notifications")) {
        // Remettre la couleur en noir
        item->setForeground(Qt::black);
        QFont font = item->font();
        font.setBold(false);
        item->setFont(font);

        // Réinitialiser le compteur
        nombreNotificationsNonLues = 0;
        item->setText("🔔 Notifications");

        // (Optionnel) Afficher la page notifications
        ui->stackedWidget->setCurrentWidget(ui->notificationsPage);
    }
}


void Widget::on_sidebarListWidget_itemClicked(QListWidgetItem *item)
{
    QString texte = item->text();
    if (texte == "📊 Ajout") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (texte == "👥 Liste Clients") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (texte == "📈 Statistiques") {
        ui->stackedWidget->setCurrentIndex(3);
        afficherTimelineClients();
    }
    else if (texte == "⚠️ Gestion Risques") {
        ui->stackedWidget->setCurrentIndex(4);

    }
    else if (texte == "🔔 Notifications") {
        ui->stackedWidget->setCurrentIndex(5);

    }


}

void Widget::afficherClients(QString colonneTri , QString ordre )
{
    ui->clientTableWidget->clearContents();
    ui->clientTableWidget->setRowCount(0);

    QString queryString = "SELECT CIN, nom, prenom, email, adresse, telephone, date_inscription, pays FROM client";
    if (!colonneTri.isEmpty()) {
        queryString += " ORDER BY " + colonneTri + " " + ordre;
    }

    QSqlQuery query(queryString);
    int row = 0;

    QStringList headers = {"CIN", "Nom", "Prénom", "Email", "Adresse", "Téléphone", "Date d'inscription", "Pays"};
    ui->clientTableWidget->setHorizontalHeaderLabels(headers);

    while (query.next()) {
        ui->clientTableWidget->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QString value;
            if (col == 6) {
                QDateTime dateTime = query.value(col).toDateTime();
                value = dateTime.toString("dd/MM/yyyy");
            } else {
                value = query.value(col).toString();
            }
            ui->clientTableWidget->setItem(row, col, new QTableWidgetItem(value));
        }
        row++;
    }

    ui->clientTableWidget->resizeColumnsToContents();
}
#include <QRegularExpression> // Make sure this is included at the top
#include <QSqlError> // Include for QSqlError
void Widget::on_ajouterButton_AddForm_clicked()
{
    resetAddClientFormStyles(); // Réinitialiser avant la validation

    // ... récupérer les données ...
    QString cinText = ui->cinLineEdit->text().trimmed();
    QString nom = ui->nomLineEdit->text().trimmed();
    QString prenom = ui->prenomLineEdit->text().trimmed();
    QString email = ui->emailLineEdit->text().trimmed();
    QString telephone = ui->telephoneLineEdit->text().trimmed();
    QString zone = ui->zoneComboBox->currentText();
    QString pays = ui->paysComboBox->currentText();
    // ...

    bool isValid = true;
    // Style d'erreur : juste une bordure rouge, pas de fond pour moins d'impact visuel
    const QString errorHighlightStyle = "border: 1px solid red;";

    // --- Validation avec Highlighting et Tooltips ---

    // CIN
    bool cinOk;
    int CIN = cinText.toInt(&cinOk); // Déclarer CIN ici pour la portée
    if (cinText.isEmpty()) {
        ui->cinLineEdit->setStyleSheet(errorHighlightStyle);
        ui->cinLineEdit->setToolTip("Le champ CIN ne peut pas être vide.");
        isValid = false;
    } else if (!cinOk || CIN <= 0 || cinText.length() < 6 || cinText.length() > 8) {
        ui->cinLineEdit->setStyleSheet(errorHighlightStyle);
        ui->cinLineEdit->setToolTip("CIN invalide (nombre positif, 6-8 chiffres).");
        isValid = false;
    }

    // Nom
    if (nom.isEmpty()) {
        ui->nomLineEdit->setStyleSheet(errorHighlightStyle);
        ui->nomLineEdit->setToolTip("Le champ Nom ne peut pas être vide.");
        isValid = false;
    }

    // Prénom
    if (prenom.isEmpty()) {
        ui->prenomLineEdit->setStyleSheet(errorHighlightStyle);
        ui->prenomLineEdit->setToolTip("Le champ Prénom ne peut pas être vide.");
        isValid = false;
    }


    // Email
    QRegularExpression emailRegex(R"(.+@.+\..+)");
    if (email.isEmpty()) {
        ui->emailLineEdit->setStyleSheet(errorHighlightStyle);
        ui->emailLineEdit->setToolTip("Le champ Email ne peut pas être vide.");
        isValid = false;
    } else if (!emailRegex.match(email).hasMatch()) {
        ui->emailLineEdit->setStyleSheet(errorHighlightStyle);
        ui->emailLineEdit->setToolTip("Le format de l'Email est invalide.");
        isValid = false;
    }

    // Téléphone (Optionnel + Format)
    QRegularExpression phoneRegex(R"(^[\d\s+()-]{5,}$)");
    if (!telephone.isEmpty() && !phoneRegex.match(telephone).hasMatch()) {
        ui->telephoneLineEdit->setStyleSheet(errorHighlightStyle);
        ui->telephoneLineEdit->setToolTip("Format téléphone invalide.");
        isValid = false; // Marquer comme invalide même si optionnel, car le format est faux
    }



    // Pays
    if (pays.isEmpty() || pays == "-- Choisir Pays --") {
        ui->paysComboBox->setStyleSheet(errorHighlightStyle);
        ui->paysComboBox->setToolTip("Veuillez sélectionner un Pays.");
        isValid = false;
    }


    // --- Si le formulaire n'est pas valide, on s'arrête ---
    if (!isValid) {
        // Optionnel : Mettre un tooltip général sur le bouton Ajouter pour indiquer qu'il y a des erreurs
        ui->ajouterButton_AddForm->setToolTip("Veuillez corriger les champs en rouge (survolez pour les détails).");
        return; // Important : arrêter ici
    }

    // --- Validation Passed ---
    // (Le code pour créer l'objet Client, appeler ajouter(), vérifier seuil zone, vider champs, rafraîchir liste reste le même)

    QString adresse = ui->adresseLineEdit->text().trimmed(); // Récupérer adresse ici si besoin pour l'objet Client
    Client client(CIN, nom, prenom, email, adresse, telephone, pays, zone);

    if (client.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès !"); // Garder un message de succès modal est souvent acceptable
        // ... check zone ...
        // ... clear form ... (resetAddClientFormStyles sera appelé au prochain clic)
        // ... refresh list ...
        ui->cinLineEdit->clear();
        ui->nomLineEdit->clear();
        ui->prenomLineEdit->clear();
        ui->emailLineEdit->clear();
        ui->adresseLineEdit->clear();
        ui->telephoneLineEdit->clear();
    } else {
        // Échec de l'ajout DB
        QString errorDetail = client.getLastError();
        QString userErrorMessage = "Erreur lors de l'ajout : ";

        if (errorDetail.contains("UNIQUE constraint failed", Qt::CaseInsensitive)) {
            userErrorMessage = "Le CIN '" + QString::number(CIN) + "' existe déjà.";
            ui->cinLineEdit->setStyleSheet(errorHighlightStyle);
            ui->cinLineEdit->setToolTip(userErrorMessage); // Mettre l'erreur spécifique sur le champ CIN
        } else {
            userErrorMessage += (errorDetail.isEmpty() ? "Erreur inconnue." : errorDetail);
            // Mettre un tooltip d'erreur générale sur le bouton Ajouter
            ui->ajouterButton_AddForm->setToolTip(userErrorMessage);
        }
        // On pourrait aussi afficher une QMessageBox ici pour les erreurs DB critiques
        QMessageBox::critical(this, "Erreur d'ajout", userErrorMessage); // Ou utiliser le tooltip du bouton
    }


}

void Widget::resetAddClientFormStyles() {
    // Reset style for LineEdits
    ui->cinLineEdit->setStyleSheet(""); // Remove specific stylesheet
    ui->nomLineEdit->setStyleSheet("");
    ui->prenomLineEdit->setStyleSheet("");
    ui->emailLineEdit->setStyleSheet("");
    ui->adresseLineEdit->setStyleSheet(""); // Adresse might be optional, adjust if needed
    ui->telephoneLineEdit->setStyleSheet("");

    // Reset style for ComboBoxes
    ui->zoneComboBox->setStyleSheet("");
    ui->paysComboBox->setStyleSheet("");

    // Alternative using dynamic properties (Requires setting up QSS in constructor or UI file)
    /*
    const QString defaultStyle = ""; // Or your default QLineEdit/QComboBox style
    ui->cinLineEdit->setProperty("invalid", false);
    ui->nomLineEdit->setProperty("invalid", false);
    ui->prenomLineEdit->setProperty("invalid", false);
    ui->emailLineEdit->setProperty("invalid", false);
    ui->adresseLineEdit->setProperty("invalid", false);
    ui->telephoneLineEdit->setProperty("invalid", false);
    ui->zoneComboBox->setProperty("invalid", false);
    ui->paysComboBox->setProperty("invalid", false);

    // Force style re-evaluation
    style()->unpolish(ui->cinLineEdit); style()->polish(ui->cinLineEdit);
    style()->unpolish(ui->nomLineEdit); style()->polish(ui->nomLineEdit);
    // ... etc for all fields ...
    */
}

void Widget::on_cancelButton_AddForm_clicked()
{
    ui->cinLineEdit->clear();
    ui->nomLineEdit->clear();
    ui->prenomLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->adresseLineEdit->clear();
    ui->telephoneLineEdit->clear();

}

void Widget::on_addButton_List_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_deleteButton_clicked()
{
    QItemSelectionModel* selection = ui->clientTableWidget->selectionModel();
    QModelIndexList selectedRows = selection->selectedRows();

    // Vérifier si aucune ligne n'est sélectionnée
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ou plusieurs lignes à supprimer.");
        return;
    }

    // Confirmation avant suppression
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation", QString("Voulez-vous vraiment supprimer %1 client(s) ?")
            .arg(selectedRows.count()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // Annule la suppression
    }

    // Variables pour suivre les résultats des suppressions
    int successCount = 0;
    int failureCount = 0;

    // Parcourir les lignes sélectionnées pour les supprimer
    for (int i = selectedRows.count() - 1; i >= 0; --i) {
        int row = selectedRows.at(i).row(); // Obtenir l'index de la ligne
        QTableWidgetItem* item = ui->clientTableWidget->item(row, 0); // Récupérer l'élément de la colonne ID
        // Vérifier que l'élément existe et récupérer l'ID
        if (!item) {
            QMessageBox::warning(this, "Erreur", "Impossible de récupérer l'ID de la ligne sélectionnée.");
            continue; // Ignorer cette ligne
        }
        int id = item->text().toInt(); // Convertir l'ID en entier
        Client c;

        // Tenter de supprimer le client de la base de données
        if (c.supprimer(id)) {
            ui->clientTableWidget->removeRow(row); // Supprimer la ligne du tableau
            successCount++;
        } else {
            failureCount++;
        }
    }

    // Affichage des résultats à l'utilisateur
    QMessageBox::information(this, "Résultat de la suppression",
                             QString("Suppression réussie pour %1 client(s).\nÉchec pour %2 client(s).")
                                 .arg(successCount).arg(failureCount));

    // Recharger les données pour synchroniser la table avec la base de données
    afficherClients();


}

void Widget::on_modifyButton_clicked()
{
    QModelIndexList selectedRows = ui->clientTableWidget->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un client.");
        return;
    }

    int row = selectedRows.first().row();
    int cin = ui->clientTableWidget->item(row, 0)->text().toInt();
    QString nom = ui->clientTableWidget->item(row, 1)->text();
    QString prenom = ui->clientTableWidget->item(row, 2)->text();
    QString email = ui->clientTableWidget->item(row, 3)->text();
    QString adresse = ui->clientTableWidget->item(row, 4)->text();
    QString telephone = ui->clientTableWidget->item(row, 5)->text();
     QString pays = ui->clientTableWidget->item(row, 7)->text();

    Dialog dialog(this);
    dialog.setClientData(cin, nom, prenom, email, adresse, telephone,pays);

    if (dialog.exec() == QDialog::Accepted) {
        ui->clientTableWidget->setItem(row, 1, new QTableWidgetItem(dialog.getNom()));
        ui->clientTableWidget->setItem(row, 2, new QTableWidgetItem(dialog.getPrenom()));
        ui->clientTableWidget->setItem(row, 3, new QTableWidgetItem(dialog.getEmail()));
        ui->clientTableWidget->setItem(row, 4, new QTableWidgetItem(dialog.getAdresse()));
        ui->clientTableWidget->setItem(row, 5, new QTableWidgetItem(dialog.getTelephone()));
        ui->clientTableWidget->setItem(row, 7, new QTableWidgetItem(dialog.getPays()));
    }
}

void Widget::on_searchLineEdit_textChanged(const QString &text)
{
    QString searchText = text.toLower();  // Convertir le texte de recherche en minuscules pour une comparaison insensible à la casse

    // Récupérer l'index de la colonne à filtrer selon le comboBox
    int columnToSearch = 0;  // Par défaut, chercher dans la colonne "Nom" (colonne 1)

    if (ui->comboBox_SortBy->currentText() == "Recherche par: Nom") {
        columnToSearch = 1; // Chercher dans la colonne "Nom"
    } else if (ui->comboBox_SortBy->currentText() == "Recherche par: CIN") {
        columnToSearch = 0; // Chercher dans la colonne "CIN"
    } else if (ui->comboBox_SortBy->currentText() == "Recherche par: Prénom") {
        columnToSearch = 2; // Chercher dans la colonne "Prénom"
    }

    // Parcourir toutes les lignes de la table pour appliquer le filtre
    for (int row = 0; row < ui->clientTableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = ui->clientTableWidget->item(row, columnToSearch); // Récupérer l'élément de la colonne sélectionnée

        if (item) {
            // Vérifiez si le texte dans l'élément contient le texte de recherche
            if (item->text().toLower().contains(searchText)) {
                ui->clientTableWidget->setRowHidden(row, false); // Affiche la ligne si elle correspond
            } else {
                ui->clientTableWidget->setRowHidden(row, true);  // Cache la ligne si elle ne correspond pas
            }
        }
    }
}

void Widget::on_functionalityMenuButton_clicked()
{QMenu *menu = new QMenu(this);

    QAction *trierParNomAction = new QAction("Trier par nom 🔤", this);
    QAction *trierParPrenomAction = new QAction("Trier par prénom 🔤", this);
    QAction *exporterPdf = new QAction("Exporter en PDF 📄", this);
    QAction *reinitialiserTri = new QAction("Réinitialiser le tri 🔄", this);

    connect(trierParNomAction, &QAction::triggered, this, [this]() {
        afficherClients("nom", "ASC");
    });

    connect(trierParPrenomAction, &QAction::triggered, this, [this]() {
        afficherClients("prenom", "ASC");
    });

    connect(exporterPdf, &QAction::triggered, this, &Widget::exporterClientsEnPDF);

    connect(reinitialiserTri, &QAction::triggered, this, [this]() {
        afficherClients("CIN", "ASC");
    });

    menu->addAction(trierParNomAction);
    menu->addAction(trierParPrenomAction);
    menu->addAction(exporterPdf);
    menu->addAction(reinitialiserTri);

    menu->exec(QCursor::pos());
}

void Widget::exporterClientsEnPDF()
{
    // 1. Prompt user for save location and filename
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter la liste des clients en PDF",
        "",
        "Fichiers PDF (*.pdf);;Tous les fichiers (*)"
        );

    // 2. Handle cancellation
    if (fileName.isEmpty()) {
        return;
    }

    // 3. Ensure the filename has the .pdf extension
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // 4. Prepare the document content (HTML format)
    QTextDocument document;
    QString contenuPdf;
    QDateTime generationTime = QDateTime::currentDateTime(); // Get current time

    // --- Add Document Header (for formality) ---
    contenuPdf.append("<p style='text-align: left; font-size: 10pt; color: #666;'>Rapport Interne</p>");
    contenuPdf.append("<hr style='border: none; border-top: 1px solid #ccc;'>"); // Simple line separator

    // --- Add Main Title ---
    contenuPdf.append("<h1 style='text-align: center; color: #1A5276; font-family: Arial, sans-serif;'>Liste des Clients</h1>");
    contenuPdf.append("<p style='text-align: center; font-size: 9pt; color: #555;'>Données extraites le : " + generationTime.toString("dd/MM/yyyy") + "</p><br/>"); // Date below title

    // --- Add CSS Styles for the Table ---
    contenuPdf.append("<style>"
                      "body { font-family: Arial, sans-serif; font-size: 10pt; }" // Base font for the document
                      "table {"
                      "  border-collapse: collapse;"
                      "  width: 100%;"
                      "  margin-top: 15px;" // Add some space above the table
                      "}"
                      "th, td {"
                      "  border: 1px solid #B0B0B0;" // Slightly darker border for better visibility
                      "  padding: 6px 8px;"        // Adjust padding (vertical, horizontal)
                      "  text-align: left;"
                      "  vertical-align: top;"      // Align text to the top of the cell
                      "  word-wrap: break-word;"   // *** Crucial for text wrapping ***
                      "  overflow-wrap: break-word;" // Alternate property, good for compatibility
                      "}"
                      "th {"
                      "  background-color: #D5D5D5;" // Lighter grey header
                      "  color: #222222;"          // Darker header text
                      "  font-weight: bold;"
                      "  text-align: center;"       // Center-align header text
                      "}"
                      "tr:nth-child(even) {"
                      "  background-color: #F2F2F2;" // Subtle zebra striping
                      "}"
                      "tr:nth-child(odd) {"
                      "  background-color: #FFFFFF;"
                      "}"
                      ".footer {"                  // Style for the footer text
                      "  margin-top: 20px;"
                      "  text-align: right;"
                      "  font-size: 9pt;"
                      "  font-style: italic;"
                      "  color: #666;"
                      "}"
                      "</style>");

    // --- Create Table Header Row ---
    contenuPdf.append("<table>");
    contenuPdf.append("<thead>"); // Use thead for proper structure
    contenuPdf.append("<tr>");
    // Add column headers - adjust based on your table columns
    contenuPdf.append("<th>CIN</th>");
    contenuPdf.append("<th>Nom</th>");
    contenuPdf.append("<th>Prénom</th>");
    contenuPdf.append("<th>Email</th>");
    contenuPdf.append("<th>Adresse</th>");
    contenuPdf.append("<th>Téléphone</th>");
    contenuPdf.append("<th>Date d'inscription</th>");
    contenuPdf.append("<th>Pays</th>");
    contenuPdf.append("</tr>");
    contenuPdf.append("</thead>");

    // --- Create Table Body with Data ---
    contenuPdf.append("<tbody>"); // Use tbody for proper structure
    int exportedRowCount = 0; // Count rows actually exported
    for (int row = 0; row < ui->clientTableWidget->rowCount(); ++row) {
        // Skip rows that are currently hidden by filtering
        if (ui->clientTableWidget->isRowHidden(row)) {
            continue;
        }
        exportedRowCount++; // Increment count for visible rows

        contenuPdf.append("<tr>");
        // Iterate through all visible columns in your table (adjust the limit '8' if needed)
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem *item = ui->clientTableWidget->item(row, col);
            // Use item->text() if item exists, otherwise use an empty string. Escape HTML chars.
            contenuPdf.append(QString("<td>%1</td>").arg(item ? item->text().toHtmlEscaped() : " ")); // Use   for empty cells for better border rendering
        }
        contenuPdf.append("</tr>");
    }
    contenuPdf.append("</tbody>");
    contenuPdf.append("</table>");

    // Add a summary line after the table
    contenuPdf.append(QString("<p style='font-size: 9pt; margin-top: 10px;'>Total de %1 clients listés.</p>").arg(exportedRowCount));


    // --- Add Document Footer (Generation Time) ---
    contenuPdf.append("<p class='footer'>Document généré le : " + generationTime.toString("dd/MM/yyyy 'à' HH:mm:ss") + "</p>");


    // 5. Set the HTML content to the document
    document.setHtml(contenuPdf);

    // 6. Prepare the PDF writer
    QPdfWriter pdfWriter(fileName);

    // --- Configure PDF metadata and layout ---
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Portrait);
    pdfWriter.setPageMargins(QMarginsF(15, 20, 15, 20)); // Increased top/bottom margins slightly
    pdfWriter.setTitle("Liste des Clients - Rapport Interne"); // More specific title
    pdfWriter.setCreator("Gestion Client App"); // Use your app's name

    // 7. Print the document content to the PDF writer
    document.print(&pdfWriter);

    // --- PDF Writing is Finished (or attempted) ---

    // 8. Check if the file was created successfully and add notification
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists() && fileInfo.size() > 0) {
        QString notificationMessage = QString("✅ Exportation PDF ('%1') terminée. %2 clients listés.").arg(fileInfo.fileName()).arg(exportedRowCount);
        this->afficherNotification(notificationMessage);

        // 9. Attempt to open the generated PDF file
        bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

        // 10. Provide final feedback to the user via QMessageBox
        if (opened) {
            QMessageBox::information(this, "Exportation réussie",
                                     QString("La liste de %1 clients a été exportée en PDF :\n'%2'\n\nLe fichier a été ouvert.").arg(exportedRowCount).arg(fileInfo.fileName()));
        } else {
            QMessageBox::warning(this, "Exportation terminée",
                                 QString("La liste de %1 clients a été exportée en PDF :\n'%2'\n\nImpossible d'ouvrir automatiquement le fichier.").arg(exportedRowCount).arg(fileInfo.fileName()));
        }
    } else {
        // Error message if file wasn't created properly
        QString errorMsg = QString("Échec de l'exportation PDF vers :\n'%1'\n\nVérifiez les permissions d'écriture ou l'espace disque.").arg(fileName);
        QMessageBox::critical(this, "Erreur d'exportation", errorMsg);
        this->afficherNotification(QString("🛑 Échec de l'exportation PDF vers '%1'.").arg(fileInfo.fileName())); // Add failure notification
    }

}
void Widget::afficherTimelineClients()
{
    QString selectedRegion = ui->comboBoxchoisi->currentText();

    QSqlQuery query;
    QString queryStr = "SELECT pays, COUNT(*) as total FROM client";

    // Ajouter filtre selon la région
    if (selectedRegion == "Asie") {
        queryStr += " WHERE pays IN ('Palestine', 'Chine', 'Inde')";
    } else if (selectedRegion == "Afrique") {
        queryStr += " WHERE pays IN ('Tunisie', 'Algérie', 'Maroc')";
    } else if (selectedRegion == "Europe") {
        queryStr += " WHERE pays IN ('Espagne', 'France', 'Allemagne')";
    }

    queryStr += " GROUP BY pays";  // Groupement par pays

    query.prepare(queryStr);

    QBarSeries *series = new QBarSeries();

    QString countryWithMaxAvg;
    double maxAvg = 0;

    if (query.exec()) {
        QMap<QString, int> totalClientsByCountry;
        QList<QPair<QString, int>> sortedCountries;
        QStringList paysList;

        int totalClients = 0;
        int totalCountries = 0;

        while (query.next()) {
            QString country = query.value("pays").toString();
            int countryTotal = query.value("total").toInt();

            totalClientsByCountry[country] = countryTotal;

            totalClients += countryTotal;
            totalCountries++;
        }

        // Calcul de la moyenne globale
        double globalAvg = totalClients / static_cast<double>(totalCountries);

        // Remplir la liste pour tri
        for (auto it = totalClientsByCountry.begin(); it != totalClientsByCountry.end(); ++it) {
            sortedCountries.append(qMakePair(it.key(), it.value()));
        }

        // Trier par nombre décroissant
        std::sort(sortedCountries.begin(), sortedCountries.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
            return a.second > b.second;
        });

        // Trouver le pays avec la plus grande moyenne
        for (const auto &countryData : sortedCountries) {
            double avg = static_cast<double>(countryData.second) / globalAvg;
            if (avg > maxAvg) {
                maxAvg = avg;
                countryWithMaxAvg = countryData.first;
            }
        }

        // Créer un label pour le pays avec la meilleure moyenne
        QString labelMaxAvgText = QString("Le pays avec la moyenne la plus élevée est %1 avec un ratio de %.2f.")
                                      .arg(countryWithMaxAvg)
                                      .arg(maxAvg);
        QLabel *resultMaxAvgLabel = new QLabel(labelMaxAvgText);
        resultMaxAvgLabel->setAlignment(Qt::AlignCenter);
        resultMaxAvgLabel->setStyleSheet(
            "font-size: 18px; "
            "color: black; "
            "font-weight: bold; "
            "padding: 5px 10px; "
            "border-radius: 5px; "
            "background-color: #F0F8FF;");

        // Layout principal
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->addWidget(resultMaxAvgLabel);

        // Remplir le QBarSet
        QBarSet *set = new QBarSet("Clients par pays");
        for (const auto &countryData : sortedCountries) {
            set->append(countryData.second);
            paysList.append(countryData.first);
        }
        series->append(set);

        // Créer le graphique
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Nombre de clients par pays");
        chart->legend()->setAlignment(Qt::AlignBottom);

        // Axes
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(paysList);  // Utilise la liste triée des pays
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Chart view
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        mainLayout->addWidget(chartView);

        // Nettoyer ancien layout de chartFrame
        QLayout *oldLayout = ui->chartFrame->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

        // Appliquer le nouveau layout
        ui->chartFrame->setLayout(mainLayout);
    }
}


// Connecter le signal de changement de sélection du comboBox à la fonction
void Widget::setupComboBoxConnection()
{
    connect(ui->comboBoxchoisi, SIGNAL(currentIndexChanged(int)), this, SLOT(onRegionChanged(int)));

}




#include <QDateTime> // ➔ Important pour récupérer date/heure



// In widget.cpp

void Widget::on_button_ClearNotifications_clicked()
{
    // Use a QSet to automatically handle uniqueness:
    // An item might be both checked AND selected. We only want to delete it once.
    QSet<QListWidgetItem*> itemsToDeleteSet;

    // 1. Add CHECKED items to the set
    for (int i = 0; i < ui->notificationsListWidget->count(); ++i) {
        QListWidgetItem* currentItem = ui->notificationsListWidget->item(i);
        if (currentItem && currentItem->checkState() == Qt::Checked) {
            itemsToDeleteSet.insert(currentItem); // Add checked item to the set
        }
    }

    // 2. Add SELECTED items to the set
    QList<QListWidgetItem*> selectedItems = ui->notificationsListWidget->selectedItems();
    for (QListWidgetItem* item : selectedItems) {
        // The set automatically handles duplicates if an item was already added because it was checked
        itemsToDeleteSet.insert(item);
    }

    // 3. Check if any items were marked for deletion (either checked or selected)
    if (itemsToDeleteSet.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucune notification n'est cochée ou sélectionnée pour la suppression.");
        return; // Nothing to do
    }

    // 4. Ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Voulez-vous vraiment supprimer %1 notification(s) (cochée(s) ou sélectionnée(s)) ?")
                                      .arg(itemsToDeleteSet.count()), // Count unique items in the set
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // User cancelled
    }

    // 5. Iterate through the unique set of items marked for deletion and remove them
    int deletedCount = 0;
    // Iterating over the set is safe for removing items from the list widget
    for (QListWidgetItem* item : itemsToDeleteSet) {
        // Find the row of the item *just before* removing it
        int row = ui->notificationsListWidget->row(item);
        if (row != -1) { // Make sure it's still validly in the list
            ui->notificationsListWidget->takeItem(row); // Remove from the list widget's model
            delete item; // Free the memory associated with the QListWidgetItem
            deletedCount++;
        }
        // Note: Don't else { delete item; } here. If row == -1, it means the item
        // was likely already removed (e.g., if it was somehow in the set twice
        // due to an error elsewhere, though QSet should prevent this).
        // Or it means the item pointer is somehow invalid.
    }

    // 6. Update the persistent storage (JSON file) to reflect the changes
    sauvegarderNotifications(); // Save the remaining items

    if (deletedCount > 0) {
        QMessageBox::information(this, "Succès", QString("%1 notification(s) supprimée(s).").arg(deletedCount));
    }
    // Optional: Add a message if some items couldn't be deleted (row == -1)
    if (deletedCount < itemsToDeleteSet.count()) {
        QMessageBox::warning(this, "Avertissement", "Certaines notifications n'ont pas pu être supprimées (elles n'étaient peut-être plus dans la liste).");
    }
}

void Widget::on_consulterbtn_clicked()
{
    // Vérifier si une ligne est sélectionnée
    QModelIndex selectedIndex = ui->clientTableWidget->currentIndex();
    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un client dans la liste !");
        return;
    }

    // Récupérer les informations du client à partir de la table "client"
    int row = selectedIndex.row();
    QString CIN = ui->clientTableWidget->item(row, 0)->text();
    QString nom = ui->clientTableWidget->item(row, 1)->text();
    QString prenom = ui->clientTableWidget->item(row, 2)->text();
    QString email = ui->clientTableWidget->item(row, 3)->text();
    QString adresse = ui->clientTableWidget->item(row, 4)->text();
    QString telephone = ui->clientTableWidget->item(row, 5)->text();
    QString dateInscription = ui->clientTableWidget->item(row, 6)->text();
    QString pays = ui->clientTableWidget->item(row, 7)->text();

    // Créer une nouvelle QDialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Informations du client");
    dialog->resize(400, 300); // Définir la taille de la boîte de dialogue

    // Créer une mise en page
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Ajouter les informations formatées
    QLabel *label = new QLabel(QString(
                                   "<div style='text-align:center;'>"
                                   "<h3 style='color:# 5dade2;'>Informations du client :</h3>"
                                   "<p><b>CIN :</b> %1</p>"
                                   "<p><b>Nom :</b> %2</p>"
                                   "<p><b>Prénom :</b> %3</p>"
                                   "<p><b>Email :</b> %4</p>"
                                   "<p><b>Adresse :</b> %5</p>"
                                   "<p><b>Téléphone :</b> %6</p>"
                                   "<p><b>Date d'inscription :</b> %7</p>"
                                   "<p><b>Pays :</b> %8</p>"
                                   "</div>"
                                   ).arg(CIN).arg(nom).arg(prenom).arg(email).arg(adresse).arg(telephone).arg(dateInscription).arg(pays));
    label->setTextInteractionFlags(Qt::TextSelectableByMouse); // Rendre le texte sélectionnable
    layout->addWidget(label);


    // Appliquer la mise en page à la boîte de dialogue
    dialog->setLayout(layout);

    // Afficher la boîte de dialogue
    dialog->exec();
}


void Widget::on_button_ReportRisk_clicked()
{

    QString typeRisque = ui->comboBox_RiskType->currentText();
    QString zone = ui->comboBox_RiskZone->currentText();
    QDateTime heureSignalee = QDateTime::currentDateTime();

    if (zone.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "La zone doit être spécifiée.");
        return;
    }

    Risque risque(0, typeRisque, zone, heureSignalee, "Signalé");

    if (risque.ajouter()) {
        QMessageBox::information(this, "Succès", "Le risque a été signalé avec succès !");

        Client client;
        QSqlQuery query = client.clientsDansZone(zone);

        ui->tableWidget_RiskClients->setRowCount(0);
        ui->tableWidget_RiskClients->setColumnCount(5);
        ui->tableWidget_RiskClients->setHorizontalHeaderLabels(QStringList() << "Nom" << "Téléphone" << "Zone" << "Action" << "Statut");

        // Étendre les colonnes automatiquement
        ui->tableWidget_RiskClients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        int row = 0;
        while (query.next()) {
            ui->tableWidget_RiskClients->insertRow(row);

            ui->tableWidget_RiskClients->setItem(row, 0, new QTableWidgetItem(query.value("nom").toString()));
            ui->tableWidget_RiskClients->setItem(row, 1, new QTableWidgetItem(query.value("telephone").toString()));
            ui->tableWidget_RiskClients->setItem(row, 2, new QTableWidgetItem(query.value("zone").toString()));
            ui->tableWidget_RiskClients->setItem(row, 4, new QTableWidgetItem("🟥En danger"));

            // Bouton "Assister"
            QPushButton *btnAider = new QPushButton("Assister");
            btnAider->setMinimumWidth(100);
            btnAider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            QWidget *btnWidget = new QWidget();
            QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
            btnLayout->addStretch();
            btnLayout->addWidget(btnAider);
            btnLayout->addStretch();
            btnLayout->setContentsMargins(0, 0, 0, 0);

            ui->tableWidget_RiskClients->setCellWidget(row, 3, btnWidget);

            int currentRow = row;
            int CINClient = query.value("CIN").toInt();

            connect(btnAider, &QPushButton::clicked, this, [=]() {
                // Statut "En cours d'évacuation"
                ui->tableWidget_RiskClients->item(currentRow, 4)->setText("🟨En cours d'évacuation");
                client.mettreAJourStatut(CINClient, "🟨En cours d'évacuation");

                // Ouvrir la fenêtre d'évacuation
                Evacuation evacDialog(this);
                evacDialog.setModal(true);

                // Quand la fenêtre est validée, on passe à "Évacué"
                if (evacDialog.exec() == QDialog::Accepted) {
                    ui->tableWidget_RiskClients->item(currentRow, 4)->setText("🟩Evacué");
                    client.mettreAJourStatut(CINClient, "🟩Evacué");

                    // Supprimer la ligne après 3 secondes
                    QTimer::singleShot(3000, this, [=]() {
                        if (currentRow < ui->tableWidget_RiskClients->rowCount()) {
                            ui->tableWidget_RiskClients->removeRow(currentRow);

                            if (ui->tableWidget_RiskClients->rowCount() == 0) {
                                QMessageBox::information(this, "Risque", "Tous les clients ont été évacués.");
                            }
                        }
                    });
                }
            });

            row++;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de signaler le risque.");
    }


}

