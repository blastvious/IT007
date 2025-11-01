#!/bin/bash




FlagAlert=90

# Lấy %CPU 
#-bn1 là một option (câu lệnh có 3 tp lệnh <op> <cac parameters file>)
# top dùng để hiển thị realtime các tiến trình
#-b: batch mode (xuất text, không)
#-n1: chỉ lấy môt lần.
#grep: tìm dòng nào giống Cpu(s)
# "|" đây là pipeline lấy dữ liệu đầu ra là dữ liệu đầu vào của câu lệnh sau
#awk trong trường hợp này là lấy 100  - đi gia trị idle (khi cpu nhàn rỗi). *$8 là giá gị nhàn rỗi được quy định hầu hết trong các phiên bản top trên linux hiện đại.
getCPUusage() {
    top -bn1 | grep "Cpu(s)" | awk '{print 100 - $8}'
}

# Lấy %Ram
# free in ra các cột total, used, free, shared, buff, cache.
#grep có chức năng tương tự tìm dòng có chữ Mem
#awke lấy dòng $3/$2 *100 lấy giá trị pipeline từ cột S3/ cột S2 ở đây S3 là giá trị ram còn trống / S2 giá trị total ram.
getRAMusage() {
    free | grep Mem | awk '{print $3/$2 * 100.0}'
}

# Lấy disk usage  (total và free) và % sử dụng
# df (disk free) hiển thị ổ gắn với / root filesystem
#tail -l lấy dòng kết quả bỏ header.
#tr (translate) -d là option xóa '%' là xóa đi kí tự %

getDISKusage() {
    df / | tail -1 | awk '{print $5}' | tr -d '%'
}

# lấy dung lượng Disk (MB còn trống / total) tương tự getDISKusage.

DISKinfo() {
    df -m / | tail -1 | awk '{printf "%sMB/%sMB", $4, $2}'
}

# lấy Network (KB/s) trong 1 giây
#Các tệp (file) như /sys/class/net/<iface>/statistics/rx_bytes là một phần của hệ thống tệp ảo (virtual filesystem) sysfs trong hạt nhân Linux (Linux Kernel)
# Chúng cung cấp một giao diện để truy cập thông tin thống kê thời gian thực về các thiết bị và hoạt động của hệ thống.
#Cụ thể chúng ta vào net thì chúng ta đang lấu Network.
#RX tổng số Byte Đã Nhận tại thời điểm 1
#TX tổng số Byte Đã Gửi tại thời điểm 1. Lưu ý nhỏ do timw waiting ngắn khả năng cao là net worl sẽ bằng 0 vì nên tải một pakage để theo dõi
#sleep 1s để tính toán các lưu lượng
getNETusage() {
    RX1=$(cat /sys/class/net/eth0/statistics/rx_bytes)
    TX1=$(cat /sys/class/net/eth0/statistics/tx_bytes)
    sleep 5
    RX2=$(cat /sys/class/net/eth0/statistics/rx_bytes)
    TX2=$(cat /sys/class/net/eth0/statistics/tx_bytes)
    RX_RATE=$(( ($RX2 - $RX1) / 1024 ))
    TX_RATE=$(( ($TX2 - $TX1) / 1024 ))
    echo "Down: ${RX_RATE}KB/s | Up: ${TX_RATE}KB/s"
}

# Gọi hàm lấy thông số
CPU=$(getCPUusage)
RAM=$(getRAMusage)
DISK=$(getDISKusage)
DISK_INFO=$(DISKinfo)
NET=$(getNETusage)


echo "==================== SYSTEM MONITOR ===================="
echo "CPU Usage : ${CPU}%"
echo "RAM Usage : ${RAM}%"
echo "Disk Usage: ${DISK}%  (Free/Total: $DISK_INFO)"
echo "Network   : $NET"
echo "========================================================"

# Kiểm tra ngưỡng cảnh báo
# Tọa mọt arr bash sau khi map vào mảng duyệt mảng để in kết quả
ALERTS=()

if (( ${CPU%.*} > FlagAlert )); then
    ALERTS+=(" CPU usage too high! (${CPU}%)")
fi

if (( ${RAM%.*} > FlagAlert )); then
    ALERTS+=(" RAM usage too high! (${RAM}%)")
fi

if (( DISK > FlagAlert )); then
    ALERTS+=(" Disk usage too high! (${DISK}%)")
fi
``
# Xuất cảnh báo nếu có
if [ ${#ALERTS[@]} -gt 0 ]; then
    echo "------------------- ALERT -------------------"
    for alert in "${ALERTS[@]}"; do
        echo "$alert"
    done
    echo "---------------------------------------------"
else
    echo " All systems normal."
fi
