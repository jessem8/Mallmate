<p align="center">
  <img src="docs/mallmate_logo.png" alt="Mallmate Logo" width="300"/>
</p>

<h1 align="center">🛒 Mallmate</h1>

<p align="center">
  <strong>A comprehensive Qt C++ mall management system</strong>
</p>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#modules">Modules</a> •
  <a href="#prerequisites">Prerequisites</a> •
  <a href="#installation">Installation</a> •
  <a href="#configuration">Configuration</a> •
  <a href="#security">Security</a>
</p>

---

## 📋 Overview

**Mallmate** is a professional desktop application suite designed for comprehensive mall management operations. Built with Qt 6 and C++, it provides a modern, feature-rich interface for managing various aspects of mall operations including parking, lost & found items, flash offers, and client/equipment management.

## ✨ Features

- 🚗 **Smart Parking Management** - Real-time spot tracking with Arduino sensor integration
- 📦 **Lost & Found System** - Complete item lifecycle management with PDF reporting
- 🏷️ **Flash Offers Platform** - Promotional offer management with AI-powered suggestions
- 👥 **Client Management** - Customer database with risk assessment and notifications
- 📊 **Advanced Analytics** - Charts and statistics across all modules
- 📄 **Export Capabilities** - PDF and CSV export functionality
- 🔒 **Secure Configuration** - Environment variable-based credential management

## 🧩 Modules

### 1. FoundItemsAdminWidget

**Lost & Found Management System**

| Feature | Description |
|---------|-------------|
| Item Registration | Add found items with descriptions, locations, and photos |
| Status Tracking | Track items through Pending → Claimed → Returned lifecycle |
| Lost Reports | Accept and match lost item reports from visitors |
| Search & Filter | Powerful search with category and status filtering |
| PDF Export | Generate professional reports with charts |
| Statistics | Visual dashboards with pie charts |

### 2. OffresFlash

**Flash Offers Management Platform**

| Feature | Description |
|---------|-------------|
| Offer CRUD | Create, read, update, delete promotional offers |
| Store Integration | Link offers to specific mall stores |
| AI Suggestions | Get AI-powered offer recommendations |
| Statistics Dashboard | Track offer performance with visual charts |
| Kiosk Monitor | Display offers on mall kiosk screens |
| QR Code Support | Generate QR codes for offers |

### 3. Parking

**Smart Parking Management System**

| Feature | Description |
|---------|-------------|
| Spot Management | Real-time available/occupied status tracking |
| Arduino Integration | Connect ultrasonic sensors via serial port |
| Reservations | Handle parking spot reservations |
| Visual Map | Interactive parking lot visualization |
| Ticket Generation | Create parking tickets with QR codes |
| Export | PDF and CSV reports for parking history |

### 4. Sans_titre10 (Equipment & Client Management)

**Client and Equipment Management System**

| Feature | Description |
|---------|-------------|
| Client Database | Manage mall client/tenant information |
| Notifications | System-wide notification management |
| Risk Assessment | Report and track potential risks |
| Evacuation Plans | Emergency evacuation management |
| Timeline View | Visual client activity timeline |
| PDF Reports | Generate comprehensive client reports |

## 📦 Prerequisites

| Requirement | Version | Notes |
|-------------|---------|-------|
| **Qt Framework** | 6.x | With Charts, SQL, and SerialPort modules |
| **C++ Compiler** | C++17+ | MSVC, GCC, or Clang |
| **Oracle Database** | 11g+ | With ODBC driver configured |
| **Qt Creator** | 10+ | Recommended IDE |

### Required Qt Modules

```
Qt Core, Qt GUI, Qt Widgets, Qt SQL, Qt Charts, Qt PrintSupport, Qt SerialPort
```

## 🔧 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/Mallmate.git
cd Mallmate
```

### 2. Build Individual Modules

Each module can be built separately using qmake:

```bash
# Build FoundItemsAdminWidget
cd FoundItemsAdminWidget
qmake FoundItemsAdminWidget.pro
make

# Build OffresFlash
cd ../OffresFlash
qmake OffresFlash.pro
make

# Build Parking
cd ../parking
qmake parking.pro
make

# Build Equipment Manager
cd ../sans_titre10
qmake sans_titre10.pro
make
```

### 3. Using Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select the `.pro` file for the desired module
4. Configure the kit (Qt 6.x with your compiler)
5. Build → Build Project

## ⚙️ Configuration

Mallmate uses environment variables for secure database configuration. See [CONFIG.md](CONFIG.md) for detailed setup instructions.

### Quick Setup (Windows PowerShell)

```powershell
$env:MALLMATE_DB_DSN = "Your_ODBC_DSN"
$env:MALLMATE_DB_USER = "your_username"
$env:MALLMATE_DB_PASSWORD = "your_password"
```

### Configuration Hierarchy

1. **Environment Variables** (Recommended) - Most secure approach
2. **QSettings Fallback** - For development convenience

## 🔒 Security

This codebase has undergone comprehensive security review with the following improvements:

| Category | Status | Description |
|----------|--------|-------------|
| **Secrets Management** | ✅ Fixed | Hardcoded credentials removed, now uses ConfigManager |
| **SQL Injection** | ✅ Fixed | All queries use prepared statements with bound values |
| **Column Injection** | ✅ Fixed | Dynamic columns validated against strict whitelist |
| **Build Artifacts** | ✅ Fixed | Comprehensive .gitignore prevents tracking build files |

> **Note:** The `common/configmanager.cpp` provides centralized, secure configuration loading across all modules.

## 🗂️ Project Structure

```
Mallmate/
├── common/                      # Shared utilities
│   ├── configmanager.cpp        # Centralized configuration loader
│   └── configmanager.h
├── FoundItemsAdminWidget/       # Lost & Found module
├── OffresFlash/                 # Flash Offers module
├── parking/                     # Parking Management module
├── sans_titre10/                # Equipment/Client module
├── docs/                        # Documentation assets
├── review/                      # Code review notes
├── CONFIG.md                    # Configuration guide
├── .gitignore                   # Git ignore rules
└── README.md                    # This file
```

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style

- Follow Qt coding conventions
- Use prepared statements for all database operations
- Never hardcode sensitive credentials
- Include meaningful comments for complex logic

## 📄 License

This project is developed for educational purposes. Please contact the maintainers for licensing information.

## 👥 Authors

Developed with ❤️ by the Mallmate Team

---

<p align="center">
  <img src="docs/mallmate_logo.png" alt="Mallmate" width="100"/>
  <br/>
  <em>Making mall management simple and efficient</em>
</p>
