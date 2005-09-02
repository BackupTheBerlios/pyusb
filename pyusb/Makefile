# This is used only for development, ignore it

LIBUSB := $(shell libusb-config --libs)
BIN := usb.so
OBJS := pyusb.o

override CFLAGS += -Wall -g -fPIC -fno-strict-aliasing -Wno-unused \
					-I/usr/include/python2.4 -pthread
override LDFLAGS += -pthread -shared $(LIBUSB)

all: $(BIN)

clean:
	-rm -f $(OBJS)

distclean: clean
	-rm -f $(BIN)

.PHONY: all clean distclean

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS): pyusb.h
