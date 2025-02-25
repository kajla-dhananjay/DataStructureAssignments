#!/bin/bash
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
source ${SCRIPT_PATH}/common.sh

set -e
code --folder-uri="vscode-remote://dev-container+$(echo "${REPO_PATH}" | tr -d '\n' | xxd -c 256 -p)"${WORKSPACE_DIR}""
set +e