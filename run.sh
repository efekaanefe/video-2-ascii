#!/bin/zsh

# Set your source and output file
SRC="main.cpp"
OUT="main"

# Compile the code
echo "Compiling $SRC..."
g++ $SRC -o $OUT `pkg-config --cflags --libs opencv4`

# Check if compile succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running program..."
    ./$OUT
else
    echo "Compilation failed!"
fi

