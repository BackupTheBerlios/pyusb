#!/usr/bin/env python
#
# PyUSB setup script
#
# Copyright 2005 - 2007 Wander Lairson Costa

from distutils.core import setup, Extension
import sys
import os
from os.path import join

program_files = os.getenv('PROGRAMFILES')

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
	extra_link_args = ['/LIBPATH:' + join(program_files, 'Libusb-win32', 'lib', 'msvc')]
	extra_compile_args = ['/I' + join(program_files, 'Libusb-win32', 'include')]
# necessary to work fine in darwin
# Many thanks to James Barabas!
elif -1 != platform.find("darwin"):
	extra_link_args = ['-framework',
					   'CoreFoundation',
					   '-framework',
					   'IOKit',
					   '-L/sw/lib',
					   '-L/usr/local/lib']
	extra_compile_args = ['-I/sw/include','-I/usr/local/lib']
# Juha Torkkel has reported problems compiling on freebsd
# when libusb is in /usr/local tree. I don't know on freebsd, but
# on Linux the paths to usr/local are in $PATH.
# Thanks Juha... ;)
elif -1 != platform.find("freebsd"):
	extra_link_args = ['-L/usr/local/lib']
	extra_compile_args = ['-I/usr/local/include']
# Added by Xiaofan Chen for NetBSD
# Tested under NetBSD 4.0 and Python 2.4
elif -1 != platform.find("netbsd"):
	extra_link_args = ['-L/usr/pkg/lib']
	extra_compile_args = ['-I/usr/pkg/include']																											

usbmodule = Extension(name = 'usb',
					libraries = libraries,
					sources = ['pyusb.c'],
					extra_link_args = extra_link_args,
					extra_compile_args = extra_compile_args,
					depends = ['pyusb.h'])

setup(name = 'pyusb',
	version = '0.4.1',
	description = "USB access extension module",
	long_description =
	"""
	PyUSB provides easy USB access to python.
	The module contains classes and methods to
	support the most USB operations.
	""",
	author = 'Wander Lairson Costa',
	author_email = 'wander.lairson@gmail.com',
	url = 'http://pyusb.berlios.de',
	license = 'BSD',
	ext_modules = [usbmodule])

# vim:ts=4
