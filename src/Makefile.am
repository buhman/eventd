bin_PROGRAMS = eventd

eventd_CFLAGS = -pedantic -D_GNU_SOURCE $(WARNING_CFLAGS)

eventd_SOURCES = alsa.h alsa.c \
	epoll.h epoll.c \
	eventd.c \
	udev.h udev.c

eventd_LDADD = -ludev -lasound
