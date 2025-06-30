#!/usr/bin/env bash

set -e

echo "== NESKVIK WEB BUILD =="
echo

# Go up to repo root from /wasm
cd "$(dirname "$0")"

# Always configure from root
emcmake cmake -S .. -B ../build/web -DBUILD_WEB=ON -DCMAKE_BUILD_TYPE=Release

echo
echo "== Building..."
cmake --build ../build/web

echo
echo "====================================="
echo "   NESKVIK WEB BUILD COMPLETE"
echo "====================================="