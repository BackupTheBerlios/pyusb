#!/usr/bin/env python
#
# PyUSB setup script
#
# Copyright 2005 Wander Lairson Costa

from distutils.core import setup, Extension

usbmodule = Extension('usb',
					libraries = ['usb'],
					sources = ['pyusb.c'])

setup(ext_modules = [usbmodule])
