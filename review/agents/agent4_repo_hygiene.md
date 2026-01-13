# Agent 4 — Repo Hygiene

Goal: ignore and remove build artifacts.

Tasks
- Add/extend `.gitignore` to exclude:
  - `*/build/`
  - `*.pro.user`
  - common Qt build outputs
- Identify tracked build outputs and delete them.
- Avoid touching unrelated files or user changes.

Acceptance
- Build outputs no longer tracked.
- Clean working tree for build artifacts after ignore.

