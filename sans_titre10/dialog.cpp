#include "dialog.h"
#include "ui_dialog.h"
#include "client.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setClientData(int cin, const QString &nom, const QString &prenom, const QString &email, const QString &adresse, const QString &telephone,const QString &pays)
{
    clientCIN = cin; // Stocke le CIN du client pour modification
    ui->cinLineEdit->setText(QString::number(clientCIN)); //
    ui->cinLineEdit->setCursor(Qt::ForbiddenCursor);
    ui->cinLineEdit->setToolTip("Le champ CIN ne peut pas être modifié."); // Info-bulle
    ui->nomLineEdit->setText(nom);
    ui->prenomLineEdit->setText(prenom);
    ui->emailLineEdit->setText(email);
    ui->adresseLineEdit->setText(adresse);
     ui->telephoneLineEdit->setText(telephone);
    ui->paysComboBox->setCurrentText(pays);
}

QString Dialog::getNom() const { return ui->nomLineEdit->text(); }
QString Dialog::getPrenom() const { return ui->prenomLineEdit->text(); }
QString Dialog::getEmail() const { return ui->emailLineEdit->text(); }
QString Dialog::getAdresse() const { return ui->adresseLineEdit->text(); }
QString Dialog::getTelephone() const { return ui->telephoneLineEdit->text(); }
QString Dialog::getPays() const { return ui->paysComboBox->currentText(); }
int Dialog::getCIN() const { return clientCIN; }

void Dialog::on_ajouterButton_AddForm_clicked()
{
    QString nom = getNom();
    QString prenom = getPrenom();
    QString email = getEmail();
    QString adresse = getAdresse();
    QString telephone = getTelephone();
    QString pays = getPays();

    qDebug() << "Données récupérées : CIN =" << clientCIN
             << ", Nom =" << nom
             << ", Prenom =" << prenom
             << ", Email =" << email
             << ", Adresse =" << adresse
             << ", Telephone =" << telephone
             << ", Pays =" << pays;

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || adresse.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    Client client;
    client.setCIN(clientCIN);
    client.setNom(nom);
    client.setPrenom(prenom);
    client.setEmail(email);
    client.setAdresse(adresse);
    client.setTelephone(telephone);
    client.setPays(pays);

    if (client.modifier()) {
        QMessageBox::information(this, "Succès", "Les informations du client ont été modifiées avec succès !");
        accept(); // Ferme le dialogue
    } else {
        qDebug() << "Échec de la modification dans la base de données.";
        QMessageBox::critical(this, "Erreur", "Échec de la modification du client dans la base de données.");
    }
}
