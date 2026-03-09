#!/bin/bash
file=$*
if [ -f "$file" ]; then
	echo "'$file' is a regular file."
	if [ -r "$file" ]; then
		echo "It is readable"
	else	
		echo "It is not readable"
	fi
	
	if [ -w "$file" ]; then
		echo "It is writable"
	else	
		echo "It is not writable"
	fi
	
	if [ -x "$file" ]; then
		echo "It is executable"
	else	
		echo "It is not executable"
	fi
else	
	echo "$file is not a regular file."
fi
