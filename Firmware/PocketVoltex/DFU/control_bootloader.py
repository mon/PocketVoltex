"""
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
"""

"""
    Front-end programmer for the LUFA HID class bootloader.

    Usage:
        python hid_bootloader_loader.py <Device> <Input>.hex

    Example:
        python hid_bootloader_loader.py at90usb1287 Mouse.hex

    Requires the pywinusb (https://pypi.python.org/pypi/pywinusb/) and
    IntelHex (http://bialix.com/intelhex/) libraries.
"""

import sys
from intelhex import IntelHex
import time

# new bootloader
import usb1

ENDPOINT_SIZE = 32

WEBUSB_ID = 0x01
MS_OS_ID = 0x02
CONFIG_ID = 0x03

COMMAND_VERSION           = 0x03
COMMAND_STARTAPPLICATION  = 0x04
COMMAND_PROGRAM           = 0x05

BOOTLOADER_VERSION = 65000

# Device information table
device_info_map = dict()
device_info_map['at90usb1287'] = {'page_size': 256, 'flash_kb': 128}
device_info_map['at90usb1286'] = {'page_size': 256, 'flash_kb': 128}
device_info_map['at90usb647']  = {'page_size': 256, 'flash_kb': 64}
device_info_map['at90usb646']  = {'page_size': 256, 'flash_kb': 64}
device_info_map['atmega32u4']  = {'page_size': 128, 'flash_kb': 32}
device_info_map['atmega32u2']  = {'page_size': 128, 'flash_kb': 32}
device_info_map['atmega16u4']  = {'page_size': 128, 'flash_kb': 16}
device_info_map['atmega16u2']  = {'page_size': 128, 'flash_kb': 16}
device_info_map['at90usb162']  = {'page_size': 128, 'flash_kb': 16}
device_info_map['atmega8u2']   = {'page_size': 128, 'flash_kb': 8}
device_info_map['at90usb82']   = {'page_size': 128, 'flash_kb': 8}

def get_version(device):
    ver = device.controlRead(usb1.libusb1.LIBUSB_REQUEST_TYPE_VENDOR | usb1.libusb1.LIBUSB_RECIPIENT_DEVICE, CONFIG_ID, 0, COMMAND_VERSION, 2)
    return ver[0] | ver[1] << 8

def get_device_handle(context):
    voltex = None
    for dev in context.getDeviceList():
        if dev.getVendorID() == 0x16D0 and dev.getProductID() == 0x0A6D:
            # because there is the pseudo-composite-parent we ignore
            try:
                voltex = dev.open()
                print "Open success!"
            except:
                print "Open fail"
    return voltex

def bootloader_boot(device):
    with device.claimInterface(0):
        device.bulkWrite(1, [42] + [0]*(ENDPOINT_SIZE-1))
    return True

def send_page_data(device, address, data):
    device.controlWrite(usb1.libusb1.LIBUSB_REQUEST_TYPE_VENDOR | usb1.libusb1.LIBUSB_RECIPIENT_DEVICE,
            CONFIG_ID, address, COMMAND_PROGRAM, data)


def program_device(hex_data, device_info):
    context = usb1.USBContext()
    device = get_device_handle(context)

    if device is None:
        print("No valid device found.")
        sys.exit(1)

    if get_version(device) != BOOTLOADER_VERSION:
        print("Device not in bootloader. TODO: reboot")
        sys.exit(1)

    try:
        print("Connected to bootloader.")

        # Program in all data from the loaded HEX file, in a number of device
        # page sized chunks
        for addr in range(0, hex_data.maxaddr(), device_info['page_size']):
            # Compute the address range of the current page in the device
            current_page_range = range(addr, addr+device_info['page_size'])

            # Extract the data from the hex file at the specified start page
            # address and convert it to a regular list of bytes
            page_data = [hex_data[i] for i in current_page_range]

            print("Writing address 0x%04X-0x%04X" % (current_page_range[0], current_page_range[-1]))

            # Devices with more than 64KB of flash should shift down the page
            # address so that it is 16-bit (page size is guaranteed to be
            # >= 256 bytes so no non-zero address bits are discarded)
            if device_info['flash_kb'] < 64:
                send_page_data(device, addr, page_data)
            else:
                send_page_data(device, addr >> 8, page_data)

        # Once programming is complete, start the application via a dummy page
        # program to the page address 0xFFFF
        print("Programming complete, starting application.")
        try:
            device.controlWrite(usb1.libusb1.LIBUSB_REQUEST_TYPE_VENDOR | usb1.libusb1.LIBUSB_RECIPIENT_DEVICE,
                CONFIG_ID, 0, COMMAND_STARTAPPLICATION, []) # reboot, ignore pipe error
        except usb1.USBErrorPipe:
            pass

    finally:
        device.close()
        context.close()


if __name__ == '__main__':
    # Load the specified HEX file
    try:
        hex_data = IntelHex(sys.argv[2])
    except:
        print("Could not open the specified HEX file.")
        sys.exit(1)

    # Retrieve the device information entry for the specified device
    try:
        device_info = device_info_map[sys.argv[1]]
    except:
        print("Unknown device name specified.")
        sys.exit(1)

    program_device(hex_data, device_info)
