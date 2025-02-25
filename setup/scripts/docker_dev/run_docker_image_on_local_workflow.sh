#!/bin/bash 
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
source "${COMMONS_PATH}/common.sh"
set -ex
cd ${REPO_PATH}
TESTFILES="./setup/scripts/workspace_tests/build_and_test.sh"
echo "Running dockerfile, mapping ${REPO_PATH} to ${WORKSPACE_DIR}"
docker network inspect ${DOCKER_NETWORK} >/dev/null 2>&1 || docker network create ${DOCKER_NETWORK}
docker run ${LOCAL_DOCKER_RUN_ARGS} --entrypoint="" ${DEV_DOCKER_IMAGE_NAME} bash -c "sudo chown -R ${USER}:docker /var/run/docker.sock && sudo -u ${USER} ${TESTFILES}"
set +ex