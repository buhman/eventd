#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>

#include "udev.h"

int
eventd_udev_enumerate(eventd_udev_context_t **u_ctx,
		      const char *subsystem,
		      const char *property)
{
  int err;
  eventd_udev_context_t *ctx;

  {
    ctx = malloc(sizeof(eventd_udev_context_t));
    if (!(ctx)) {
      fprintf(stderr, "malloc(): NULL\n");
      return -1;
    }
  }
  
  {
    ctx->u = udev_new();
    if (!(ctx->u)) {
      fprintf(stderr, "udev_new(): NULL\n");
      return -1;
    }
  } /* ... */

  {
    ctx->ue = udev_enumerate_new(ctx->u);
    if (!(ctx->ue)) {
      fprintf(stderr, "udev_enumerate_new(): NULL\n");
      return -1;
    }

    err = udev_enumerate_add_match_subsystem(ctx->ue, subsystem);
    if (err < 0) {
      fprintf(stderr, "udev_enumerate_add_match_subsystem(): %d\n", err);
      return err;
    }
    
    err = udev_enumerate_add_match_property(ctx->ue, property, "1");
    if (err < 0) {
      fprintf(stderr, "udev_enumerate_add_match_property(): %d\n", err);
      return err;
    }
    
    err = udev_enumerate_scan_devices(ctx->ue);
    if (err < 0) {
      fprintf(stderr, "udev_enumerate_scan_devices(): %d\n", err);
      return err;
    }    
  } /* ... */

  {
    ctx->le = udev_enumerate_get_list_entry(ctx->ue);
    if (!ctx->le) {
      fprintf(stderr, "udev_enumerate_get_list_entry(): NULL\n");
      return -1;
    }
  } /* ... */

  {
    *u_ctx = ctx;
  }
  
  return 0;
}

int
eventd_udev_next_device(eventd_udev_context_t *u_ctx,
			const char **devnode)
{
  struct udev_device *u_device = NULL;
  const char *syspath;

  while (1) {
    syspath = udev_list_entry_get_name(u_ctx->le);
    u_device = udev_device_new_from_syspath(u_ctx->u, syspath);
 
    *devnode = udev_device_get_devnode(u_device);
    if (*devnode)
      break;

    u_ctx->le = udev_list_entry_get_next(u_ctx->le);
    u_device = udev_device_unref(u_device);
  } /* ... */
  
  u_ctx->le = udev_list_entry_get_next(u_ctx->le);
  /*u_device = udev_device_unref(u_device); FIXME*/

  return u_ctx->le ? 1 : 0;
}

void
eventd_udev_unref(eventd_udev_context_t **u_ctx)
{
  do {
    (*u_ctx)->u = udev_unref((*u_ctx)->u);
  } while ((*u_ctx)->u);
  do {
    (*u_ctx)->ue = udev_enumerate_unref((*u_ctx)->ue);
  } while ((*u_ctx)->ue);

  free(*u_ctx);
  *u_ctx = NULL;
}
