# Agent 1 — Secrets and Config

Goal: remove hardcoded secrets and load config from env/QSettings.

Tasks
- Remove hardcoded DB credentials and API keys from source.
- Add a config loading pattern (env vars + QSettings fallback).
- Update these paths:
  - `OffresFlash/offersflashscreen.cpp`
  - `FoundItemsAdminWidget/main.cpp`
  - `sans_titre10/connection.cpp`
- Add a short README note or inline comment describing required env vars.

Acceptance
- No secrets remain in source.
- App still connects when env vars are set.
- Clear error message when config is missing.

