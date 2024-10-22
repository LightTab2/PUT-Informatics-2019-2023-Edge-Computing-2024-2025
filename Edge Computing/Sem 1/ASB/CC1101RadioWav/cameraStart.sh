#!/bin/bash

# Activate the virtual environment
source "./venv/bin/activate"

# Check if the virtual environment was activated successfully
if [[ "$VIRTUAL_ENV" != "" ]]; then
    echo "Virtual environment activated."

    # Run the Python script
    python ./app.py

    # Deactivate the virtual environment
    deactivate
else
    echo "Failed to activate virtual environment."
fi
