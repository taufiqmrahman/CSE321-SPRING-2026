#!/bin/bash

echo "Enter the directory path:"
read dir
echo " Select a display format: "
echo " 1) Short format Display"
echo " 2) Long format Display"
echo " 3) Hidden files Display"
echo "Enter choice [1-3] : "
read choice  

case $choice in
	1)
		echo "Short Format Listing:"
        	ls "$dir"
        	;;
        2)
        	echo "Long Format Listing:"
        	ls -l "$dir"
        	;;
        3)
        	echo "Including Hidden Files:"
        	ls -a "$dir"
        	;;
        *)
        	echo "Invalid Input"
        	;;
esac
