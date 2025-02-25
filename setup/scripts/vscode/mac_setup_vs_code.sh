#!/bin/bash

SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
source ${SCRIPT_PATH}/common.sh

set -e

OS="$(uname -s)"

case "${OS}" in
    Darwin)
        echo "Detected macOS"
        if ! command -v brew &> /dev/null
        then
            echo "Homebrew not found. Installing Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        else
            echo "Homebrew is already installed."
        fi

        if [ "$(brew list --cask | grep visual-studio-code)" ] && [ ! "$(command -v code)" ];
        then
            echo "Visual Studio Code is already installed."
            exit 0
        else
            echo "Installing Visual Studio Code..."
            brew install --cask visual-studio-code
        fi
        ;;
    Linux)
        if [ -f /etc/os-release ]; then
            . /etc/os-release
            if [ "$ID" = "ubuntu" ]; then
                echo "Detected Ubuntu"
                sudo apt update
                sudo apt install -y wget gpg

                if ! command -v code &> /dev/null
                then
                    echo "Installing Visual Studio Code..."
                    wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
                    sudo install -o root -g root -m 644 packages.microsoft.gpg /usr/share/keyrings/
                    sudo sh -c 'echo "deb [arch=amd64 signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
                    sudo apt update
                    sudo apt install -y code
                else
                    echo "Visual Studio Code is already installed."
                fi
            else
                echo "Unsupported Linux distribution: $ID"
                exit 1
            fi
        else
            echo "Unsupported Linux distribution"
            exit 1
        fi
        ;;
    *)
        echo "Unsupported operating system: ${OS}"
        exit 1
        ;;
esac

echo "Installing VS Code extensions..."

code --install-extension ms-vscode.cpptools-extension-pack
code --install-extension ms-vscode-remote.remote-containers
code --install-extension GitHub.copilot
code --install-extension ms-azuretools.vscode-docker

echo "VS Code and extensions have been installed successfully!"

set +e