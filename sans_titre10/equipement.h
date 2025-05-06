#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Equipement {
private:
    int idEquipement;               // Identifiant unique de l'équipement
    QString nomEquipement;          // Nom de l'équipement
    QString statutEquipement;       // Statut de l'équipement
    QString disponibiliteEquipement; // Disponibilité de l'équipement
    float humiditeSurface;          // Humidité de la surface
    float temperatureStade;         // Température de l'équipement

public:
    Equipement();
    Equipement(int, QString, QString, QString, float, float);

    int getIdEquipement();
    QString getNomEquipement();
    QString getStatutEquipement();
    QString getDisponibiliteEquipement();
    float getHumiditeSurface();
    float getTemperatureStade();

    bool ajouter(); // Create
    QSqlQueryModel* afficher(); // Read

};
#endif // EQUIPEMENT_H
