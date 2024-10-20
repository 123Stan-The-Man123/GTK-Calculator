#!/bin/bash

TARGET_DIR="$HOME/.local/share/GTK-Calculator"

mkdir -p "$TARGET_DIR"
sudo cp -r . "$TARGET_DIR"

echo "Source code installed in $TARGET_DIR. You can compile it with:"
echo "cd $TARGET_DIR && make"
