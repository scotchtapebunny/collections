#!/bin/sh -e

export AVD_SDK=$1
export AVD_TAG=$2
export AVD_ARCH=$3
export AVD_PACKAGE="system-images;android-${AVD_SDK};${AVD_TAG};${AVD_ARCH}"

# https://androidstudio.googleblog.com/2019/02/emulator-2818-canary.html
if [ -n "$emulator" ]; then
    echo "using user-provided emulator binary path"
elif [ $AVD_ARCH = "x86" ] \
	 && [ -x $ANDROID_HOME/emulator/qemu/linux-x86_64/qemu-system-i386-headless ] \
	 && which emulator-headless; then
    emulator=emulator-headless
elif [ $AVD_ARCH = "armeabi-v7a" ] \
	 && [ -x $ANDROID_HOME/emulator/qemu/linux-x86_64/qemu-system-armel-headless ] \
	 && which emulator-headless; then
    emulator=emulator-headless
elif [ -x $ANDROID_HOME/emulator/qemu/linux-x86_64/qemu-system-${AVD_ARCH}-headless ] \
	 && which emulator-headless; then
    emulator=emulator-headless
else
    emulator=emulator
fi

set -x

# This block of packages comes directly from the Google setup:
# https://github.com/google/android-emulator-container-scripts/blob/25bce20caa/emu/templates/Dockerfile#L23
# mesa-utils needs to be installed instead of libgl1, so that libgl1
# comes from stretch and not stretch-backports.
if [ "emulator" = $emulator ]; then
    apt-get update
    apt-get -qy install --no-install-recommends \
	    libc6 \
	    libdbus-1-3 \
	    libfontconfig1 \
	    libgcc1 \
	    libglu1 \
	    libnss3 \
	    libpulse0 \
	    libtinfo5 \
	    libx11-6 \
	    libxcb1 \
	    libxcomposite1 \
	    libxcursor1 \
	    libxdamage1 \
	    libxext6 \
	    libxfixes3 \
	    pulseaudio \
	    socat \
	    zlib1g
fi

avd_name="avd_${AVD_SDK}_${AVD_TAG}_${AVD_ARCH}"
if ! avdmanager list avd | grep "Name. $avd_name$"; then
    rm -rf ~/.android/avd  $ANDROID_HOME/system-images;
    echo y | sdkmanager "$AVD_PACKAGE" "platforms;android-$AVD_SDK" > /dev/null;
    echo no | avdmanager create avd --name $avd_name --tag "$AVD_TAG" --package "$AVD_PACKAGE" --sdcard 64M --device "pixel_4_xl";
    export RAMSIZE="`sed -n 's,^MemAvailable:[^0-9]*\([0-9][0-9]*\)[^0-9]*$,\1,p' /proc/meminfo`";
    if [ $RAMSIZE -le 2000000 ]; then
        sed -i '/^hw\.ramSize\s*=.*/d' ~/.android/avd/*.avd/config.ini;
        echo "hw.ramSize=1024" >> ~/.android/avd/*.avd/config.ini;
    fi;
    avdmanager list avd;
fi

DEFAULT_FLAGS='
 -gpu swiftshader_indirect
 -no-audio
 -no-boot-anim
 -no-snapstorage
 -no-window
 -skip-adb-auth
 -verbose
 -wipe-data
 -shell-serial file:kernel.log
 -logcat-output logcat.txt
'

grep -v '^License' $ANDROID_HOME/system-images/android-$AVD_SDK/$AVD_TAG/$AVD_ARCH/source.properties
$emulator -version
$emulator -accel-check || true

if [ -n "$flags" ]; then
    echo "using preconfigured emulator flags"
elif [ -w /dev/kvm ]; then
    flags="$DEFAULT_FLAGS"
else
    echo "No KVM, using -no-accel"
    flags="-no-accel $DEFAULT_FLAGS"
fi

$emulator -avd $avd_name $flags &

echo
