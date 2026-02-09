#!/bin/bash
destination=$1
shift
for files in $* ; do
	echo "Copying $f to '$destination'"
        cp "$files" "$destination"
done
echo "Copy process finished."

