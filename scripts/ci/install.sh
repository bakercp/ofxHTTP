#!/usr/bin/env bash
set -e

OF_ROOT=$( cd "$(dirname "$0")/../../../.." ; pwd -P )

# Default addon github info.
GH_USERNAME='bakercp'
GH_BRANCH='master'
GH_DEPTH=1

# An array of required addons that will be gathered below.
REQUIRED_ADDONS=()

# Extract ADDON_DEPENDENCIES from addon_config.mk file.
if [ -f ${OF_ROOT}/addons/${OF_ADDON_NAME}/addon_config.mk ]; then
  while read line; do
    if [[ $line == ADDON_DEPENDENCIES* ]] ;
    then
      line=${line#*=}
      IFS=' ' read -ra ADDR <<< "$line"
      for i in "${ADDR[@]}"; do
          REQUIRED_ADDONS+=($i)
      done
    fi
  done < ${OF_ROOT}/addons/${OF_ADDON_NAME}/addon_config.mk
fi

# Gather addons from all examples.
for addons_make in ${OF_ROOT}/addons/${OF_ADDON_NAME}/example*/addons.make; do
  while read addon; do
    if [ ${addon} != ${OF_ADDON_NAME} ] ;
    then
      REQUIRED_ADDONS+=($addon)
    fi
  done < $addons_make
done

# We aren't de-duplicating array to keep it pure bash.
for addon in "${REQUIRED_ADDONS[@]}"
do
  if [ ! -d ${OF_ROOT}/addons/${addon} ]; then
    git clone --depth=$GH_DEPTH https://github.com/$GH_USERNAME/$addon.git ${OF_ROOT}/addons/${addon}
  fi
done
