#!/bin/bash

# Function to check if a Python package is installed
check_and_install() {
    PACKAGE=$1
    if python3 -c "import $PACKAGE" &> /dev/null; then
        echo "$PACKAGE is already installed."
    else
        echo "$PACKAGE is not installed. Installing..."
        pip3 install $PACKAGE
    fi
}

# Check and install required packages
check_and_install numpy
check_and_install matplotlib
check_and_install pandas
check_and_install tabulate