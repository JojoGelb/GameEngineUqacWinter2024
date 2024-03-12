#!/usr/bin/env sh
find . -maxdepth 1 \( -type f -not -path "./.*" -not -path "*.zip" -o \( -iname "include" -o -iname "src" \) \) -print0 | xargs -0 zip -r remise.zip