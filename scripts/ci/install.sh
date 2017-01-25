#!/usr/bin/env bash
set -e

echo `pwd`

python3 scripts/ci/common/install_required_addons.py

ls -la ~/openFrameworks/addons