#!/bin/bash

while true
do
    echo  "Nhap so nguyen n:"

    read number;

    if [ "$number" -lt 10 ]; then
        echo "Gia tri phai lon hon hoac = 10. Nhap lai !!!"
    else
        break
    fi
done

sum=0

for((i=1; i<=number;i++)); do
    sum=$((sum + i))
done

echo "Tong cac so tu 1 den $number la: $sum"
exit 0;