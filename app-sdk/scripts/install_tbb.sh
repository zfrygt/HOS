#!/bin/bash

echo "installing tbb..."
{
	sudo apt-get install libtbb2
	sudo ldconfig
} &> /dev/null
echo "Done."
