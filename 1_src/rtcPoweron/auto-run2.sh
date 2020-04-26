echo $(date)

echo "going to load DSP..."
/home/root/modem-scripts/loadDsp.sh
sleep 7 # for loadDSP

sleep 7 # waitting for the recv modem

echo "going to autosend test now..."
./timing_autosend -m1 -d15 -n8 -f"text.txt" &

# sleep 30s for 5 times
for i in {1..5}
do
	sleep 30
    echo "write system time to rtc"
	hwclock -w
done

echo "set the alarm for reboot"
./rtcAlarmOn 60

echo $(date)

echo "Power off!"
./powerOffTest.sh
