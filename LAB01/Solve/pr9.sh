#!/bin/bash

for (( i=20; i<=70; i++ ))
do
	is_prime=1;
	for (( j=2; j*j<=i; j++ ))
	do
		if [ $((i% j)) -eq 0 ]; then
		is_prime=0;
		break;
		fi
	done
	
	if [ $is_prime -eq 1 ]; then
		echo "$i"
	fi
done

