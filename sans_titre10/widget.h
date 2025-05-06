#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QSoundEffect>
#include <QSystemTrayIcon>
#include <QIcon> // Needed for setting an icon for the tray message
#include <QSystemTrayIcon> // Also include here for the declaration

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_sidebarListWidget_itemClicked(QListWidgetItem *item);
    void afficherClients(QString colonneTri = "", QString ordre = "ASC");
    void on_ajouterButton_AddForm_clicked();
    void on_cancelButton_AddForm_clicked();
    void on_addButton_List_clicked();
    void on_deleteButton_clicked();
    void on_modifyButton_clicked();
    void setupComboBoxConnection();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void exporterClientsEnPDF();
    void on_functionalityMenuButton_clicked();
    void afficherTimelineClients();
    void resetAddClientFormStyles(); // Helper function to reset field styles
    void afficherNotification(const QString &message);  // Affiche bulle + ajoute à la liste
    void gererClicSidebar(QListWidgetItem *item);
    void on_button_ClearNotifications_clicked();
    void  chargerNotifications();  // Charger les notifications dès que le widget est chargé
    void sauvegarderNotifications();
    void on_consulterbtn_clicked();

    void on_button_ReportRisk_clicked();

private:
    Ui::Widget *ui;
     QSystemTrayIcon *trayIcon; // ✅ Icône pour les notifications
    int nombreNotificationsNonLues = 0;
     QString m_lastSortColumn = "";
     QString m_lastSortOrder = "ASC";

};
#endif // WIDGET_H
