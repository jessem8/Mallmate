#include "evacuation.h"
#include "ui_evacuation.h"

Evacuation::Evacuation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Evacuation)
{
    ui->setupUi(this);

    stepLayout = new QVBoxLayout();
    ui->pathFrame->setLayout(stepLayout); // On garde un seul layout

    connect(ui->addStepButton, &QPushButton::clicked, this, &Evacuation::onAddStepClicked);
    connect(ui->undoStepButton, &QPushButton::clicked, this, &Evacuation::undoStep);
    connect(ui->btnvalider, &QPushButton::clicked, this, &QDialog::accept);


}

Evacuation::~Evacuation()
{
    delete ui;
}

#include <QInputDialog> // pour QInputDialog

void Evacuation::onAddStepClicked() {
    QString selectedStep = ui->pathComboBox->currentText();

    if (selectedStep == "Other") {
        bool ok;
        selectedStep = QInputDialog::getText(this, "Ajouter une étape personnalisée",
                                             "Entrez l'étape :", QLineEdit::Normal,
                                             "", &ok);
        if (!ok || selectedStep.trimmed().isEmpty()) {
            return; // utilisateur a annulé ou rien saisi
        }
    }

    QLabel *stepLabel = new QLabel(selectedStep);
    stepLabel->setStyleSheet("font-size: 16px; margin: 3px;");

    stepLayout->addWidget(stepLabel);
    stepsList.append(stepLabel);
}


void Evacuation::undoStep() {
    if (!stepsList.isEmpty()) {
        QLabel *last = stepsList.takeLast();
        stepLayout->removeWidget(last);
        last->deleteLater();
    }
}
