#!/usr/bin/env bash
set -e

# We assume we are in the `openFrameworks/addons/ofxAddon/scripts` directory.
OF_ROOT=${OF_ROOT:-$(cd "$( dirname "${BASH_SOURCE[0]}" )/../../../" && pwd)}

OPTIND=1
while getopts o: opt ; do
  case "$opt" in
    o) OF_ROOT=$OPTARG ; ;;
  esac
done
shift "$((OPTIND-1))"

if [ ! -d $OF_ROOT ]; then 
    echo "The \"${OF_ROOT}\" directory doesn't exist. Please set the correct OF_ROOT using the -o option (e.g. ./bootstrap -o /user/foo/openFrameworks)."
fi

if [ ! -d $OF_ROOT/scripts ]; then 
    echo "The \"${OF_ROOT}\" directory exists, but \"${OF_ROOT}/scripts\" doesn't exist. Please set the correct OF_ROOT using the -o option (e.g. ./bootstrap -o /user/foo/openFrameworks)."
fi

OFX_PATH=${OF_ROOT}/scripts/ofx

if ! [ -f ${OFX_PATH}/ofx ] ; then
  echo "ofx if not installed, pulling latest version."
  git clone https://github.com/openframeworks/apothecary.git ${OFX_PATH}
else
  pushd "${OFX_PATH}" > /dev/null
  if git rev-parse --is-inside-work-tree ; then
    echo "ofx is under git control, updating."
    git pull origin master
  else
    echo "ofx is not under git control, so it may not be up-to-date."
  fi
  popd > /dev/null
fi

# Get the path of this addon.
THIS_ADDON_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)

# Bootstrap using ofx
/usr/bin/env bash ${OFX_PATH}/ofx -o ${OF_ROOT} bootstrap ${THIS_ADDON_PATH}
