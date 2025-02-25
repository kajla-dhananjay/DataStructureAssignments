#!/bin/bash
set -e

SCRIPT_DIR=$(dirname "$(realpath "${BASH_SOURCE[0]}")")

echo "Setting up vs-code" && \
${SCRIPT_DIR}/vscode/mac_setup_vs_code.sh && \

echo "Building docker file" && \
${SCRIPT_DIR}/docker_dev/build_dockerfile.sh -x && \

echo "Building docker image" && \
${SCRIPT_DIR}/docker_dev/build_docker_image.sh && \

echo "Configuring vs-code dev container" && \
${SCRIPT_DIR}/vscode/generate_dev_container.sh && \

echo "Starting vs-code" && \
${SCRIPT_DIR}/vscode/open_folder_in_devcontainer.sh

set +e
