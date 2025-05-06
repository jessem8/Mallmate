#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    // Méthode pour pré-remplir les données du client dans le formulaire
    void setClientData(int cin, const QString &nom, const QString &prenom, const QString &email, const QString &adresse, const QString &telephone,const QString &pays);

    // Méthodes pour récupérer les données modifiées
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getAdresse() const;
    QString getTelephone() const;
    QString getPays() const;
    int getCIN() const; // Récupérer le CIN

private slots:
    void on_ajouterButton_AddForm_clicked(); // Slot pour gérer le clic du bouton "Valider"

private:
    Ui::Dialog *ui;
    int clientCIN; // Stocke le CIN du client pour modification
};

#endif // DIALOG_H
