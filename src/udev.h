#include <libudev.h>

typedef struct eventd_udev_context eventd_udev_context_t;
struct eventd_udev_context {
  struct udev *u;
  struct udev_enumerate *ue;
  struct udev_list_entry *le;
};

int
eventd_udev_enumerate(eventd_udev_context_t **u_ctx,
		      const char *subsystem,
		      const char *property);

int
eventd_udev_next_device(eventd_udev_context_t *u_ctx,
			const char **devnode);

void
eventd_udev_unref(eventd_udev_context_t **u_ctx);
