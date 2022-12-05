#!/bin/sh
echo -n "Enter the path(Example './C'): "
read VAR1
echo -n "Enter the pattern(Example 'task'): "
read VAR2
ls -a $VAR1 | readlink -f $(grep $VAR2) > ./info.txt
echo "Information was written in 'info.txt'"