bin_PROGRAMS = eventd

AM_CFLAGS = -pedantic -D_GNU_SOURCE $(WARNING_CFLAGS)

eventd_SOURCES = alsa.h alsa.c \
	epoll.h epoll.c \
	eventd.c \
	udev.h udev.c

eventd_CFLAGS = \
	$(AM_CFLAGS) \
	$(UDEV_CFLAGS) \
	$(ALSA_CFLAGS)

eventd_LDADD = \
	$(LTLIBINTL) \
	$(UDEV_LIBS) \
	$(ALSA_LIBS)
