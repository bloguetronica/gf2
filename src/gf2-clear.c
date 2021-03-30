/* GF2 Clear Command - Version 1.1 for Debian Linux
   Copyright (c) 2018-2019 Samuel Lourenço

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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "gf2-core.h"
#include "libusb-extra.h"

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    libusb_context *context;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
        libusb_device_handle *devhandle;
        if (argc < 2)  // If the program was called without arguments
            devhandle = libusb_open_device_with_vid_pid(context, 0x10C4, 0x8BF1);  // Open a device and get the device handle
        else  // Serial number was specified as argument
            devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8BF1, (unsigned char *)argv[1]);  // Open the device having the specified serial number, and get the device handle
        if (devhandle == NULL)  // If the previous operation fails to get a device handle
        {
            fprintf(stderr, "Error: Could not find device.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If the device is successfully opened and a handle obtained
        {
            bool kernel_attached = false;
            if (libusb_kernel_driver_active(devhandle, 0) != 0)  // If a kernel driver is active on the interface
            {
                libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver
                kernel_attached = true;  // Flag that the kernel driver was attached
            }
            if (libusb_claim_interface(devhandle, 0) != 0)  // Claim the interface. In case of failure
            {
                fprintf(stderr, "Error: Device is currently unavailable.\n");
                err_level = EXIT_FAILURE;
            }
            else  // If the interface is successfully claimed
            {
                configure_spi_mode(devhandle, 0, CPOL1, CPHA0);  // Clock polarity regarding channel 0 is active low (CPOL = 1) and data is valid on each falling edge (CPHA = 0)
                disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                configure_spi_mode(devhandle, 1, CPOL0, CPHA1);  // Clock polarity regarding channel 1 is active high (CPOL = 0) and data is valid on each falling edge (CPHA = 1)
                disable_spi_delays(devhandle, 1);  // Disable all SPI delays for channel 1
                set_gpio2(devhandle, false);  // If not already, set GPIO.2 to a logical low in preparation for reset
                select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                setup(devhandle);  // Configure the AD9834 so that it acknowledges reset by pin
                usleep(100);  // Wait 100us, in order to prevent possible errors while setting GPIO.2 high (workaround)
                set_gpio2(devhandle, true);  // Set GPIO.2 to a logical high to enable reset
                clear_registers(devhandle);  // Clear all waveform generation registers (by sending a specific sequence of bytes to the AD9834 waveform generator on channel 0)
                usleep(100);  // Wait 100us, in order to prevent possible errors while switching the chip select (workaround)
                select_cs(devhandle, 1);  // Enable the chip select corresponding to channel 1, and disable the one corresponding to channel 0 (the previously selected channel)
                set_amplitude(devhandle, 0);  // Set the amplitude to zero (AD5310 DAC on channel 1)
                usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the previouly enabled chip select (workaround)
                disable_cs(devhandle, 1);  // Disable the chip select corresponding to channel 1, which is the only one that is active to this point
                set_gpio3(devhandle, false);  // Set GPIO.3 to a logical low to enable the AD9834 internal DAC
                set_gpio4(devhandle, false);  // Set GPIO.4 to a logical low, so that the FREQ0 register defines the frequency of the AD9834
                set_gpio5(devhandle, false);  // Set GPIO.5 to a logical low, so that the PHASE0 register defines the phase of the AD9834
                set_gpio6(devhandle, false);  // Set GPIO.6 to a logical low to enable the synchronous clock generation (TLV3501 comparator)
                set_gpio2(devhandle, false);  // Set GPIO.2 to a logical low to disable reset (the waveform generator is now enabled)
                if (err_level == 0)  // If all goes well
                    printf("All settings cleared.\n");
                libusb_release_interface(devhandle, 0);  // Release the interface
            }
            if (kernel_attached)  // If a kernel driver was attached to the interface before
                libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
            libusb_close(devhandle);  // Close the device
        }
        libusb_exit(context);  // Deinitialize libusb
    }
    return err_level;
}
