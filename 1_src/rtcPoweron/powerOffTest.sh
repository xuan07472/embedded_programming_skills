#!/bin/bash
echo 3 > /sys/class/gpio/unexport
echo 3 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio3/direction
cat /sys/class/gpio/gpio3/direction
cat /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

echo 5 > /sys/class/gpio/unexport
echo 5 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio5/direction
cat /sys/class/gpio/gpio5/direction 
cat /sys/class/gpio/gpio5/value            
echo 0 > /sys/class/gpio/gpio5/value

## gpio3 is clk, gpio5 is data
## power down sequence: 010110
# 0
echo 0 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

# 1
echo 1 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

# 0
echo 0 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

# 1
echo 1 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

# 1
echo 1 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value

# 0
echo 0 > /sys/class/gpio/gpio5/value
echo 1 > /sys/class/gpio/gpio3/value
echo 0 > /sys/class/gpio/gpio3/value
