#!/bin/bash

mkdir hos_dependencies
cd hos_dependencies
../install_protobuf.sh
../install_sodium.sh
../install_zmq.sh
../install_tbb.sh
cd ..
echo "removing dependency folder"
rm -r hos_dependencies
