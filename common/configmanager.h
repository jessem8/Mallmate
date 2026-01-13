#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QSettings>

/**
 * @brief Configuration manager for database credentials.
 *
 * Loads database configuration from environment variables first,
 * falling back to QSettings if env vars are not set.
 *
 * Required environment variables:
 * - MALLMATE_DB_DSN: ODBC Data Source Name or connection string
 * - MALLMATE_DB_USER: Database username
 * - MALLMATE_DB_PASSWORD: Database password
 */
class ConfigManager
{
public:
    /**
     * @brief Get the database DSN/connection string.
     * @param fallbackDsn Optional fallback value if not configured.
     * @return The DSN string, or empty if not configured.
     */
    static QString getDatabaseDsn(const QString &fallbackDsn = QString());

    /**
     * @brief Get the database username.
     * @param fallbackUser Optional fallback value if not configured.
     * @return The username, or empty if not configured.
     */
    static QString getDatabaseUser(const QString &fallbackUser = QString());

    /**
     * @brief Get the database password.
     * @param fallbackPassword Optional fallback value if not configured.
     * @return The password, or empty if not configured.
     */
    static QString getDatabasePassword(const QString &fallbackPassword = QString());

    /**
     * @brief Check if all required database configuration is present.
     * @return true if DSN, user, and password are all configured.
     */
    static bool isDatabaseConfigured();

    /**
     * @brief Get a formatted error message for missing configuration.
     * @return A user-friendly error message listing missing config items.
     */
    static QString getMissingConfigMessage();

private:
    static QString getEnvOrSetting(const QString &envKey, const QString &settingsKey, const QString &fallback = QString());
};

#endif // CONFIGMANAGER_H
