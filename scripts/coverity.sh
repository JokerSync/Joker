#!/bin/sh

set -e

echo "TRAVIS_OS_NAME: $TRAVIS_OS_NAME"
echo "JOB: $JOB"

if [[ $TRAVIS_OS_NAME != osx || $JOB != BUILD_RELEASE_JOKER ]]; then
  echo "Bad build configuration: $TRAVIS_OS_NAME / $JOB"
  exit 1
fi

curl -s https://scan.coverity.com/scripts/travisci_build_coverity_scan.sh | bash
