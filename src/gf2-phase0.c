/* GF2 Phase0 Command - Version 1.0 for Debian Linux
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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "libusb-extra.h"

// Defines
#define EXIT_USERERR 2  // Exit status value to indicate a command usage error
#define TR_TIMEOUT 500  // Transfer timeout in milliseconds

// Function prototypes
void configure_spi_mode(libusb_device_handle *devhandle, unsigned char channel, bool cpol, bool cpha);
void disable_cs(libusb_device_handle *devhandle, unsigned char channel);
void disable_spi_delays(libusb_device_handle *devhandle, unsigned char channel);
bool isnumber(char *value);
void select_cs(libusb_device_handle *devhandle, unsigned char channel);
void set_phase0(libusb_device_handle *devhandle, unsigned short value);

// Global variables
int err_level = EXIT_SUCCESS;  // This variable is manipulated by other functions besides main()!

int main(int argc, char **argv)
{
    libusb_context *context;
    libusb_device_handle *devhandle;
    bool kernel_attached = false;
    float phase;
    unsigned short phase_code;
    if (argc < 2)  // If the program was called without arguments
    {
        fprintf(stderr, "Error: Missing argument.\nUsage: gf2-phase0 PHASE(°)\n");
        err_level = EXIT_USERERR;
    }
    else if (!isnumber(argv[1]))  // If the argument string doesn't constitute a valid number
    {
        fprintf(stderr, "Error: Argument is not a valid number.\n");
        err_level = EXIT_USERERR;
    }
    else
    {
        phase = atof(argv[1]);  // Convert the argument string into a floating point number
        if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
        {
            fprintf(stderr, "Error: Could not initialize libusb.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If libusb is initialized
        {
            if (argc == 2)  // If no serial number was specified
                devhandle = libusb_open_device_with_vid_pid(context, 0x10C4, 0x8BF1);  // Open a device and get the device handle
            else  // Serial number was specified as a second (optional) argument
                devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8BF1, (unsigned char *)argv[2]);  // Open the device having the specified serial number, and get the device handle
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
                    phase_code = (unsigned short)((fmod(phase, 360) < 0 ? 360 + fmod(phase, 360) : fmod(phase, 360)) * 4096 / 360 + 0.5);
                    configure_spi_mode(devhandle, 0, true, false);  // Clock polarity regarding channel 0 is active low (CPOL = 1) and data is valid on each falling edge (CPHA = 0)
                    disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                    select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                    set_phase0(devhandle, phase_code);  // Set the PHASE0 register to the intended value (by sending a sequence of bytes to the AD9834 waveform generator on channel 0)
                    usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
                    disable_cs(devhandle, 0);  // Disable the previously enabled chip select
                    if (err_level == 0)  // If all goes well
                        printf("Phase 0 set to %.2f°.\n", phase_code * 360.0 / 4096);
                    libusb_release_interface(devhandle, 0);  // Release the interface
                }
                if (kernel_attached)  // If a kernel driver was attached to the interface before
                    libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
                libusb_close(devhandle);  // Close the device
            }
            libusb_exit(context);  // Deinitialize libusb
        }
    }
    return err_level;
}

void configure_spi_mode(libusb_device_handle *devhandle, unsigned char channel, bool cpol, bool cpha)  // Configures the given SPI channel in respect to its clock polarity and phase
{
    unsigned char control_buf_out[2] = {
        channel,                          // Selected channel
        0x20 * cpha + 0x10 * cpol + 0x08  // Control word (specified polarity and phase, push-pull mode, 12MHz)
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x31, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x31).\n");
        err_level = EXIT_FAILURE;
    }
}

void disable_cs(libusb_device_handle *devhandle, unsigned char channel)  // Disables the chip select corresponding to the target channel
{
    unsigned char control_buf_out[2] = {
        channel,  // Selected channel
        0x00      // Corresponding chip select disabled
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x25, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x25).\n");
        err_level = EXIT_FAILURE;
    }
}

void disable_spi_delays(libusb_device_handle *devhandle, unsigned char channel)  // Disables all SPI delays for a given channel
{
    unsigned char control_buf_out[8] = {
        channel,     // Selected channel
        0x00,        // All SPI delays disabled, no CS toggle
        0x00, 0x00,  // Inter-byte,
        0x00, 0x00,  // post-assert and
        0x00, 0x00   // pre-deassert delays all set to 0us
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x33, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x33).\n");
        err_level = EXIT_FAILURE;
    }
}

bool isnumber(char *value)  // Checks if a given string constitutes a valid number
{
    bool retval = true;
    bool dp_exists = false;
    if (value[0] != '+' && value[0] != '-' && value[0] != '.' && (value[0] < '0' || value[0] > '9'))  // Number is not valid if the first character is not a '+', '-', '.', nor a digit
        retval = false;
    else
    {
        if (value[0] == '.')  // If character is a decimal point
            dp_exists = true;
        for (size_t i = 1; i < strlen(value); i++)
        {
            if ((value[i] != '.' || (value[i] == '.' && dp_exists)) && (value[i] < '0' || value[i] > '9'))  // Number is not valid if subsequent characters are not digits or if there is more than one decimal point
            {
                retval = false;
                break;
            }
            if (value[i] == '.')  // As before, if character is a decimal point
                dp_exists = true;
        }
    }
    return retval;
}

void select_cs(libusb_device_handle *devhandle, unsigned char channel)  // Enables the chip select of the target channel, disabling any others
{
    unsigned char control_buf_out[2] = {
        channel,  // Selected channel
        0x02      // Only the corresponding chip select is enabled, all the others are disabled
    };
    if (libusb_control_transfer(devhandle, 0x40, 0x25, 0x0000, 0x0000, control_buf_out, sizeof(control_buf_out), TR_TIMEOUT) != sizeof(control_buf_out))
    {
        fprintf(stderr, "Error: Failed control transfer (0x40, 0x25).\n");
        err_level = EXIT_FAILURE;
    }
}

void set_phase0(libusb_device_handle *devhandle, unsigned short value)  // Sets the PHASE0 register on the AD9834 waveform generator to a given value
{
    unsigned char write_command_buf[10] = {
        0x00, 0x00,                                   // Reserved
        0x01,                                         // Write command
        0x00,                                         // Reserved
        0x02, 0x00, 0x00, 0x00,                       // Two bytes to write
        0xC0 + (0x0F & (unsigned char)(value >> 8)),  // PHASE0 register set to the given value
        (unsigned char)value
    };
    int bytes_written;
    if (libusb_bulk_transfer(devhandle, 0x01, write_command_buf, sizeof(write_command_buf), &bytes_written, TR_TIMEOUT) != 0)
    {
        fprintf(stderr, "Error: Failed bulk OUT transfer to endpoint 1 (address 0x01).\n");
        err_level = EXIT_FAILURE;
    }
}
