#include "configmanager.h"
#include <QProcessEnvironment>
#include <QCoreApplication>
#include <QDebug>

QString ConfigManager::getEnvOrSetting(const QString &envKey, const QString &settingsKey, const QString &fallback)
{
    // 1. Try environment variable first
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains(envKey)) {
        QString value = env.value(envKey);
        if (!value.isEmpty()) {
            qDebug() << "[ConfigManager] Loaded" << envKey << "from environment variable.";
            return value;
        }
    }

    // 2. Fall back to QSettings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName().isEmpty() ? "Mallmate" : QCoreApplication::organizationName(),
                       QCoreApplication::applicationName().isEmpty() ? "Mallmate" : QCoreApplication::applicationName());

    if (settings.contains(settingsKey)) {
        QString value = settings.value(settingsKey).toString();
        if (!value.isEmpty()) {
            qDebug() << "[ConfigManager] Loaded" << settingsKey << "from QSettings.";
            return value;
        }
    }

    // 3. Return fallback (may be empty)
    if (!fallback.isEmpty()) {
        qDebug() << "[ConfigManager] Using fallback value for" << envKey;
    }
    return fallback;
}

QString ConfigManager::getDatabaseDsn(const QString &fallbackDsn)
{
    return getEnvOrSetting("MALLMATE_DB_DSN", "Database/DSN", fallbackDsn);
}

QString ConfigManager::getDatabaseUser(const QString &fallbackUser)
{
    return getEnvOrSetting("MALLMATE_DB_USER", "Database/User", fallbackUser);
}

QString ConfigManager::getDatabasePassword(const QString &fallbackPassword)
{
    return getEnvOrSetting("MALLMATE_DB_PASSWORD", "Database/Password", fallbackPassword);
}

bool ConfigManager::isDatabaseConfigured()
{
    return !getDatabaseDsn().isEmpty() &&
           !getDatabaseUser().isEmpty() &&
           !getDatabasePassword().isEmpty();
}

QString ConfigManager::getMissingConfigMessage()
{
    QStringList missing;

    if (getDatabaseDsn().isEmpty()) {
        missing << "MALLMATE_DB_DSN (Database DSN/connection string)";
    }
    if (getDatabaseUser().isEmpty()) {
        missing << "MALLMATE_DB_USER (Database username)";
    }
    if (getDatabasePassword().isEmpty()) {
        missing << "MALLMATE_DB_PASSWORD (Database password)";
    }

    if (missing.isEmpty()) {
        return QString();
    }

    return QString("Missing database configuration:\n\n%1\n\n"
                   "Please set these as environment variables or in your application settings.\n\n"
                   "Example (environment variables):\n"
                   "  set MALLMATE_DB_DSN=Mallmate_DBN\n"
                   "  set MALLMATE_DB_USER=mallmate_user\n"
                   "  set MALLMATE_DB_PASSWORD=your_password\n\n"
                   "See CONFIG.md for more details.")
        .arg(missing.join("\n"));
}
