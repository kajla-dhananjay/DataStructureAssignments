#!/bin/bash
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
source ${COMMONS_PATH}/common.sh
PORT=8100
TEST_SERVER_NAME="python_server"
TEST_CLIENT_NAME="curl_client"
