#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Client {
private:
    int CIN;
    QString nom;
    QString prenom;
    QString email;
    QString adresse;
    QString telephone;
    QString pays;
    QString zone;      // Zone du client
    QString statut;    // Statut du client
    QString lastError; // Private variable to store the last error message

public:
    // Constructeur par défaut
    Client();

    // Constructeur avec paramètres (ajout d'un statut et zone optionnels)
    Client(int CIN, QString nom, QString prenom, QString email, QString adresse, QString telephone, QString pays, QString zone = "", QString statut = "OK");

    // Getters
    int getCIN();
    QString getNom();

    QString getEmail();
    QString getAdresse();
    QString getTelephone();
    QString getPrenom();
    QString getPays();
    QString getStatut();
    QString getZone();
    QString getLastError() const; // Public method to retrieve the last error

    // Setters
    void setCIN(int CIN);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setEmail(const QString &email);
    void setAdresse(const QString &adresse);
    void setTelephone(const QString &telephone);
    void setPays(const QString &pays);
    void setStatut(const QString &statut);
    void setZone(const QString &zone);


    // Méthodes pour interagir avec la base de données
    bool ajouter();
    QSqlQueryModel* afficher(); // Read
    bool supprimer(int CIN);
    bool modifier();
    QSqlQuery clientsDansZone(QString zone); // Méthode pour récupérer les clients dans une zone

    // Méthode pour mettre à jour le statut d'un client
    bool mettreAJourStatut(int CIN, const QString &nouveauStatut) const; // Ajout du mot-clé 'const' ici
};

#endif // CLIENT_H
