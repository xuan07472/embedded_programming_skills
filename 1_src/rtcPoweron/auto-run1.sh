echo $(date)

echo "going to load DSP..."
/home/root/modem-scripts/loadDsp.sh
sleep 7 # for loadDSP

echo "starting to autorecv test..."
./timing_autorecv &

# sleep 30s for 5 times
for i in {1..5}
do
    sleep 30
    echo "write system time to rtc"
    hwclock -w
done

./rtcAlarmOn 60

echo $(date)

echo "Power off!"
./powerOffTest.sh
