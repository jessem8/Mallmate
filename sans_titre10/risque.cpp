#include "risque.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDateTime>

// Constructeur par défaut
Risque::Risque() {
    idRisque = 0;
    typeRisque = "";
    zone = "";
    heureSignalee = QDateTime::currentDateTime();  // Par défaut, l'heure actuelle
    statut = "Signalé";  // Valeur par défaut
}

// Constructeur avec paramètres
Risque::Risque(int idRisque, QString typeRisque, QString zone, QDateTime heureSignalee, QString statut) {
    this->idRisque = idRisque;
    this->typeRisque = typeRisque;
    this->zone = zone;
    this->heureSignalee = heureSignalee;
    this->statut = statut;
}

// Méthode pour ajouter un risque à la base de données
bool Risque::ajouter() {
    QSqlQuery query;


    query.prepare("INSERT INTO risques (type_risque, zone_concernee, heure_signalee, statut_risque) "
                  "VALUES (:typeRisque, :zone, :heureSignalee, :statut)");
    query.bindValue(":typeRisque", typeRisque);
    query.bindValue(":zone", zone);
    query.bindValue(":heureSignalee", heureSignalee);
    query.bindValue(":statut", statut);


    return query.exec();
}
