This directory contains all source code files required for compiling the
commands for GF2 Function Generator. A list of relevant files follows:
– common.c;
– common.h;
– gf2-amp.c;
– gf2-amp50.c;
– gf2-clear.c;
– gf2-clkoff.c;
– gf2-clkon.c;
– gf2-core.c;
– gf2-core.h;
– gf2-dacoff.c;
– gf2-dacon.c;
– gf2-freq.c;
– gf2-freq0.c;
– gf2-freq1.c;
– gf2-list.c;
– gf2-lockotp.c;
– gf2-phase.c;
– gf2-phase0.c;
– gf2-phase1.c;
– gf2-reset.c;
– gf2-selfreq0.c;
– gf2-selfreq1.c;
– gf2-selphase0.c;
– gf2-selphase1.c;
– gf2-sine.c;
– gf2-start.c;
– gf2-status.c;
– gf2-stop.c;
– gf2-tri.c;
– libusb-extra.c;
– libusb-extra.h;
– Makefile.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make all install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean all install"
if you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
