/* GF2 Status Command - Version 1.0 for Debian Linux
   Copyright (c) 2018 Samuel Lourenço

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
#include <libusb-1.0/libusb.h>
#include "libusb-extra.h"

// Defines
#define TR_TIMEOUT 500  // Transfer timeout in milliseconds

// Function prototypes
bool get_gpio2(libusb_device_handle *devhandle);
bool get_gpio3(libusb_device_handle *devhandle);
bool get_gpio4(libusb_device_handle *devhandle);
bool get_gpio5(libusb_device_handle *devhandle);
bool get_gpio6(libusb_device_handle *devhandle);

// Global variables
int err_level = EXIT_SUCCESS;  // This variable is manipulated by other functions besides main()!

int main(int argc, char **argv)
{
    libusb_context *context;
    libusb_device_handle *devhandle;
    bool cmp, fsel, kernel_attached = false, psel, rst, slp;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
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
                rst = get_gpio2(devhandle);  // Get the current value of the GPIO.2 pin, which corresponds to RESET on the AD9834 waveform generator
                slp = get_gpio3(devhandle);  // Get the current value of the GPIO.3 pin, which corresponds to SLEEP on the AD9834 waveform generator
                cmp = get_gpio6(devhandle);  // Get the current value of the GPIO.6 pin, which corresponds to SHDN on the TLV3501 comparator
                fsel = get_gpio4(devhandle);  // Get the current value of the GPIO.4 pin, which corresponds to FSELECT on the AD9834 waveform generator
                psel = get_gpio5(devhandle);  // Get the current value of the GPIO.5 pin, which corresponds to PSELECT on the AD9834 waveform generator          
                if (err_level == 0)  // If all goes well
                    printf("Status: %s\n", rst ? "Stopped" : "Running");  // Print waveform generation status
                    printf("Waveform generator DAC: %s\n", slp ? "Disabled" : "Enabled");  // Print waveform generator DAC status
                    printf("Synchronous clock: %s\n", cmp ? "Disabled" : "Enabled");  // Print synchronous clock status
                    printf("Analog output: %s\n", rst || slp ? "Inactive" : "Active");  // Print analog output status
                    printf("Digital output: %s\n", rst || slp || cmp ? "Inactive" : "Active");  // Print digital output status
                    printf("Active frequency parameter: Frequency %d\n", fsel);  // Print selected frequency parameter
                    printf("Active phase parameter: Phase %d\n", psel);  // Print selected phase parameter
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

bool get_gpio2(libusb_device_handle *devhandle)  // Gets the current value of the GPIO.2 pin on the CP2130
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x20).\n");
        err_level = EXIT_FAILURE;
    }
    return ((0x20 & control_buf_in[1]) != 0x00);  // Returns one if bit 5 of byte 1, which corresponds to the GPIO.2 pin, is not set to zero
}

bool get_gpio3(libusb_device_handle *devhandle)  // Gets the current value of the GPIO.3 pin on the CP2130
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x20).\n");
        err_level = EXIT_FAILURE;
    }
    return ((0x40 & control_buf_in[1]) != 0x00);  // Returns one if bit 6 of byte 1, which corresponds to the GPIO.3 pin, is not set to zero
}

bool get_gpio4(libusb_device_handle *devhandle)  // Gets the current value of the GPIO.4 pin on the CP2130
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x20).\n");
        err_level = EXIT_FAILURE;
    }
    return ((0x80 & control_buf_in[1]) != 0x00);  // Returns one if bit 7 of byte 1, which corresponds to the GPIO.4 pin, is not set to zero
}

bool get_gpio5(libusb_device_handle *devhandle)  // Gets the current value of the GPIO.5 pin on the CP2130
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x20).\n");
        err_level = EXIT_FAILURE;
    }
    return ((0x01 & control_buf_in[0]) != 0x00);  // Returns one if bit 0 of byte 0, which corresponds to the GPIO.5 pin, is not set to zero
}

bool get_gpio6(libusb_device_handle *devhandle)  // Gets the current value of the GPIO.6 pin on the CP2130
{
    unsigned char control_buf_in[2];
    if (libusb_control_transfer(devhandle, 0xC0, 0x20, 0x0000, 0x0000, control_buf_in, sizeof(control_buf_in), TR_TIMEOUT) != sizeof(control_buf_in))
    {
        fprintf(stderr, "Error: Failed control transfer (0xC0, 0x20).\n");
        err_level = EXIT_FAILURE;
    }
    return ((0x04 & control_buf_in[0]) != 0x00);  // Returns one if bit 2 of byte 0, which corresponds to the GPIO.6 pin, is not set to zero
}
