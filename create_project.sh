#!/bin/bash

if [ -z $1 ]; then
	echo "Please enter the name of the project group you would like added."
	exit 1
fi

mkdir -p $1
cd $1
printf "# $1\n\n" > README.md
printf "## Objective\n\n" >>README.md
printf "## Resources\n\n" >> README.md

