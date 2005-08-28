#ifndef __pyusb_h__
#define __pyusb_h__

#include <Python.h>
#include <structmember.h>
#include <usb.h>

#define STRING_ARRAY_SIZE 256

#define PYUSB_STATIC static

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

/*
 * Endpoint Object
 */
typedef struct _Py_usb_Endpoint {
	PyObject_HEAD
	u_int8_t epdescdata[sizeof(Py_usb_EndpointDescriptor)-sizeof(PyObject)];
	struct _Py_usb_DeviceHandle *deviceHandle;
} Py_usb_Endpoint;

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

/*
 * Bus Object
 */
typedef struct _Py_usb_Bus {
	PyObject_HEAD
	char dirname[PATH_MAX + 1];
	u_int32_t location;
	PyObject *devices;
} Py_usb_Bus;

/*
 * DeviceHandle object
 */
typedef struct _Py_usb_DeviceHandle {
	PyObject_HEAD
	usb_dev_handle *deviceHandle;
} Py_usb_DeviceHandle;

/*
 * Functions prototypes
 */

PYUSB_STATIC void set_EndpointDescriptor_fields(
	Py_usb_EndpointDescriptor *endpoint,
	struct usb_endpoint_descriptor *ep
	);

PYUSB_STATIC Py_usb_EndpointDescriptor *new_EndpointDescriptor(
	struct usb_endpoint_descriptor *ep
	);

PYUSB_STATIC PyObject *Py_usb_Endpoint_write(
	PyObject *self, PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_Endpoint_read(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_Endpoint_reset(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_Endpoint_clearHalt(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC Py_usb_Endpoint *new_Endpoint(
	struct usb_endpoint_descriptor *ep,
	struct _Py_usb_DeviceHandle *h
	);

PYUSB_STATIC Py_usb_Endpoint *new_Endpoint2(
	Py_usb_EndpointDescriptor *ep,
	struct _Py_usb_DeviceHandle *h
	);

PYUSB_STATIC void Py_usb_Interface_del(
	PyObject *self
	);

PYUSB_STATIC void set_Interface_fields(
	Py_usb_Interface *interface,
	struct usb_interface_descriptor *i
	);

PYUSB_STATIC Py_usb_Interface *new_Interface(
	struct usb_interface_descriptor *i
	);

PYUSB_STATIC void set_Configuration_fields(
	Py_usb_Configuration *configuration,
	struct usb_config_descriptor *config
	);

PYUSB_STATIC Py_usb_Configuration *new_Configuration(
	struct usb_config_descriptor *conf
	);

PYUSB_STATIC PyObject *Py_usb_Device_open(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC void set_Device_fields(
	Py_usb_Device *device,
	struct usb_device *dev
	);

PYUSB_STATIC Py_usb_Device *new_Device(
	struct usb_device *dev
	);

PYUSB_STATIC Py_usb_Bus *new_Bus(
	struct usb_bus *b
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_controlMsg(
	PyObject *self,
	PyObject *args,
	PyObject *kwds
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_setConfiguration(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_claimInterface(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_releaseInterface(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_setAltInterface(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC PyObject *Py_usb_DeviceHandle_getEndpoint(
	PyObject *self,
	PyObject *args
	);

PYUSB_STATIC Py_usb_DeviceHandle *new_DeviceHandle(
	Py_usb_Device *device
	);

PYUSB_STATIC PyObject *busses(
	PyObject *self,
	PyObject *args
	);

#endif /* __pyusb_h__ */