#!/bin/sh

NAME="Nguyen Anh Tuan"
MSSV="23521717"

echo "Nhap thong tin gom ho&ten cung MSSV cua ban vao day"
echo "Ho & Ten: "
read name
echo "MSSV:"
read mssv

if [ "$MSSV" != "$mssv" ] || [ "$name" != "$NAME" ] ; then
    echo "Access denied"
else
    echo "Xin chao $NAME mssv: $MSSV"
fi
exit 0;