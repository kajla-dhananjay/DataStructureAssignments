#!/bin/bash 
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
source ${COMMONS_PATH}/common.sh
set -ex
cd ${DOCKERFILE_DIRECTORY}
if [ "$1" == "-n" ]; then
    DOCKER_BUILD_ARGS=" --no-cache ${DOCKER_BUILD_ARGS}"
fi
docker build ${DOCKER_BUILD_ARGS} -f "${DOCKERFILE_PATH}" -t ${DEV_DOCKER_IMAGE_NAME} . 
set +ex
