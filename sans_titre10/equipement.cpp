#include "equipement.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QSqlError>

// Constructeurs
Equipement::Equipement() {}
Equipement::Equipement(int id, QString nom, QString statut, QString disponibilite, float humidite, float temperature)
    : idEquipement(id), nomEquipement(nom), statutEquipement(statut), disponibiliteEquipement(disponibilite),
    humiditeSurface(humidite), temperatureStade(temperature) {}

// Getters
int Equipement::getIdEquipement() { return idEquipement; }
QString Equipement::getNomEquipement() { return nomEquipement; }
QString Equipement::getStatutEquipement() { return statutEquipement; }
QString Equipement::getDisponibiliteEquipement() { return disponibiliteEquipement; }
float Equipement::getHumiditeSurface() { return humiditeSurface; }
float Equipement::getTemperatureStade() { return temperatureStade; }

// Méthode ajouter
bool Equipement::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO equipement (idEquipement, nomEquipement, statutEquipement, disponibiliteEquipement, humiditeSurface, temperatureStade) "
                  "VALUES (:idEquipement, :nomEquipement, :statutEquipement, :disponibiliteEquipement, :humiditeSurface, :temperatureStade)");
    query.bindValue(":idEquipement", idEquipement);
    query.bindValue(":nomEquipement", nomEquipement);
    query.bindValue(":statutEquipement", statutEquipement);
    query.bindValue(":disponibiliteEquipement", disponibiliteEquipement);
    query.bindValue(":humiditeSurface", humiditeSurface);
    query.bindValue(":temperatureStade", temperatureStade);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return false;
    }
    return true;
}
QSqlQueryModel* Equipement::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM equipement");
    return model; }
