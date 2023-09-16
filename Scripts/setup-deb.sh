#!/bin/bash

apt update
apt install python3 -y
apt install make -y
apt install build-essential -y
apt install git -y

python3 setup.py