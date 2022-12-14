#!/bin/bash
#
# Uses sys.boot_completed to see if the emulator has started. This
# uses init.svc.bootanim to output better status messages, since it
# https://gitlab.com/fdroid/ci-images-client/-/blob/master/wait-for-emulator
# sometimes works even when -no-boot-anim is used with the emulator.

sec=0
timeout=900

err() {
	echo "$@"
	exit 1
}

explain() {
	if [[ "$1" =~ "not found" ]]; then
		printf "device not found"
	elif [[ "$1" =~ "offline" ]]; then
		printf "device offline"
	elif [[ "$1" =~ "running" ]]; then
		printf "booting"
	else
		printf "$1"
	fi
}

adb -e wait-for-device
adb devices

while true; do
	if [[ $sec -ge $timeout ]]; then
		err "Timeout ($timeout seconds) reached - Failed to start emulator"
	fi
	out=$(adb -e shell getprop init.svc.bootanim 2>&1 | grep -v '^\*')
	if [[ "$out" =~ "command not found" ]]; then
		err "$out"
	fi
	if [ "$(adb -e shell getprop sys.boot_completed | tr -d '\r')" = "1" ]; then
		break
	fi
	let "r = sec % 5"
	if [[ $r -eq 0 ]]; then
		echo "Waiting for emulator to start: $(explain "$out")"
	fi
	sleep 1
	let "sec++"
done

echo "Emulator is ready (took ${sec})"
