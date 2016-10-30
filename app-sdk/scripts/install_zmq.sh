#!/bin/bash

echo "installing zeromq..."
{
	git clone https://github.com/zeromq/libzmq.git
	cd libzmq
	./autogen.sh
	./configure --with-libsodium
	make -j $nproc
	sudo make install
	sudo ldconfig
} &> /dev/null
echo "Done."
