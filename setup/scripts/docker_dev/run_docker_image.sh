#!/bin/bash 
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
source "${COMMONS_PATH}/common.sh"
set -ex
# If -i is not passed, the container will not be interactive add -d to DOCKER_RUN_FLAGS else add -it to DOCKER_RUN_FLAG
if [ "$1" == "-i" ]; then
  DEV_DOCKER_RUN_ARGS+=" -it"
else 
  DEV_DOCKER_RUN_ARGS+=" -d"
fi
echo "Running dockerfile, mapping ${REPO_PATH} to ${WORKSPACE_DIR}"
docker network inspect ${DOCKER_NETWORK} >/dev/null 2>&1 || docker network create ${DOCKER_NETWORK}
echo "${DOCKER_RUN_FLAGS}"
docker run ${DEV_DOCKER_RUN_ARGS} "${DEV_DOCKER_IMAGE_NAME}"
set +ex
