#!/bin/bash

# Build script for Calculator GTK application

echo "Building Calculator..."

# Compile the application
g++ main.cpp `pkg-config --cflags --libs gtk+-3.0` -o calculator

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Run './calculator' to start the application."
else
    echo "Build failed. Please check for errors."
fi
