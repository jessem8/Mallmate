#include "Client.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QSqlError>  // Ajoutez cette ligne pour inclure QSqlError
#include <QDebug>      // Assurez-vous que QDebug est inclus pour afficher les erreurs

Client::Client() {}

Client::Client(int CIN, QString nom, QString prenom, QString email, QString adresse, QString telephone, QString pays, QString zone, QString statut)
    : CIN(CIN), nom(nom), prenom(prenom), email(email), adresse(adresse), telephone(telephone), pays(pays), zone(zone), statut(statut.isEmpty() ? "OK" : statut) {}

int Client::getCIN() { return CIN; }
QString Client::getNom() { return nom; }
QString Client::getEmail() { return email; }
QString Client::getAdresse() { return adresse; }
QString Client::getTelephone() { return telephone; }
QString Client::getPrenom() { return prenom; }
QString Client::getPays() { return pays; }
QString Client::getStatut() { return statut; }
QString Client::getZone() { return zone; }

void Client::setCIN(int CIN) {
    this->CIN = CIN;
}
QString Client::getLastError() const
{
    return lastError;
}
void Client::setNom(const QString &nom) {
    this->nom = nom;
}

void Client::setPrenom(const QString &prenom) {
    this->prenom = prenom;
}

void Client::setEmail(const QString &email) {
    this->email = email;
}

void Client::setAdresse(const QString &adresse) {
    this->adresse = adresse;
}

void Client::setTelephone(const QString &telephone) {
    this->telephone = telephone;
}

void Client::setPays(const QString &pays) {
    this->pays = pays;
}

void Client::setStatut(const QString &statut) {
    this->statut = statut;
}

void Client::setZone(const QString &zone) {
    this->zone = zone;
}

bool Client::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO client (CIN, nom, prenom, email, adresse, telephone, pays, zone, statut) "
                  "VALUES (:CIN, :nom, :prenom, :email, :adresse, :telephone, :pays, :zone, :statut)");
    query.bindValue(":CIN", CIN);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone", telephone);
    query.bindValue(":pays", pays);
    query.bindValue(":zone", zone);
    query.bindValue(":statut", statut); // Ajout du statut dynamique

    return query.exec();
}

QSqlQueryModel* Client::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM client");
    return model;
}

bool Client::supprimer(int CIN) {
    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE CIN = :CIN");
    query.bindValue(":CIN", CIN);
    return query.exec();
}

bool Client::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE client SET nom = :nom, prenom = :prenom, email = :email, adresse = :adresse, telephone = :telephone, pays=:pays WHERE CIN = :CIN");
    query.bindValue(":CIN", CIN);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone", telephone);
    query.bindValue(":pays", pays);

    return query.exec();
}
QSqlQuery Client::clientsDansZone(QString zone) {
    QSqlQuery query;
    query.prepare("SELECT nom, telephone, zone FROM client WHERE zone = :zone");
    query.bindValue(":zone", zone);
    query.exec();
    return query;
}
bool Client::mettreAJourStatut(int CIN, const QString &nouveauStatut) const {
    QSqlQuery query;

    // Préparer la requête SQL pour mettre à jour le statut
    query.prepare("UPDATE client SET statut = :statut WHERE CIN = :CIN");

    // Lier les valeurs aux paramètres dans la requête
    query.bindValue(":statut", nouveauStatut);
    query.bindValue(":CIN", CIN);

    // Exécuter la requête et vérifier si elle a réussi
    if (query.exec()) {
        return true;  // La mise à jour a réussi
    } else {
        // Si une erreur se produit, vous pouvez l'afficher ou la consigner pour le débogage
        qDebug() << "Erreur lors de la mise à jour du statut : " << query.lastError().text();
        return false;  // La mise à jour a échoué
    }
}
