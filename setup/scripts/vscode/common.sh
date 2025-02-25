#!/bin/bash
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
source ${COMMONS_PATH}/common.sh
DEVCONTAINER_DIRECTORY="${REPO_PATH}/.devcontainer"
DEVCONTAINER_PATH="${DEVCONTAINER_DIRECTORY}/devcontainer.json"
