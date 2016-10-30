#!/bin/bash

echo "installing protocol buffers..."
{
	git clone https://github.com/google/protobuf.git
	cd protobuf
	./autogen.sh
	./configure
	make -j $nproc
	sudo make install
	sudo ldconfig
} &> /dev/null
echo "Done."
