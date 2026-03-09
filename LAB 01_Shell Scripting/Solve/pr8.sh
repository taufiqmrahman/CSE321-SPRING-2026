#!/bin/bash

pattern=$1
file=$2

lines=$(grep -c "$pattern" "$file")
words=$(wc -w < "$file")

echo "Lines containing '$pattern': $lines"
echo "Total words in file: $words"


