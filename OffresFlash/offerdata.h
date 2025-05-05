#ifndef OFFERDATA_H
#define OFFERDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QtDebug>

// Define the structure to hold data for one flash offer,
// matching the Oracle database schema.
struct OfferData {
    // --- Members matching Oracle Table OFFERS ---
    int       offreId = -1;             // Corresponds to OffreID NUMBER PRIMARY KEY
    QString   description;              // Corresponds to Description CLOB
    double    percentageReduction = 0.0;// Corresponds to PourcentageReduction NUMBER(5, 2)
    double    prixSpecial = 0.0;        // Corresponds to PrixSpecial NUMBER(10, 2)
    int       quantiteLimitee = 0;      // Corresponds to QuantiteLimitee NUMBER (0 might mean unlimited)
    QDateTime startDateTime;            // Corresponds to DateDebut TIMESTAMP
    QDateTime endDateTime;              // Corresponds to DateFin TIMESTAMP
    int       magasinId = -1;           // Corresponds to MagasinID NUMBER (Foreign Key)
    int       articleId = -1;           // <<< ADDED: Corresponds to ARTICLEID NUMBER (Foreign Key) >>>
    QString   validationStatus = "UNKNOWN"; // Stores validation status (e.g., PENDING, VALIDATED, REJECTED)

    // --- Added Members ---
    int       claimCount = 0;           // Stores claim count (calculated via subquery)


    // --- Helper function to determine display status dynamically ---
    // (getCurrentStatus function remains unchanged)
    QString getCurrentStatus() const {
        QDateTime now = QDateTime::currentDateTime();

        // 1. Check explicit validation status first
        if (validationStatus.compare("REJECTED", Qt::CaseInsensitive) == 0) {
            return "Rejetée"; // Offer was explicitly rejected
        }
        if (validationStatus.compare("PENDING", Qt::CaseInsensitive) == 0) {
            // Offer is waiting for validation, regardless of dates (usually)
            // Or, you might want to check if it's expired even while pending:
            // if (endDateTime.isValid() && now > endDateTime) {
            //     return "Expirée (En Attente)";
            // }
            return "En Attente";
        }

        // 2. If not Rejected or Pending, check dates (assuming it might be VALIDATED or UNKNOWN)
        if (!startDateTime.isValid() || !endDateTime.isValid()) {
            // Check if it's a suggestion (ID -1) which might have invalid dates initially
            if (offreId == -1) {
                // It's an unsaved suggestion, status is determined by validationStatus only for now
                if (validationStatus.compare("VALIDATED", Qt::CaseInsensitive) == 0) return "Validée";
                if (validationStatus.compare("PENDING", Qt::CaseInsensitive) == 0) return "En Attente";
                // otherwise fall through or return specific status?
            } else {
                qWarning() << "OfferData::getCurrentStatus: Invalid dates for existing OfferID:" << offreId;
                return "Invalide"; // Handle cases where dates might be invalid for saved offers
            }
        }

        // Dates are valid (or it's an unsaved suggestion we handled above)
        if (now < startDateTime) {
            // Offer is scheduled for the future.
            // Check if it was explicitly validated.
            if (validationStatus.compare("VALIDATED", Qt::CaseInsensitive) == 0) {
                return "Planifiée"; // Validated and upcoming
            } else {
                // If status is UNKNOWN or PENDING, but dates are future
                return "Planifiée (Non Validée)"; // Or just "Planifiée" / "En Attente"
            }
        }

        if (now > endDateTime) {
            // Offer has expired, regardless of validation status (unless Rejected/Pending handled above)
            return "Terminee";
        }

        // 3. If we reach here, startDateTime <= now <= endDateTime
        // Check if it was explicitly validated to be considered "Active"
        if (validationStatus.compare("VALIDATED", Qt::CaseInsensitive) == 0) {
            return "En cours"; // Validated and currently running
        } else {
            // If status is UNKNOWN or PENDING, but dates are current
            return "En cours (Non Validée)"; // Or maybe treat as "En Attente"
        }
    }
};

#endif // OFFERDATA_H
