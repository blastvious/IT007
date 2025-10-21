#!/bin/bash
MSSV="23521717"
NAME="Nguyen Anh Tuan"

echo "Nhap ho & ten: "
read name
echo "Nhap MSSV !"
read mssv
if [ "$mssv" == "$MSSV" ] && [ "$name" == "$NAME" ]; then
        echo "Xin chao $NAME mssv: $MSSV";
        exit 0
    fi
while [ "$mssv" != "$MSSV" ] || [ "$name" != "$NAME" ]
do
    
for((i=1;i<=5;i++))
do
    echo "Nhap lai"
    echo "Nhap ho & ten: "
    read name
    echo "Nhap MSSV !"
    read mssv
    if [ "$mssv" == "$MSSV" ] && [ "$name" == "$NAME" ]; then
        echo "Xin chao $NAME mssv: $MSSV";
        exit 0
    fi
done
    echo "Qua so lan nhap";
break;
done

exit 0