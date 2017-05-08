#!/usr/bin/env bash
set -e

THIS_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if ! [ -z ${OF_ROOT} ]; then
  OF_ROOT=$( cd "$( dirname "${BASH_SOURCE[0]}")/../../../.." ; pwd -P )
fi

if ! [ -z ${ADDON_NAME} ]; then
  ADDON_NAME=$(basename $( cd "$( dirname "${BASH_SOURCE[0]}")/../.." ; pwd -P ))
fi

# Default addon github info.
GH_USERNAME='bakercp'
GH_BRANCH='master'
GH_DEPTH=1

# An array of required addons that will be gathered below.
REQUIRED_ADDONS=()

# Extract ADDON_DEPENDENCIES from addon_config.mk file.
ADDON_CONFIG_MK=$OF_ROOT/addons/$ADDON_NAME/addon_config.mk

echo ""
echo "Running ${THIS_DIR}/install.sh"
echo "---------------------------------"
echo "        OF_ROOT: ${OF_ROOT}"
echo "     ADDON_NAME: ${ADDON_NAME}"
echo "ADDON_CONFIG_MK: ${ADDON_CONFIG_MK}"
echo ""

echo "Examining ${ADDON_CONFIG_MK} ..."
if [ -f $ADDON_CONFIG_MK ]; then
  while read line; do
    if [[ $line == ADDON_DEPENDENCIES* ]] ;
    then
      line=${line#*=}
      IFS=' ' read -ra ADDR <<< "$line"
      for i in "${ADDR[@]}"; do
          REQUIRED_ADDONS+=($i)
      done
    fi
  done < $ADDON_CONFIG_MK
else
  echo "No ${ADDON_CONFIG_MK} found, skipping."
fi

echo "Gathering addons from addons.make files ..."
# Gather addons from all examples.
for addons_make in $OF_ROOT/addons/$ADDON_NAME/example*/addons.make; do
  echo "Examining ${addons_make} ..."
  while read addon; do
    if [ ${addon} != ${ADDON_NAME} ] ;
    then
      REQUIRED_ADDONS+=($addon)
    fi
  done < $addons_make
done

# We aren't de-duplicating array to keep it pure bash.
echo "Cloning addons ..."
for addon in "${REQUIRED_ADDONS[@]}"
do
  if [ ! -d ${OF_ROOT}/addons/${addon} ]; then
    echo "Cloning addon: ${addon}."
    git clone --depth=$GH_DEPTH https://github.com/$GH_USERNAME/$addon.git ${OF_ROOT}/addons/${addon}
  else
    echo "Addon: ${addon} already exists, skipping."
  fi
done

echo "Finished installing addon dependencies."
