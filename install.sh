#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/gf2/man
cp -f src/cp2130.cpp /usr/local/src/gf2/.
cp -f src/cp2130.h /usr/local/src/gf2/.
cp -f src/error.cpp /usr/local/src/gf2/.
cp -f src/error.h /usr/local/src/gf2/.
cp -f src/gf2-amp.cpp /usr/local/src/gf2/.
cp -f src/gf2-amp50.cpp /usr/local/src/gf2/.
cp -f src/gf2-clear.cpp /usr/local/src/gf2/.
cp -f src/gf2-clkoff.cpp /usr/local/src/gf2/.
cp -f src/gf2-clkon.cpp /usr/local/src/gf2/.
cp -f src/gf2-dacoff.cpp /usr/local/src/gf2/.
cp -f src/gf2-dacon.cpp /usr/local/src/gf2/.
cp -f src/gf2device.cpp /usr/local/src/gf2/.
cp -f src/gf2device.h /usr/local/src/gf2/.
cp -f src/gf2-freq.cpp /usr/local/src/gf2/.
cp -f src/gf2-freq0.cpp /usr/local/src/gf2/.
cp -f src/gf2-freq1.cpp /usr/local/src/gf2/.
cp -f src/gf2-info.cpp /usr/local/src/gf2/.
cp -f src/gf2-list.cpp /usr/local/src/gf2/.
cp -f src/gf2-lockotp.cpp /usr/local/src/gf2/.
cp -f src/gf2-phase.cpp /usr/local/src/gf2/.
cp -f src/gf2-phase0.cpp /usr/local/src/gf2/.
cp -f src/gf2-phase1.cpp /usr/local/src/gf2/.
cp -f src/gf2-reset.cpp /usr/local/src/gf2/.
cp -f src/gf2-selfreq0.cpp /usr/local/src/gf2/.
cp -f src/gf2-selfreq1.cpp /usr/local/src/gf2/.
cp -f src/gf2-selphase0.cpp /usr/local/src/gf2/.
cp -f src/gf2-selphase1.cpp /usr/local/src/gf2/.
cp -f src/gf2-sine.cpp /usr/local/src/gf2/.
cp -f src/gf2-start.cpp /usr/local/src/gf2/.
cp -f src/gf2-status.cpp /usr/local/src/gf2/.
cp -f src/gf2-stop.cpp /usr/local/src/gf2/.
cp -f src/gf2-tri.cpp /usr/local/src/gf2/.
cp -f src/GPL.txt /usr/local/src/gf2/.
cp -f src/LGPL.txt /usr/local/src/gf2/.
cp -f src/libusb-extra.c /usr/local/src/gf2/.
cp -f src/libusb-extra.h /usr/local/src/gf2/.
cp -f src/Makefile /usr/local/src/gf2/.
cp -f src/man/gf2-amp.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-amp50.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-clear.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-clkoff.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-clkon.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-dacoff.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-dacon.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-freq.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-freq0.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-freq1.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-info.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-list.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-lockotp.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-phase.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-phase0.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-phase1.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-reset.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-selfreq0.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-selfreq1.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-selphase0.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-selphase1.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-sine.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-start.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-status.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-stop.1 /usr/local/src/gf2/man/.
cp -f src/man/gf2-tri.1 /usr/local/src/gf2/man/.
cp -f src/README.txt /usr/local/src/gf2/.
cp -f src/utils.cpp /usr/local/src/gf2/.
cp -f src/utils.h /usr/local/src/gf2/.
echo Building and installing binaries and man pages...
make -C /usr/local/src/gf2 install clean
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-gf2.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8bf1", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8bf1", MODE="0666"
EOF
service udev restart
echo Done!
