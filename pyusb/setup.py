#!/usr/bin/env python
#
# PyUSB setup script
#
# Copyright 2005 Wander Lairson Costa

from distutils.core import setup, Extension
import sys

extra_link_args = []
extra_compile_args = []
platform = sys.platform.lower()
libraries = ["usb"]

# necessary to work fine in MacOS
# many thanks to Damian Staniforth! :-)
if -1 != platform.find("mac"):
	extra_link_args = ['-framework',
					   'CoreFoundation',
					   '-framework',
					   'IOKit']
elif -1 != platform.find("win32"):
	libraries = ["libusb"]	

usbmodule = Extension('usb',
					libraries = libraries,
					sources = ['pyusb.c'],
					extra_link_args = extra_link_args,
					extra_compile_args = extra_compile_args)

setup(ext_modules = [usbmodule])
