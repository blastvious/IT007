#!/bin/bash

echo "Nhap chuoi ban muon kiem tra"

read string

file="test.txt"
output="output.txt"

if [ ! -f "$file" ]; then
    echo "File $file khong ton tai!" > output.txt
    exit 1
fi

found=0

while read line
do
    if [[ "$line" == *"$string"* ]]; then
        found=1
        break
    fi
done < "$file"

if [ $found -eq 1 ]; then
    echo "Chuoi $string ton tai trong file $file." >> "$output"
else
    echo "Chuoi $string khong ton tai trong file $file." >> "$output"
fi


cat "$output"