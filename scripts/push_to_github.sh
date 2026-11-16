#!/usr/bin/env bash
set -euo pipefail

REMOTE_URL="${1:-https://github.com/wk-frank/gd32-px.git}"
REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_DIR"

if git remote get-url origin >/dev/null 2>&1; then
  git remote set-url origin "$REMOTE_URL"
else
  git remote add origin "$REMOTE_URL"
fi

echo "Remote 'origin' configured -> $(git remote get-url origin)"
echo "Pushing main and feat/gd32h757-nuttx to $REMOTE_URL ..."
git push -u origin main
git push -u origin feat/gd32h757-nuttx
echo "Push completed."
