/*
 * PyUSB - Python module for USB Access
 *
 * Copyright 2005 Wander Lairson Costa
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * $Log: pyusb.c,v $
 * Revision 1.2  2005/07/29 15:29:47  wander
 * - Correct $Id$ indentifier in pyusb.c
 * - Patch to interfaceProtocol attribute, is was referencing interfaceSubClass
 * - Added attribute interfaceClass in usbenum
 *
 * Revision 1.1.1.1  2005/07/28 20:49:09  wander
 * PyUSB project
 *
 */

#include <Python.h>
#include <structmember.h>
#include <usb.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef unix
#include <unistd.h>
#endif /* unix */
#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#define STRING_ARRAY_SIZE 256

static char cvsid[] = "$Id: pyusb.c,v 1.2 2005/07/29 15:29:47 wander Exp $";

/*
 * USBError
 */
static PyObject *PyExc_USBError;

#define PyUSB_Error() PyErr_SetString(PyExc_USBError, usb_strerror())

struct _Py_usb_DeviceHandle;

/*
 * EndpointDescriptor object
 */
typedef struct _Py_usb_EndpointDescriptor {
	PyObject_HEAD
	u_int8_t address;
	u_int8_t type;
	u_int16_t maxPacketSize;
	u_int8_t interval;
	u_int8_t refresh;
} Py_usb_EndpointDescriptor;

static PyMemberDef Py_usb_EndpointDescriptor_Members[] = {
	{"address",
	 T_UBYTE,
	 offsetof(Py_usb_EndpointDescriptor, address),
	 READONLY,
	 "Endpoint address"},

	{"type",
	 T_UBYTE,
	 offsetof(Py_usb_EndpointDescriptor, type),
	 READONLY,
	 "Transfer type"},

	{"maxPacketSize",
	 T_USHORT,
	 offsetof(Py_usb_EndpointDescriptor, maxPacketSize),
	 READONLY,
	 "Maximum packet size"},

	{"interval",
	 T_UBYTE,
	 offsetof(Py_usb_EndpointDescriptor, interval),
	 READONLY,
	 "Interval polling at miliseconds"},

	{NULL}
};

static PyMethodDef Py_usb_EndpointDescriptor_Methods[] = {
	{NULL, NULL}
};

static PyTypeObject Py_usb_EndpointDescriptor_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.EndpointDescriptor",  /*tp_name*/
    sizeof(Py_usb_EndpointDescriptor), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "EndpointDescriptor object", /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_EndpointDescriptor_Methods,   /* tp_methods */
    Py_usb_EndpointDescriptor_Members,   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,						   /* destructor */
};

static void
set_EndpointDescriptor_fields(Py_usb_EndpointDescriptor *endpoint,
							struct usb_endpoint_descriptor *ep)
{
	endpoint->address = ep->bEndpointAddress;
	endpoint->type = ep->bmAttributes & 3;
	endpoint->maxPacketSize = ep->wMaxPacketSize;
	endpoint->interval = ep->bInterval;
	endpoint->refresh = ep->bRefresh;
	// endpoint->synchAddress - ep->bSynchAddress;
}

static Py_usb_EndpointDescriptor *
new_EndpointDescriptor(struct usb_endpoint_descriptor *ep)
{
	Py_usb_EndpointDescriptor *endpoint;

	endpoint = PyObject_New(Py_usb_EndpointDescriptor,
							&Py_usb_EndpointDescriptor_Type);

	if (endpoint && ep)
		set_EndpointDescriptor_fields(endpoint, ep);

	return endpoint;
}

/*
 * Endpoint Object
 */
typedef struct _Py_usb_Endpoint {
	PyObject_HEAD
	u_int8_t epdescdata[sizeof(Py_usb_EndpointDescriptor)-sizeof(PyObject)];
	struct _Py_usb_DeviceHandle *deviceHandle;
} Py_usb_Endpoint;

static PyMemberDef Py_usb_Endpoint_Members[] = {
	{NULL}
};

static PyObject *
Py_usb_Endpoint_write(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_Endpoint_read(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_Endpoint_reset(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_Endpoint_clearHalt(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyMethodDef Py_usb_Endpoint_Methods[] = {
	{"write",
	 Py_usb_Endpoint_write,
	 METH_VARARGS,
	 "Writes data according the endpoint type.\n"
	 "\tdata - sequence of data to write.\n"
	 "\ttimeout - timeout at miliseconds of the operation. Default 100.\n"
	 "Returns the number of bytes written."},

	{"read",
	 Py_usb_Endpoint_read,
	 METH_VARARGS,
	 "Reads data according the endpoint type.\n"
	 "\tsize - number of bytes to read.\n"
	 "\ttimeout - timeout at miliseconds of the operation. Default 100.\n"
	 "Returns a tuple with data read."},

	{"reset",
	 Py_usb_Endpoint_reset,
	 METH_NOARGS,
	 "Reset the endpoint."},

	{"clearHalt",
	 Py_usb_Endpoint_clearHalt,
	 METH_NOARGS,
	 "Clears any halt state on the endpoint."},

	{NULL, NULL}
};

static void
Py_usb_Endpoint_del(PyObject *self)
{
	Py_XDECREF((PyObject *) ((Py_usb_Endpoint *) self)->deviceHandle);
}

static PyTypeObject Py_usb_Endpoint_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.Endpoint",        	   /*tp_name*/
    sizeof(Py_usb_Endpoint),   /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Endpoint object",     	   /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_Endpoint_Methods,   /* tp_methods */
    Py_usb_Endpoint_Members,   /* tp_members */
    0,                         /* tp_getset */
    &Py_usb_EndpointDescriptor_Type, /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_Endpoint_del		   /* destructor */
};

static Py_usb_Endpoint *
new_Endpoint(struct usb_endpoint_descriptor *ep, struct _Py_usb_DeviceHandle *h)
{
	Py_usb_Endpoint *endpoint;

	endpoint = PyObject_NEW(Py_usb_Endpoint, &Py_usb_Endpoint_Type);

	if (endpoint) {
		if (ep) set_EndpointDescriptor_fields((Py_usb_EndpointDescriptor *) endpoint, ep);
		if (h) endpoint->deviceHandle = h;
	}

	return endpoint;
}

static void
set_Endpoint_fields(Py_usb_Endpoint *endpoint, Py_usb_EndpointDescriptor *ep)
{
	Py_usb_EndpointDescriptor *epd = (Py_usb_EndpointDescriptor *) endpoint;
	epd->address = ep->address;
	epd->type = ep->type;
	epd->maxPacketSize = ep->maxPacketSize;
	epd->interval = ep->interval;
	epd->refresh = ep->refresh;
	//epd->synchAddress - ep->synchAddress;
}

static Py_usb_Endpoint *
new_Endpoint2(Py_usb_EndpointDescriptor *ep, struct _Py_usb_DeviceHandle *h)
{
	Py_usb_Endpoint *endpoint;

	endpoint = PyObject_NEW(Py_usb_Endpoint, &Py_usb_Endpoint_Type);

	if (endpoint) {
		if (ep) set_Endpoint_fields(endpoint, ep);
		if (h) endpoint->deviceHandle = h;
	}

	return endpoint;
}

/*
 * Interface Object
 */
typedef struct _Py_usb_Interface {
	PyObject_HEAD
	u_int8_t interfaceNumber;
	u_int8_t alternateSetting;
	u_int8_t interfaceClass;
	u_int8_t interfaceSubClass;
	u_int8_t interfaceProtocol;
	u_int8_t iInterface;
	PyObject *endpoints;
} Py_usb_Interface;

static PyMemberDef Py_usb_Interface_Members[] = {
	{"interfaceNumber",
	 T_UBYTE,
	 offsetof(Py_usb_Interface, interfaceNumber),
	 READONLY,
	 "Interface number"},

	{"alternateSetting",
	 T_UBYTE,
	 offsetof(Py_usb_Interface, alternateSetting),
	 READONLY,
	 "Alternate setting number"},

	{"interfaceClass",
	 T_UBYTE,
	 offsetof(Py_usb_Interface, interfaceClass),
	 READONLY,
	 "Interface class"},

	{"interfaceSubClass",
	 T_UBYTE,
	 offsetof(Py_usb_Interface, interfaceSubClass),
	 READONLY,
	 "Interface sub-class"},

	{"interfaceProtocol",
	 T_UBYTE,
	 offsetof(Py_usb_Interface, interfaceProtocol),
	 READONLY,
	 "Interface protocol"},

	{"endpoints",
	 T_OBJECT,
	 offsetof(Py_usb_Interface, endpoints),
	 READONLY,
	 "Tuple with interface endpoints"},
	
	{NULL}
};

static void
Py_usb_Interface_del(PyObject *self)
{
	Py_XDECREF((PyObject *) ((Py_usb_Interface *) self)->endpoints);
}

static PyMethodDef Py_usb_Interface_Methods[] = {
	{NULL}
};

static PyTypeObject Py_usb_Interface_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.Interface",    	   /*tp_name*/
    sizeof(Py_usb_Interface),   /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
	0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Interface object", 	   /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_Interface_Methods,  /* tp_methods */
    Py_usb_Interface_Members,  /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_Interface_del		/* destructor */
};

static void
set_Interface_fields(Py_usb_Interface *interface,
					struct usb_interface_descriptor *i)
{
	u_int8_t index;

	interface->interfaceNumber = i->bInterfaceNumber;
	interface->alternateSetting = i->bAlternateSetting;
	interface->interfaceClass = i->bInterfaceClass;
	interface->interfaceSubClass = i->bInterfaceSubClass;
	interface->interfaceProtocol = i->bInterfaceProtocol;
	interface->iInterface = i->iInterface;

	interface->endpoints = PyTuple_New(i->bNumEndpoints);

	if (!interface->endpoints) {
		return;
	}

	for (index = 0; index < i->bNumEndpoints; ++index)
		PyTuple_SET_ITEM(interface->endpoints, index,
						(PyObject *) new_EndpointDescriptor(i->endpoint+index));
}

static Py_usb_Interface *
new_Interface(struct usb_interface_descriptor *i)
{
	Py_usb_Interface *interface;

	interface = PyObject_NEW(Py_usb_Interface, &Py_usb_Interface_Type);

	if (interface) {
		set_Interface_fields(interface, i);

		if (PyErr_Occurred()) {
			Py_XDECREF((PyObject *) interface);
			return NULL;
		}
	}

	return interface;
}

/*
 * Configuration object
 */
typedef struct _Py_usb_Configuration {
	PyObject_HEAD
	u_int16_t totalLength;
	u_int8_t value;
	u_int8_t iConfiguration;
	u_int8_t selfPowered;
	u_int8_t remoteWakeup;
	u_int16_t maxPower;
	PyObject *interfaces;
} Py_usb_Configuration;

static PyMemberDef Py_usb_Configuration_Members[] = {
	{"totalLength",
	 T_USHORT,
	 offsetof(Py_usb_Configuration, totalLength),
	 READONLY,
	 "The total bytes that the device returns"},

	{"value",
	 T_USHORT,
	 offsetof(Py_usb_Configuration, value),
	 READONLY,
	 "Configuration value"},

	{"selfPowered",
	 T_UBYTE,
	 offsetof(Py_usb_Configuration, selfPowered),
	 READONLY,
	 "True if the device is self powered"},

	{"remoteWakeup",
	 T_UBYTE,
	 offsetof(Py_usb_Configuration, remoteWakeup),
	 READONLY,
	 "True if the device supports remote wakeup feature"},

	{"maxPower",
	 T_UBYTE,
	 offsetof(Py_usb_Configuration, maxPower),
	 READONLY,
	 "Specifies the device current. This is the absolute value"},

	{"interfaces",
	 T_OBJECT,
	 offsetof(Py_usb_Configuration, interfaces),
	 READONLY,
	 "Tuple with a tuple of the configuration interfaces.\n"
	 "Each element in the first represents a sequence of the\n"
	 "alternate settings for that interface"},

	{NULL}
};

static PyMethodDef Py_usb_Configuration_Methods[] = {
	{NULL, NULL}
};

static void
Py_usb_Configuration_del(PyObject *self)
{
	Py_XDECREF((PyObject *) ((Py_usb_Configuration *) self)->interfaces);
}

static PyTypeObject Py_usb_Configuration_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.Configuration",   	   /*tp_name*/
    sizeof(Py_usb_Configuration),   /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
	0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Configuration object",    /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_Configuration_Methods,  /* tp_methods */
    Py_usb_Configuration_Members,  /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_Configuration_del	/* destructor */
};

static void
set_Configuration_fields(Py_usb_Configuration *configuration,
						struct usb_config_descriptor *config)
{
	u_int8_t i, j, k;
	PyObject *t1;

	configuration->totalLength = config->wTotalLength;
	configuration->value = config->bConfigurationValue;
	configuration->iConfiguration = config->iConfiguration;
	configuration->selfPowered = (config->bmAttributes >> 6) & 1;
	configuration->remoteWakeup = (config->bmAttributes >> 5) & 1;
	configuration->maxPower = config->MaxPower << 2;

	configuration->interfaces = PyTuple_New(config->bNumInterfaces);

	if (!configuration->interfaces) return;

	for (i = 0; i < config->bNumInterfaces; ++i) {
		k = config->interface[i].num_altsetting;
		t1 = PyTuple_New(k);
		if (!t1) return;
		for (j = 0; j < k; ++j)
			PyTuple_SET_ITEM(t1, j, (PyObject *) new_Interface(config->interface[i].altsetting+j));
		PyTuple_SET_ITEM(configuration->interfaces, i, t1);
	}
}

static Py_usb_Configuration *
new_Configuration(struct usb_config_descriptor *conf)
{
	Py_usb_Configuration *configuration;
	configuration = PyObject_NEW(Py_usb_Configuration, &Py_usb_Configuration_Type);

	if (configuration) {
		set_Configuration_fields(configuration, conf);
		if (PyErr_Occurred()) {
			Py_DECREF((PyObject *) configuration);
			return NULL;
		}
	}

	return configuration;
}

/*
 * Device object
 */
typedef struct _Py_usb_Device {
	PyObject_HEAD
	char usbVersion[STRING_ARRAY_SIZE];
	u_int8_t deviceClass;
	u_int8_t deviceSubClass;
	u_int8_t deviceProtocol;
	u_int8_t maxPacketSize;
	u_int16_t idVendor;
	u_int16_t idProduct;
	char deviceVersion[STRING_ARRAY_SIZE];
	u_int8_t iManufacturer;
	u_int8_t iProduct;
	u_int8_t iSerialNumber;
	char filename[PATH_MAX + 1];
	PyObject *configurations;
	struct usb_device *dev; // necessary for usb_open
} Py_usb_Device;

static PyMemberDef Py_usb_Device_Members[] = {
	{"usbVersion",
	 T_STRING_INPLACE,
	 offsetof(Py_usb_Device, usbVersion),
	 READONLY,
	 "String containing the usb specification release number"},

	{"deviceClass",
	 T_UBYTE,
	 offsetof(Py_usb_Device, deviceClass),
	 READONLY,
	 "Device class"},

	{"deviceSubClass",
	 T_UBYTE,
	 offsetof(Py_usb_Device, deviceSubClass),
	 READONLY,
	 "Device subclass"},

	{"deviceProtocol",
	 T_UBYTE,
	 offsetof(Py_usb_Device, deviceProtocol),
	 READONLY,
	 "Device protocol"},
	
	{"maxPacketSize",
	 T_UBYTE,
	 offsetof(Py_usb_Device, maxPacketSize),
	 READONLY,
	 "Maximum packet size for endpoint 0"},

	{"idVendor",
	 T_USHORT,
	 offsetof(Py_usb_Device, idVendor),
	 READONLY,
	 "Vendor ID"},

	{"idProduct",
	 T_USHORT,
	 offsetof(Py_usb_Device, idProduct),
	 READONLY,
	 "Product ID"},

	{"deviceVersion",
	 T_STRING_INPLACE,
	 offsetof(Py_usb_Device, deviceVersion),
	 READONLY,
	 "Device release number"},

	{"filename",
	 T_STRING_INPLACE,
	 offsetof(Py_usb_Device, filename),
	 READONLY,
	 "file name"},
	
	{"configurations",
	 T_OBJECT,
	 offsetof(Py_usb_Device, configurations),
	 READONLY,
	 "Tuple with the device configurations"},

	{NULL}
};

static PyObject *
Py_usb_Device_open(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "Not implemented");
	return NULL;
}

static PyMethodDef Py_usb_Device_Methods[] = {
	{"open",
	 Py_usb_Device_open,
	 METH_NOARGS,
	 "Open the device. Returns a DeviceHandle object."},

	{NULL, NULL}
};

static void
Py_usb_Device_del(PyObject *self)
{
	Py_XDECREF(((Py_usb_Device *) self)->configurations);
}

static PyTypeObject Py_usb_Device_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.Device",   	   	   /*tp_name*/
    sizeof(Py_usb_Device),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
	0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Device object",    	   /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_Device_Methods,     /* tp_methods */
    Py_usb_Device_Members,	   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_Device_del			/* destructor */
};

static void
set_Device_fields(Py_usb_Device *device, struct usb_device *dev)
{
	struct usb_device_descriptor *desc = &dev->descriptor;
	u_int8_t i;

	device->usbVersion[0] = ((desc->bcdUSB >> 12) & 0xf) + '0';
	device->usbVersion[1] = ((desc->bcdUSB >> 8) & 0xf) + '0';
	device->usbVersion[2] = '.';
	device->usbVersion[3] = ((desc->bcdUSB >> 4) & 0xf) + '0';
	device->usbVersion[4] = (desc->bcdUSB & 0xf) + '0';
	device->usbVersion[5] = '\0';

	device->deviceVersion[0] = ((desc->bcdDevice >> 12) & 0xf) + '0';
	device->deviceVersion[1] = ((desc->bcdDevice >> 8) & 0xf) + '0';
	device->deviceVersion[2] = '.';
	device->deviceVersion[3] = ((desc->bcdDevice >> 4) & 0xf) + '0';
	device->deviceVersion[4] = (desc->bcdDevice & 0xf) + '0';
	device->deviceVersion[5] = '\0';

	device->deviceClass = desc->bDeviceClass;
	device->deviceSubClass = desc->bDeviceSubClass;
	device->deviceProtocol = desc->bDeviceProtocol;
	device->maxPacketSize = desc->bMaxPacketSize0;
	device->idVendor = desc->idVendor;
	device->idProduct = desc->idProduct;
	device->iManufacturer = desc->iManufacturer;
	device->iProduct = desc->iProduct;
	device->iSerialNumber = desc->iSerialNumber;
	strcpy(device->filename, dev->filename);
	device->dev = dev;

	device->configurations = PyTuple_New(desc->bNumConfigurations);
	if (!device->configurations) return;

	for (i = 0; i < desc->bNumConfigurations; ++i)
		PyTuple_SET_ITEM(device->configurations, i, (PyObject *) new_Configuration(dev->config+i));
}

static Py_usb_Device *
new_Device(struct usb_device *dev)
{
	Py_usb_Device *device;

	device = PyObject_NEW(Py_usb_Device, &Py_usb_Device_Type);

	if (device) {
		set_Device_fields(device, dev);
		if (PyErr_Occurred()) {
			Py_DECREF((PyObject *) device);
			return NULL;
		}
	}
	return device;
}

/*
 * Bus Object
 */
typedef struct _Py_usb_Bus {
	PyObject_HEAD
	char dirname[PATH_MAX + 1];
	u_int32_t location;
	PyObject *devices;
} Py_usb_Bus;

static PyMemberDef Py_usb_Bus_Members[] = {
	{"dirname",
	 T_STRING_INPLACE,
	 offsetof(Py_usb_Bus, dirname),
	 READONLY,
	 ""},

	{"location",
	 T_UINT,
	 offsetof(Py_usb_Bus, location),
	 READONLY,
	 ""},

	{"devices",
	 T_OBJECT,
	 offsetof(Py_usb_Bus, devices),
	 READONLY,
	 "Tuple with the bus devices"},

	{NULL}
};

static PyMethodDef Py_usb_Bus_Methods[] = {
	{NULL, NULL}
};

static void
Py_usb_Bus_del(PyObject *self)
{
	Py_XDECREF(((Py_usb_Bus *) self)->devices);
}

static PyTypeObject Py_usb_Bus_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.Bus",   	   	   	   /*tp_name*/
    sizeof(Py_usb_Bus),        /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
	0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Bus object",    	  	   /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_Bus_Methods,        /* tp_methods */
    Py_usb_Bus_Members,	   	   /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_Bus_del			/* destructor */
};

static Py_usb_Bus *
new_Bus(struct usb_bus *b) {
	Py_usb_Bus *bus;
	u_int32_t i;
	struct usb_device *dev;

	bus = PyObject_NEW(Py_usb_Bus, &Py_usb_Bus_Type);

	if (bus) {
		bus->location = b->location;
		strcpy(bus->dirname, b->dirname);
		for(dev = b->devices, i = 0; dev; dev = dev->next) ++i;
		bus->devices = PyTuple_New(i);
		if(!bus->devices) {
			Py_DECREF((PyObject *) bus);
			return NULL;
		}
		for(dev = b->devices, i=0; dev; dev = dev->next, ++i)
			PyTuple_SET_ITEM(bus->devices, i, (PyObject *) new_Device(dev));

		if (PyErr_Occurred()) {
			Py_DECREF((PyObject *) bus);
			bus = NULL;
		}
	}
	return bus;
}

/*
 * DeviceHandle object
 */
typedef struct _Py_usb_DeviceHandle {
	PyObject_HEAD
	usb_dev_handle *deviceHandle;
} Py_usb_DeviceHandle;

static PyMemberDef Py_usb_DeviceHandle_Members[] = {
	{NULL}
};

static PyObject *
Py_usb_DeviceHandle_controlMsg(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_DeviceHandle_setConfiguration(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_DeviceHandle_claimInterface(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_DeviceHandle_releaseInterface(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_DeviceHandle_setAltInterface(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyObject *
Py_usb_DeviceHandle_getEndpoint(PyObject *self, PyObject *args)
{
	PyErr_SetString(PyExc_USBError, "not implemented");
	return NULL;
}

static PyMethodDef Py_usb_DeviceHandle_Methods[] = {
	{"controlMsg",
	 Py_usb_DeviceHandle_controlMsg,
	 METH_VARARGS,
	 ""},

	{"setConfiguration",
	 Py_usb_DeviceHandle_setConfiguration,
 	 METH_O,
	 ""},	 

	{"claimInterface",
	 Py_usb_DeviceHandle_claimInterface,
	 METH_O,
	 ""},

	{"releaseInterface",
	 Py_usb_DeviceHandle_releaseInterface,
	 METH_O,
	 ""},
	
	{"setAltInterface",
	 Py_usb_DeviceHandle_setAltInterface,
	 METH_O,
	 ""},

	{"getEndpoint",
	 Py_usb_DeviceHandle_getEndpoint,
	 METH_O,
	 ""},

	{NULL, NULL}
};

static void
Py_usb_DeviceHandle_del(PyObject *self)
{
	struct usb_dev_handle *h = ((Py_usb_DeviceHandle *) self)->deviceHandle;
	if (h) usb_close(h);
}

static PyTypeObject Py_usb_DeviceHandle_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "usb.DeviceHandle",   	   /*tp_name*/
    sizeof(Py_usb_DeviceHandle), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
	0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "DeviceHandle object",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Py_usb_DeviceHandle_Methods, /* tp_methods */
    Py_usb_DeviceHandle_Members, /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,					       /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_usb_DeviceHandle_del		/* destructor */
};

static Py_usb_DeviceHandle *
new_DeviceHandle(Py_usb_Device *device)
{
	Py_usb_DeviceHandle *dh;
	struct usb_dev_handle *h;

	dh = PyObject_NEW(Py_usb_DeviceHandle, &Py_usb_DeviceHandle_Type);

	if (dh) {
		h = usb_open(device->dev);
		if (!h) {
			PyUSB_Error();
			Py_DECREF((PyObject *) dh);
			return NULL;
		}
		dh->deviceHandle = h;
	}

	return dh;
}

/*
 * Global functions
 */

static PyObject *
busses(PyObject *self, PyObject *args)
{
	PyObject *tuple;
	struct usb_bus *bus, *b;
	u_int32_t i;

	if (usb_find_busses() < 0) {
		PyUSB_Error();
		return NULL;
	}

	if (usb_find_devices() < 0) {
		PyUSB_Error();
		return;
	}

	bus = usb_get_busses();

	if (!bus) {
		PyUSB_Error();
		return NULL;
	}

	for(i=0,b=bus;b;b=b->next) ++i;
	tuple = PyTuple_New(i);
	if (!tuple) return NULL;
	for(b=bus,i=0;b;++i,b=b->next)
		PyTuple_SET_ITEM(tuple, i, (PyObject *) new_Bus(b));

	if (PyErr_Occurred()) {
		Py_DECREF(tuple);
		return NULL;
	}
	return tuple;
}

static PyMethodDef usb_Methods[] = {
	{"busses", busses, METH_NOARGS, "Returns a tuple with the usb busses"},
	{NULL, NULL}
};

static void
addConstant(PyObject *dict, const char *name, long value)
{
	PyObject *val; 
	val = PyInt_FromLong(value); 
	if (val) { 
		PyDict_SetItemString(dict,  name, val);
		Py_DECREF(val); 
	} 
}

static void
installModuleConstants(PyObject *module)
{
	PyObject *dict;
	dict = PyModule_GetDict(module);
	addConstant(dict, "CLASS_PER_INTERFACE", USB_CLASS_PER_INTERFACE);
	addConstant(dict, "CLASS_AUDIO", USB_CLASS_AUDIO);
	addConstant(dict, "CLASS_COMM", USB_CLASS_COMM);
	addConstant(dict, "CLASS_HID", USB_CLASS_HID);
	addConstant(dict, "CLASS_PRINTER", USB_CLASS_PRINTER);
	addConstant(dict, "CLASS_MASS_STORAGE", USB_CLASS_MASS_STORAGE);
	addConstant(dict, "CLASS_HUB", USB_CLASS_HUB);
	addConstant(dict, "CLASS_DATA", USB_CLASS_DATA);
	addConstant(dict, "CLASS_VENDOR_SPEC", USB_CLASS_VENDOR_SPEC);
	addConstant(dict, "DT_DEVICE", USB_DT_DEVICE);
	addConstant(dict, "DT_CONFIG", USB_DT_CONFIG);
	addConstant(dict, "DT_STRING", USB_DT_STRING);
	addConstant(dict, "DT_INTERFACE", USB_DT_INTERFACE);
	addConstant(dict, "DT_ENDPOINT", USB_DT_ENDPOINT);
	addConstant(dict, "DT_HID", USB_DT_HID);
	addConstant(dict, "DT_REPORT", USB_DT_REPORT);
	addConstant(dict, "DT_PHYSICAL", USB_DT_PHYSICAL);
	addConstant(dict, "DT_HUB", USB_DT_HUB);
	addConstant(dict, "DT_DEVICE_SIZE", USB_DT_DEVICE_SIZE);
	addConstant(dict, "DT_CONFIG_SIZE", USB_DT_CONFIG_SIZE);
	addConstant(dict, "DT_INTERFACE_SIZE", USB_DT_INTERFACE_SIZE);
	addConstant(dict, "DT_ENDPOINT_SIZE", USB_DT_ENDPOINT_SIZE);
	addConstant(dict, "DT_ENDPOINT_AUDIO_SIZE", USB_DT_ENDPOINT_AUDIO_SIZE);
	addConstant(dict, "DT_HUB_NONVAR_SIZE", USB_DT_HUB_NONVAR_SIZE);
	addConstant(dict, "MAXENDPOINTS", USB_MAXENDPOINTS);
	addConstant(dict, "ENDPOINT_ADDRESS_MASK", USB_ENDPOINT_ADDRESS_MASK);
	addConstant(dict, "ENDPOINT_DIR_MASK", USB_ENDPOINT_DIR_MASK);
	addConstant(dict, "ENDPOINT_TYPE_MASK", USB_ENDPOINT_TYPE_MASK);
	addConstant(dict, "ENDPOINT_TYPE_CONTROL", USB_ENDPOINT_TYPE_CONTROL);
	addConstant(dict, "ENDPOINT_TYPE_ISOCHRONOUS", USB_ENDPOINT_TYPE_ISOCHRONOUS);
	addConstant(dict, "ENDPOINT_TYPE_BULK", USB_ENDPOINT_TYPE_BULK);
	addConstant(dict, "ENDPOINT_TYPE_INTERRUPT", USB_ENDPOINT_TYPE_INTERRUPT);
	addConstant(dict, "MAXINTERFACES", USB_MAXINTERFACES);
	addConstant(dict, "MAXALTSETTING", USB_MAXALTSETTING);
	addConstant(dict, "MAXCONFIG", USB_MAXCONFIG);
	addConstant(dict, "REQ_GET_STATUS", USB_REQ_GET_STATUS);
	addConstant(dict, "REQ_CLEAR_FEATURE", USB_REQ_CLEAR_FEATURE);
	addConstant(dict, "REQ_SET_FEATURE", USB_REQ_SET_FEATURE);
	addConstant(dict, "REQ_SET_ADDRESS", USB_REQ_SET_ADDRESS);
	addConstant(dict, "REQ_GET_DESCRIPTOR", USB_REQ_GET_DESCRIPTOR);
	addConstant(dict, "REQ_SET_DESCRIPTOR", USB_REQ_SET_DESCRIPTOR);
	addConstant(dict, "REQ_GET_CONFIGURATION", USB_REQ_GET_CONFIGURATION);
	addConstant(dict, "REQ_SET_CONFIGURATION", USB_REQ_SET_CONFIGURATION);
	addConstant(dict, "REQ_GET_INTERFACE", USB_REQ_GET_INTERFACE);
	addConstant(dict, "REQ_SET_INTERFACE", USB_REQ_SET_INTERFACE);
	addConstant(dict, "REQ_SYNCH_FRAME", USB_REQ_SYNCH_FRAME);
	addConstant(dict, "TYPE_STANDARD", USB_TYPE_STANDARD);
	addConstant(dict, "TYPE_CLASS", USB_TYPE_CLASS);
	addConstant(dict, "TYPE_VENDOR", USB_TYPE_VENDOR);
	addConstant(dict, "TYPE_RESERVED", USB_TYPE_RESERVED);
	addConstant(dict, "RECIP_DEVICE", USB_RECIP_DEVICE);
	addConstant(dict, "RECIP_INTERFACE", USB_RECIP_INTERFACE);
	addConstant(dict, "RECIP_ENDPOINT", USB_RECIP_ENDPOINT);
	addConstant(dict, "RECIP_OTHER", USB_RECIP_OTHER);
	addConstant(dict, "ENDPOINT_IN", USB_ENDPOINT_IN);
	addConstant(dict, "ENDPOINT_OUT", USB_ENDPOINT_OUT);
	addConstant(dict, "ERROR_BEGIN", USB_ERROR_BEGIN);
}

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif /* PyMODINIT_FUNC */

#ifdef _WIN32
#define PyMODINIT_EXPORT __declspec(dllexport)
#else
#define PyMODINIT_EXPORT
#endif /* _WIN32 */

/*
 * Entry point for the module
 */
PyMODINIT_EXPORT PyMODINIT_FUNC initusb(void)
{
	PyObject *module;

	PyExc_USBError = PyErr_NewException("usb.USBError", PyExc_IOError, NULL);
	if (!PyExc_USBError) return;

	module = Py_InitModule3("usb", usb_Methods,"USB access module");
	if (!module) return;

	if (PyType_Ready(&Py_usb_EndpointDescriptor_Type) < 0) return;
	Py_INCREF(&Py_usb_EndpointDescriptor_Type);
	PyModule_AddObject(module, "EndpointDescriptor", (PyObject *) &Py_usb_EndpointDescriptor_Type);

	if (PyType_Ready(&Py_usb_Endpoint_Type) < 0) return;
	Py_INCREF(&Py_usb_Endpoint_Type);
	PyModule_AddObject(module, "Endpoint", (PyObject *) &Py_usb_Endpoint_Type);

	if (PyType_Ready(&Py_usb_Interface_Type) < 0) return;
	Py_INCREF(&Py_usb_Interface_Type);
	PyModule_AddObject(module, "Interface", (PyObject *) &Py_usb_Interface_Type);
	
	if (PyType_Ready(&Py_usb_Configuration_Type) < 0) return;
	Py_INCREF(&Py_usb_Configuration_Type);
	PyModule_AddObject(module, "Configuration", (PyObject *) &Py_usb_Configuration_Type);

	if (PyType_Ready(&Py_usb_Device_Type) < 0) return;
	Py_INCREF(&Py_usb_Device_Type);
	PyModule_AddObject(module, "Device", (PyObject *) &Py_usb_Device_Type);

	if (PyType_Ready(&Py_usb_Bus_Type) < 0) return;
	Py_INCREF(&Py_usb_Bus_Type);
	PyModule_AddObject(module, "Bus", (PyObject *) &Py_usb_Bus_Type);

	if (PyType_Ready(&Py_usb_DeviceHandle_Type) < 0) return;
	Py_INCREF(&Py_usb_DeviceHandle_Type);
	PyModule_AddObject(module, "DeviceHandle", (PyObject *) &Py_usb_DeviceHandle_Type);

	installModuleConstants(module);

	usb_init();
}
