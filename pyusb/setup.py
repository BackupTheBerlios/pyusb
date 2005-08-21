#!/usr/bin/env python
#
# PyUSB setup script
#
# Copyright 2005 Wander Lairson Costa

from distutils.core import setup, Extension
import sys

# necessary to work fine in MacOS
# many thanks for Damian Staniforth! :-)
if -1 != sys.platform.lower().find("mac"):
	extra_link_args = ['-framework',
					   'CoreFoundation',
					   '-framework',
					   'IOKit']
else:
	extra_link_args = []

usbmodule = Extension('usb',
					libraries = ['usb'],
					sources = ['pyusb.c'],
					extra_link_args = extra_link_args)

setup(ext_modules = [usbmodule])
