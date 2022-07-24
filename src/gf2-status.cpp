/* GF2 Status Command - Version 2.0 for Debian Linux
   Copyright (c) 2018-2022 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <cstdlib>
#include <iostream>
#include <string>
#include "error.h"
#include "gf2device.h"

int main(int argc, char **argv)
{
    int err, errlvl = EXIT_SUCCESS;
    GF2Device device;
    if (argc < 2) {  // If the program was called without arguments
        err = device.open();  // Open a device and get the device handle
    } else {  // Serial number was specified as argument
        err = device.open(argv[1]);  // Open the device having the specified serial number, and get the device handle
    }
    if (err == GF2Device::SUCCESS) {  // Device was successfully opened
        int errcnt = 0;
        std::string errstr;
        bool gen = device.isWaveGenEnabled(errcnt, errstr);  // AD9834 waveform generator status
        bool dac = device.isDACEnabled(errcnt, errstr);  // AD9834 internal DAC status
        bool clk = device.isClockEnabled(errcnt, errstr);  // Synchronous clock status
        bool fsel = device.getFrequencySelection(errcnt, errstr);  // Frequency selection
        bool psel = device.getPhaseSelection(errcnt, errstr);  // Phase selection
        if (errcnt > 0) {  // In case of error
            if (device.disconnected()) {  // If the device disconnected
                std::cerr << "Error: Device disconnected.\n";
            } else {
                printErrors(errstr);
            }
            errlvl = EXIT_FAILURE;
        } else {  // Operation successful
            std::cout << "Status: " << (gen ? "Running" : "Stopped") << std::endl;  // Print waveform generation status
            std::cout << "Waveform generator DAC: " << (dac ? "Enabled" : "Disabled") << std::endl;  // Print waveform generator DAC status
            std::cout << "Synchronous clock: " << (clk ? "Enabled" : "Disabled") << std::endl;  // Print synchronous clock status
            std::cout << "Analog output: " << (gen && dac ? "Active" : "Inactive") << std::endl;  // Print analog output status
            std::cout << "Digital output: " << (gen && dac && clk ? "Active" : "Inactive") << std::endl;  // Print digital output status
            std::cout << "Active frequency parameter: Frequency " << fsel << std::endl;  // Print selected frequency parameter
            std::cout << "Active phase parameter: Phase " << psel << std::endl;  // Print selected phase parameter
        }
        device.close();
    } else {  // Failed to open device
        if (err == GF2Device::ERROR_INIT) {  // Failed to initialize libusb
            std::cerr << "Error: Could not initialize libusb\n";
        } else if (err == GF2Device::ERROR_NOT_FOUND) {  // Failed to find device
            std::cerr << "Error: Could not find device.\n";
        } else if (err == GF2Device::ERROR_BUSY) {  // Failed to claim interface
            std::cerr << "Error: Device is currently unavailable.\n";
        }
        errlvl = EXIT_FAILURE;
    }
    return errlvl;
}
