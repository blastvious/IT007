#!/bin/bash



# Ngưỡng cảnh báo (%)
FlagAlert=90

# Hàm lấy % CPU sử dụng
get_cpu_usage() {
    top -bn1 | grep "Cpu(s)" | awk '{print 100 - $8}'
}

# Hàm lấy % RAM sử dụng
get_ram_usage() {
    free | grep Mem | awk '{print $3/$2 * 100.0}'
}

# Hàm lấy dung lượng đĩa (total và free) và % sử dụng
get_disk_usage() {
    df / | tail -1 | awk '{print $5}' | tr -d '%'
}

# Hàm lấy dung lượng Disk (MB còn trống / total)
get_disk_info() {
    df -m / | tail -1 | awk '{printf "%sMB/%sMB", $4, $2}'
}

# Hàm lấy Network (KB/s) trong 1 giây
get_network_usage() {
    RX1=$(cat /sys/class/net/eth0/statistics/rx_bytes)
    TX1=$(cat /sys/class/net/eth0/statistics/tx_bytes)
    sleep 1
    RX2=$(cat /sys/class/net/eth0/statistics/rx_bytes)
    TX2=$(cat /sys/class/net/eth0/statistics/tx_bytes)
    RX_RATE=$(( ($RX2 - $RX1) / 1024 ))
    TX_RATE=$(( ($TX2 - $TX1) / 1024 ))
    echo "Down: ${RX_RATE}KB/s | Up: ${TX_RATE}KB/s"
}

# Lấy thông số
CPU=$(get_cpu_usage)
RAM=$(get_ram_usage)
DISK=$(get_disk_usage)
DISK_INFO=$(get_disk_info)
NET=$(get_network_usage)

# Hiển thị thông tin
echo "==================== SYSTEM MONITOR ===================="
echo "CPU Usage : ${CPU}%"
echo "RAM Usage : ${RAM}%"
echo "Disk Usage: ${DISK}%  (Free/Total: $DISK_INFO)"
echo "Network   : $NET"
echo "========================================================"

# Kiểm tra ngưỡng cảnh báo
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
