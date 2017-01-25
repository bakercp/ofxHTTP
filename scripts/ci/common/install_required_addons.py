"""This thing ... """

import subprocess
import argparse
from pathlib import Path

parser = argparse.ArgumentParser()

parser.add_argument("--username", help="The default github username.")
parser.add_argument("--branch", help="The default github branch.")
parser.add_argument("--depth", help="The default github clone depth.")

args = parser.parse_args()

GH_USERNAME = args.username if args.username else 'bakercp'
GH_BRANCH = args.branch if args.branch else 'master'
GH_DEPTH = args.depth if args.depth else 1

# These paths assume that this file is located in
#   OF_ROOT/addons/ADDON_NAME/ci/common/install_dependencies.py

ADDON_PATH = Path('../../..')
ADDON_NAME = ADDON_PATH.resolve().name
OF_ADDONS = Path('../../../..')
OF_ROOT = Path('../../../../..')

print(ADDON_PATH)
print(ADDON_NAME)
print(OF_ADDONS)
print(OF_ROOT)

REQUIRED_ADDONS = set([])

ADDON_CONFIG_MK = Path(ADDON_PATH, 'addon_config.mk')

# Collect any required dependencies from the ADDON_CONFIG.mk file.
if ADDON_CONFIG_MK.exists():
    for line in ADDON_CONFIG_MK.open().readlines():
        if 'ADDON_DEPENDENCIES' in line:
            addons = line[line.index('=') + 1:].strip().split()
            for addon in addons:
                REQUIRED_ADDONS.add(addon)

# Collect any required dependencies from the example*/addons.make files.
for addon_mk in ADDON_PATH.glob("example*/addons.make"):
    for addon in addon_mk.open().readlines():
        REQUIRED_ADDONS.add(addon.strip())

print(REQUIRED_ADDONS)

# Remove this addon from the list.
REQUIRED_ADDONS.remove(ADDON_NAME)

for addon in REQUIRED_ADDONS:
    addon_path = OF_ADDONS.resolve() / addon
    if not addon_path.exists():
        repository = "https://github.com/" + GH_USERNAME + "/" + addon + ".git"
        subprocess.call(["git", "clone", repository, str(addon_path)])

