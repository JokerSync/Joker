#!/bin/bash

brew install python pyenv
eval "$(pyenv init -)"
pyenv install 2.7.6
pyenv global 2.7.6
pyenv rehash
pip install cpp-coveralls
