#!/bin/bash

echo "installing sodium..."
{
	git clone https://github.com/jedisct1/libsodium.git
	cd libsodium
	./autogen.sh
	./configure
	make -j $nproc
	#sudo make install
	#sudo ldconfig
} &> /dev/null
echo "Done."
