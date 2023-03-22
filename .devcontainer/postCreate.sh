#!/bin/sh

# https://code.visualstudio.com/docs/remote/devcontainerjson-reference
# this script should be executed from the workspaceFolder in the dev container
# it fires after the source code has been mounted

# invoke this script automatically by adding the following line to devcontainer.json
# "postCreateCommand": ".devcontainer/postCreate.sh",

# clone git repo dependencies into 'lib' directory
# We need to do this here as a 'postCreate' lifecycle event since we wish to clone into the workspace dir.
# Cloning prior to this (i.e. in Dockerfile) doesn't work because workspace isn't mounted yet.
# When workspace gets mounted, any work we had done there disappears.

mkdir -p lib
cd lib

git clone --depth 1 -b master https://github.com/raspberrypi/pico-sdk.git \
    && cd pico-sdk \
    && git submodule update --init \
    && cd ..

cd ..
