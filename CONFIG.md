# Mallmate Configuration

## Database Configuration

Mallmate requires database credentials to connect to the Oracle database. Credentials should **never** be hardcoded in source code.

### Environment Variables (Recommended)

Set the following environment variables before running the application:

| Variable | Description | Example |
|----------|-------------|---------|
| `MALLMATE_DB_DSN` | ODBC Data Source Name or connection string | `Mallmate_DBN` or `Driver={Oracle in XE};Dbq=//localhost:1521/XE` |
| `MALLMATE_DB_USER` | Database username | `mallmate_user` |
| `MALLMATE_DB_PASSWORD` | Database password | `your_secure_password` |

#### Windows (Command Prompt)

```cmd
set MALLMATE_DB_DSN=Mallmate_DBN
set MALLMATE_DB_USER=mallmate_user
set MALLMATE_DB_PASSWORD=your_password
YourApp.exe
```

#### Windows (PowerShell)

```powershell
$env:MALLMATE_DB_DSN = "Mallmate_DBN"
$env:MALLMATE_DB_USER = "mallmate_user"
$env:MALLMATE_DB_PASSWORD = "your_password"
.\YourApp.exe
```

#### Persistent Environment Variables (Windows)

Use System Properties → Environment Variables to set these permanently for your user account.

### QSettings Fallback

If environment variables are not set, the application will look for settings in the user's configuration file:

- **Windows**: `%APPDATA%\Mallmate\Mallmate.ini`
- **Linux**: `~/.config/Mallmate/Mallmate.conf`

Settings keys:

- `Database/DSN`
- `Database/User`
- `Database/Password`

### Error Handling

If configuration is missing, the application will display a clear error message listing which values are required and how to set them.
