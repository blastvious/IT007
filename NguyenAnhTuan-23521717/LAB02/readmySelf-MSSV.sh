#!/bin/sh
MSSV="23521717"


while true
do
    echo "Nhap MSSV !"
    read mssv

if [ "$mssv" != "$MSSV" ]; then
    echo "Nhap lai"
else 
     echo "Chinh xac!!!"
     break;
fi
   
done

exit 0