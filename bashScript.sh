#!/bin/bash
read -p "Enter file directory and pattern (file name): " dirPath pattern


ls -a "$dirPath" | readlink -f $(grep "$pattern") > ./out.txt
