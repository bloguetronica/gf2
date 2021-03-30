#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/gf2
cp -f src/common.c /usr/local/src/gf2/.
cp -f src/common.h /usr/local/src/gf2/.
cp -f src/gf2-amp.c /usr/local/src/gf2/.
cp -f src/gf2-amp50.c /usr/local/src/gf2/.
cp -f src/gf2-clear.c /usr/local/src/gf2/.
cp -f src/gf2-clkoff.c /usr/local/src/gf2/.
cp -f src/gf2-clkon.c /usr/local/src/gf2/.
cp -f src/gf2-core.c /usr/local/src/gf2/.
cp -f src/gf2-core.h /usr/local/src/gf2/.
cp -f src/gf2-dacoff.c /usr/local/src/gf2/.
cp -f src/gf2-dacon.c /usr/local/src/gf2/.
cp -f src/gf2-freq.c /usr/local/src/gf2/.
cp -f src/gf2-freq0.c /usr/local/src/gf2/.
cp -f src/gf2-freq1.c /usr/local/src/gf2/.
cp -f src/gf2-list.c /usr/local/src/gf2/.
cp -f src/gf2-lockotp.c /usr/local/src/gf2/.
cp -f src/gf2-phase.c /usr/local/src/gf2/.
cp -f src/gf2-phase0.c /usr/local/src/gf2/.
cp -f src/gf2-phase1.c /usr/local/src/gf2/.
cp -f src/gf2-reset.c /usr/local/src/gf2/.
cp -f src/gf2-selfreq0.c /usr/local/src/gf2/.
cp -f src/gf2-selfreq1.c /usr/local/src/gf2/.
cp -f src/gf2-selphase0.c /usr/local/src/gf2/.
cp -f src/gf2-selphase1.c /usr/local/src/gf2/.
cp -f src/gf2-sine.c /usr/local/src/gf2/.
cp -f src/gf2-start.c /usr/local/src/gf2/.
cp -f src/gf2-status.c /usr/local/src/gf2/.
cp -f src/gf2-stop.c /usr/local/src/gf2/.
cp -f src/gf2-tri.c /usr/local/src/gf2/.
cp -f src/GPL.txt /usr/local/src/gf2/.
cp -f src/LGPL.txt /usr/local/src/gf2/.
cp -f src/libusb-extra.c /usr/local/src/gf2/.
cp -f src/libusb-extra.h /usr/local/src/gf2/.
cp -f src/Makefile /usr/local/src/gf2/.
cp -f src/README.txt /usr/local/src/gf2/.
echo Building and installing binaries...
make -C /usr/local/src/gf2 all install clean
echo Installing man pages...
mkdir -p /usr/local/share/man/man1
cp -f man/gf2-amp.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-amp50.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-clear.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-clkoff.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-clkon.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-dacoff.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-dacon.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-freq.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-freq0.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-freq1.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-list.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-lockotp.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-phase.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-phase0.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-phase1.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-reset.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-selfreq0.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-selfreq1.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-selphase0.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-selphase1.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-sine.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-start.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-status.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-stop.1.gz /usr/local/share/man/man1/.
cp -f man/gf2-tri.1.gz /usr/local/share/man/man1/.
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-gf2.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8bf1", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8bf1", MODE="0666"
EOF
service udev restart
echo Done!
