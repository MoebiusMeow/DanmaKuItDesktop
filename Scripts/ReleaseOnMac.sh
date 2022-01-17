#!/usr/bin/env bash

# Should be run in project root

QT_ROOT=/usr/local/Qt-6.2.2/

set -e

if [ ! -f ./CMakeLists.txt ]; then
  echo "CMakeLists.txt does not exist. Maybe not at project root!"
  exit 1
fi

echo "Creating build folder"

rm -rf ./cmake-build-release
mkdir -p ./cmake-build-release
cd cmake-build-release

echo "Building..."

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel

echo "Signing..."

$QT_ROOT/bin/macdeployqt ./DanmakuItDesktop.app -codesign="Developer ID Application" -dmg