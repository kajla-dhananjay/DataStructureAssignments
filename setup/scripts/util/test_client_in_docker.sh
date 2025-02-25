#!/bin/bash 
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
source "${SCRIPT_PATH}/common.sh"
set -e
docker network inspect "${DOCKER_NETWORK}" >/dev/null 2>&1 || docker network create "${DOCKER_NETWORK}"
DOCKER_RUN_FLAGS=""
DOCKER_RUN_FLAGS+=" --network=${DOCKER_NETWORK}"
docker run ${DOCKER_RUN_FLAGS} --name ${TEST_CLIENT_NAME} curlimages/curl curl http://${TEST_SERVER_NAME}:${PORT}
set +e