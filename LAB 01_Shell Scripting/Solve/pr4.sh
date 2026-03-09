#!/bin/bash
file=$*
find . -type f -name "$file" -delete
echo "'$file' Deleted"
