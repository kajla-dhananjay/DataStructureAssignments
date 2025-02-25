#!/bin/bash
set -ex
BUILD_DOCKER_IMAGE_ARG="${1:-}"
./setup/scripts/docker_dev/build_dockerfile.sh -x && ./setup/scripts/docker_dev/build_docker_image.sh ${BUILD_DOCKER_IMAGE_ARG} && ./setup/scripts/docker_dev/run_docker_image.sh -i
set -ex