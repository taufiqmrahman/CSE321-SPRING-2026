#!/bin/bash
file=$*
linec=$(wc -l < "$file")
wordc=$(wc -w < "$file")
echo "Number of lines : $linec"
echo "Number of words : $wordc"
