# Makefile for all GF2 commands, Debian Linux (GCC)
# Copyright (c) 2018-2022 Samuel Lourenço

# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.

# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <https://www.gnu.org/licenses/>.


# Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com


prefix = /usr/local

CC = gcc
CFLAGS = -O2 -std=c11 -Wall -pedantic
CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall -pedantic
LDFLAGS = -s
LDLIBS = -lusb-1.0
MANPAGES = gf2-amp.1 gf2-amp50.1 gf2-clear.1 gf2-clkoff.1 gf2-clkon.1 gf2-dacoff.1 gf2-dacon.1 gf2-freq.1 gf2-freq0.1 gf2-freq1.1 gf2-info.1 gf2-list.1 gf2-lockotp.1 gf2-phase.1 gf2-phase0.1 gf2-phase1.1 gf2-reset.1 gf2-selfreq0.1 gf2-selfreq1.1 gf2-selphase0.1 gf2-selphase1.1 gf2-sine.1 gf2-start.1 gf2-status.1 gf2-stop.1 gf2-tri.1
MANPAGESGZ = $(MANPAGES:=.gz)
MKDIR = mkdir -p
MV = mv -f
OBJECTS = cp2130.o error.o gf2device.o libusb-extra.o utils.o
RMDIR = rmdir --ignore-fail-on-non-empty
TARGETS = gf2-amp gf2-amp50 gf2-clear gf2-clkoff gf2-clkon gf2-dacoff gf2-dacon gf2-freq gf2-freq0 gf2-freq1 gf2-info gf2-list gf2-lockotp gf2-phase gf2-phase0 gf2-phase1 gf2-reset gf2-selfreq0 gf2-selfreq1 gf2-selphase0 gf2-selphase1 gf2-sine gf2-start gf2-status gf2-stop gf2-tri

.PHONY: all clean install uninstall

all: $(TARGETS)

$(TARGETS): % : %.o $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) *.o $(TARGETS)

install: all install-bin install-man

install-bin:
	$(MKDIR) $(DESTDIR)$(prefix)/bin && $(MV) $(TARGETS) $(DESTDIR)$(prefix)/bin/.

install-man:
	cd man && gzip -fknv9 $(MANPAGES) && $(MKDIR) $(DESTDIR)$(prefix)/share/man/man1 && $(MV) $(MANPAGESGZ) $(DESTDIR)$(prefix)/share/man/man1/.

uninstall: uninstall-man uninstall-bin clean

uninstall-bin:
	cd $(DESTDIR)$(prefix)/bin && $(RM) $(TARGETS)

uninstall-man:
	if [ -d $(DESTDIR)$(prefix)/share/man/man1 ]; then cd $(DESTDIR)$(prefix)/share/man/man1 && $(RM) $(MANPAGESGZ) && $(RMDIR) $(DESTDIR)$(prefix)/share/man/man1; fi
