#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-gf2.rules
service udev restart
echo Removing man pages...
rm -f /usr/local/share/man/man1/gf2-amp.1.gz
rm -f /usr/local/share/man/man1/gf2-amp50.1.gz
rm -f /usr/local/share/man/man1/gf2-clear.1.gz
rm -f /usr/local/share/man/man1/gf2-clkoff.1.gz
rm -f /usr/local/share/man/man1/gf2-clkon.1.gz
rm -f /usr/local/share/man/man1/gf2-dacoff.1.gz
rm -f /usr/local/share/man/man1/gf2-dacon.1.gz
rm -f /usr/local/share/man/man1/gf2-freq.1.gz
rm -f /usr/local/share/man/man1/gf2-freq0.1.gz
rm -f /usr/local/share/man/man1/gf2-freq1.1.gz
rm -f /usr/local/share/man/man1/gf2-list.1.gz
rm -f /usr/local/share/man/man1/gf2-lockotp.1.gz
rm -f /usr/local/share/man/man1/gf2-phase.1.gz
rm -f /usr/local/share/man/man1/gf2-phase0.1.gz
rm -f /usr/local/share/man/man1/gf2-phase1.1.gz
rm -f /usr/local/share/man/man1/gf2-reset.1.gz
rm -f /usr/local/share/man/man1/gf2-selfreq0.1.gz
rm -f /usr/local/share/man/man1/gf2-selfreq1.1.gz
rm -f /usr/local/share/man/man1/gf2-selphase0.1.gz
rm -f /usr/local/share/man/man1/gf2-selphase1.1.gz
rm -f /usr/local/share/man/man1/gf2-sine.1.gz
rm -f /usr/local/share/man/man1/gf2-start.1.gz
rm -f /usr/local/share/man/man1/gf2-status.1.gz
rm -f /usr/local/share/man/man1/gf2-stop.1.gz
rm -f /usr/local/share/man/man1/gf2-tri.1.gz
rmdir --ignore-fail-on-non-empty /usr/local/share/man/man1
echo Removing binaries...
make -C /usr/local/src/gf2 uninstall
echo Removing source code files...
rm -rf /usr/local/src/gf2
echo Done!
