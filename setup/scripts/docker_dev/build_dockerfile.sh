#!/bin/bash
SCRIPT_PATH=$(dirname $(realpath "${BASH_SOURCE[0]}"))
COMMONS_PATH=$(realpath "${SCRIPT_PATH}/../commons")
set -a
source "${COMMONS_PATH}/common.sh"
set +a

set -e
DISABLE_SSH_KEY_COPY="#"

usage() {
  echo "Usage: $0 [-o|--os OS] [-u|--user USER] [-p|--password PASSWORD] [-r|--required-packages PACKAGE] [-x|--migrate-ssh-keys] [-c|--custom-script SCRIPT_PATH]"
  exit 1
}
MIGRATE_SSH_KEYS=false

mkdir -p ${DOCKERFILE_DIRECTORY}

while [[ $# -gt 0 ]]; do
  case "$1" in
    -o|--os)
      OS="$2"
      shift 2
      ;;
    -u|--user)
      USER="$2"
      shift 2
      ;;
    -p|--password)
      PASSWORD="$2"
      shift 2
      ;;
    -r|--required-packages)
      REQUIRED_PACKAGES+=("$2")
      shift 2
      ;;
    -x|--migrate-ssh-keys)
      MIGRATE_SSH_KEYS=true
      DISABLE_SSH_KEY_COPY=""
      shift
      ;;
    -c|--custom-script)
      CUSTOM_SCRIPT_PATH="$2"
      shift 2
      ;;
    *)
      usage
      ;;
  esac
done

rm -rf 

if $MIGRATE_SSH_KEYS; then
  DISABLE_SSH_KEY_COPY=""
  if [ -d "$HOME/.ssh" ]; then
    echo "Copying SSH folder to ${DOCKERFILE_DIRECTORY}/.ssh"
    cp -r "$HOME/.ssh" "${DOCKERFILE_DIRECTORY}/.ssh"
  else
    echo "SSH directory not found: $HOME/.ssh"
    exit 1
  fi
else 
  export DISABLE_SSH_KEY_COPY="#"
fi

if [ -n "$CUSTOM_SCRIPT_PATH" ]; then
  if [ -f "$CUSTOM_SCRIPT_PATH" ]; then
    cp "$CUSTOM_SCRIPT_PATH" "${DOCKERFILE_DIRECTORY}/custom_script.sh"
    echo "Custom script copied to ${DOCKERFILE_DIRECTORY}/custom_script.sh"
  else
    echo "Custom script file does not exist: $CUSTOM_SCRIPT_PATH"
    exit 1
  fi
fi

export REQUIRED_PACKAGES_FORMATTED=$(printf "\t%s \\  \n" "${REQUIRED_PACKAGES[@]}")
echo "${REQUIRED_PACKAGES_FORMATTED}"
# Create data directory
mkdir -p "${DATA_DIRECTORY}"
mkdir -p "${TMP_PATH}"
# export DISABLE_PROTO2_INSTALL="#"

envsubst < "${TEMPLATE_DIRECTORY}/${HOST_CONFIG_TEMPLATE}" > "${TMP_PATH}/${HOST_CONFIG_FILE_NAME}"
ls "${TMP_PATH}"
cat "${TMP_PATH}/${HOST_CONFIG_FILE_NAME}"
awk '{ gsub(/\\n/, "\n") } 1' "${TMP_PATH}/${HOST_CONFIG_FILE_NAME}" > "${DOCKERFILE_DIRECTORY}/${HOST_CONFIG_FILE_NAME}"
echo  "${TEMPLATE_DIRECTORY}/${STARTUP_SCRIPT_TEMPLATE} ${DOCKERFILE_DIRECTORY}/${STARTUP_SCRIPT_NAME}" 
envsubst < "${TEMPLATE_DIRECTORY}/${STARTUP_SCRIPT_TEMPLATE}" > "${DOCKERFILE_DIRECTORY}/${STARTUP_SCRIPT_NAME}"
envsubst < "${TEMPLATE_DIRECTORY}/${DEV_ENV_DOCKERFILE_TEMPLATE}" > "${DOCKERFILE_DIRECTORY}/${DEV_ENV_DOCKERFILE_NAME}"
echo "Dockerfile has been created at ${DOCKERFILE_PATH}"

set +e