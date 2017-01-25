"""This thing ... """

import argparse
import os
import subprocess
from pathlib import Path

parser = argparse.ArgumentParser()

# parser.add_argument("--addon_path", help="The path of the addon.")
parser.add_argument("--of_root", help="The OF_ROOT directory.")
# parser.add_argument("--of_addons", help="The OF_ADDONS directory.")
parser.add_argument("--username", help="The default github username.")
parser.add_argument("--branch", help="The default github branch.")
parser.add_argument("--depth", help="The default github clone depth.")

args = parser.parse_args()


# These paths assume that this file is located in
#   OF_ROOT/addons/ADDON_NAME/ci/common/install_dependencies.py

ADDON_PATH = Path('../../..')
ADDON_NAME = ADDON_PATH.resolve().name

if args.of_root:
    OF_ROOT = Path(args.of_root).expanduser()
elif os.getenv('OF_ROOT'):
    OF_ROOT = Path(os.getenv('OF_ROOT')).expanduser()
else:
    OF_ROOT = Path('../../../../..').resolve()

OF_ADDONS = OF_ROOT / 'addons'

GH_USERNAME = args.username if args.username else 'bakercp'
GH_BRANCH = args.branch if args.branch else 'master'
GH_DEPTH = args.depth if args.depth else 1

print(ADDON_PATH)
print(ADDON_NAME)
print(OF_ROOT)
print(OF_ADDONS.resolve())

# print(ADDON_PATH)
# print(ADDON_NAME)
# print(OF_ADDONS)
# print(OF_ROOT)

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

for required_addon in REQUIRED_ADDONS:
    required_addon_path = OF_ADDONS.resolve() / addon
    if not required_addon_path.exists():
        repository = "https://github.com/" + GH_USERNAME + "/" + addon + ".git"
        subprocess.call(["git", "clone", repository, str(required_addon_path)])

