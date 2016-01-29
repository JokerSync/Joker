#!/bin/bash

# Taken from https://www.objc.io/issues/6-build-tools/travis-ci/#add-scripts

set -e

# Create a custom keychain
security create-keychain -p travis macos-build.keychain

# Make the custom keychain default, so xcodebuild will use it for signing
security default-keychain -s macos-build.keychain

# Unlock the keychain
security unlock-keychain -p travis macos-build.keychain

# Set keychain timeout to 1 hour for long builds
security set-keychain-settings -t 3600 -l ~/Library/Keychains/macos-build.keychain

# Add certificates to keychain and allow codesign to access them
security import ./scripts/certs/apple.cer -k ~/Library/Keychains/macos-build.keychain -A
security import ./scripts/certs/mac_app.cer -k ~/Library/Keychains/macos-build.keychain -A
security import ./scripts/certs/dev_id.p12 -k ~/Library/Keychains/macos-build.keychain -P $KEY_PASSWORD -A

# Display the available identities
security find-identity
