#!/bin/bash

SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
source ${SCRIPT_PATH}/common.sh

set -e

echo "Creating devcontainer.json at ${DEVCONTAINER_PATH}"

if [ ! -d "${DEVCONTAINER_DIRECTORY}" ]; then
    echo "${DEVCONTAINER_DIRECTORY} does not exist. Creating it..."
    mkdir -p "${DEVCONTAINER_DIRECTORY}"
fi

cat > "${DEVCONTAINER_PATH}" <<EOL
{
    "name": "Warp Drive Container",
    "build": {
        "dockerfile": "${DOCKERFILE_PATH}"
    },
    "runArgs": [
        "--network=${DOCKER_NETWORK}",
        "-v",
        "${DOCKER_SOCKET}:${DOCKER_SOCKET}"
    ],
    "customizations": {
        "vscode": {
            "settings": {
                "terminal.integrated.shell.linux": "${DOCKER_SHELL_PATH}"
            }, 
            "extensions": [
                "ms-vscode.cpptools",
                "ms-vscode.cmake-tools",
                "ms-vscode.cpptools-extension-pack",
                "GitHub.copilot"
            ]
        }
    },
    "postStartCommand": "/home/dev/host_config.sh"
}
EOL

echo "devcontainer.json has been created at ${DEVCONTAINER_PATH}"

set +e