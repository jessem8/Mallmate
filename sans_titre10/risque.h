#ifndef RISQUE_H
#define RISQUE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>  // Pour la gestion de la date et de l'heure

class Risque {
private:
    int idRisque;
    QString typeRisque;
    QString zone;
    QDateTime heureSignalee;  // Utilisation de QDateTime pour l'heure
    QString statut;

public:
    Risque();
    Risque(int idRisque, QString typeRisque, QString zone, QDateTime heureSignalee, QString statut = "Signalé");

    int getIdRisque() const;
    QString getTypeRisque() const;
    QString getZone() const;
    QDateTime getHeureSignalee() const;
    QString getStatut() const;

    void setIdRisque(int idRisque);
    void setTypeRisque(const QString &typeRisque);
    void setZone(const QString &zone);
    void setHeureSignalee(const QDateTime &heureSignalee);
    void setStatut(const QString &statut);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int idRisque);
    bool modifier();
};

#endif // RISQUE_H
