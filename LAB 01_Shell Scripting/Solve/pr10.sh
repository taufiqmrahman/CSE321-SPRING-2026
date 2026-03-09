#!/bin/bash

echo "Enter number: "
read n

for (( i=1; i<=n; i++ ))
do
    for (( j=i; j<n; j++ ))
    do
        echo -n " "
    done
    for (( j=1; j<=i; j++ ))
    do
        if (( j == 1 || j == i || i == n )); then
            echo -n "*"
        else
            echo -n " "
        fi
    done
    echo ""
done
