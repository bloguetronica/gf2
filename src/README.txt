This directory contains all source code files required for compiling the
commands for GF2 Function Generator. A list of relevant files follows:
– cp2130.cpp;
– cp2130.h;
– error.cpp;
– error.h;
– gf2-amp.cpp;
– gf2-amp50.cpp;
– gf2-clear.cpp;
– gf2-clkoff.cpp;
– gf2-clkon.cpp;
– gf2-dacoff.cpp;
– gf2-dacon.cpp;
– gf2device.cpp;
– gf2device.h;
– gf2-freq.cpp;
– gf2-freq0.cpp;
– gf2-freq1.cpp;
– gf2-info.cpp;
– gf2-list.cpp;
– gf2-lockotp.cpp;
– gf2-phase.cpp;
– gf2-phase0.cpp;
– gf2-phase1.cpp;
– gf2-reset.cpp;
– gf2-selfreq0.cpp;
– gf2-selfreq1.cpp;
– gf2-selphase0.cpp;
– gf2-selphase1.cpp;
– gf2-sine.cpp;
– gf2-start.cpp;
– gf2-status.cpp;
– gf2-stop.cpp;
– gf2-tri.cpp;
– libusb-extra.c;
– libusb-extra.h;
– Makefile;
– man/gf2-amp.1;
– man/gf2-amp50.1;
– man/gf2-clear.1;
– man/gf2-clkoff.1;
– man/gf2-clkon.1;
– man/gf2-dacoff.1;
– man/gf2-dacon.1;
– man/gf2-freq.1;
– man/gf2-freq0.1;
– man/gf2-freq1.1;
– man/gf2-info.1;
– man/gf2-list.1;
– man/gf2-lockotp.1;
– man/gf2-phase.1;
– man/gf2-phase0.1;
– man/gf2-phase1.1;
– man/gf2-reset.1;
– man/gf2-selfreq0.1;
– man/gf2-selfreq1.1;
– man/gf2-selphase0.1;
– man/gf2-selphase1.1;
– man/gf2-sine.1;
– man/gf2-start.1;
– man/gf2-status.1;
– man/gf2-stop.1;
– man/gf2-tri.1;
– utils.cpp;
– utils.h.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean install" if
you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
