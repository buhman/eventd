#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

#include <sys/epoll.h>
#include <linux/input.h>

#include "alsa.h"
#include "udev.h"
#include "epoll.h"

static snd_mixer_elem_t *elem;
static snd_mixer_t *mixer;
static eventd_udev_context_t *udev_ctx;

static void
read_event(int fd)
{
  int err;
  struct input_event event;

  err = read(fd, &event, sizeof(struct input_event));
  if (err < 0) {
    switch (errno) {
    case EAGAIN:
      return;
      break;
    default:
      perror("read()");
      return;
      break;
    }
  }


  if (event.type == EV_KEY && (event.value == 1 || event.value == 2)) {
    switch(event.code) {
    /*
    case KEY_F1: //59
    case KEY_VOLUMEDOWN: //114
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_volume_offset(elem, -1);
      break;
    case KEY_F2: //60
    case KEY_VOLUMEUP: //115
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_volume_offset(elem, 1);
      break;
    case KEY_F3: //61
    case KEY_F20: //190
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_switch_toggle(elem);
      break;
    */
    default:
      printf("%d: %d, %d, %d\n", fd, event.type, event.code, event.value);
      break;
    }
  }
}

int
main(int argc,
     char **argv)
{
  int err, epoll_fd, epoll_eventc;
  struct epoll_event *events;

  {
    epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd < 0) {
      perror("epoll_create1()");
      return EXIT_FAILURE;
    }

    epoll_eventc = 0;
  } /* ... */

  /*
  {
    err = eventd_get_mixer_elem("default", "Master", &mixer, &elem);
    if (err < 0) {
      perror("eventd_get_mixer_elem()");
      return EXIT_FAILURE;
    }
  }  ... */

  {
    err = eventd_udev_enumerate(&udev_ctx, "input", "ID_INPUT");
    if (err < 0) {
      perror("eventd_udev_enumerate()");
      return EXIT_FAILURE;
    }
  }

  {
    const char *devnode;

    while(eventd_udev_next_device(udev_ctx, &devnode)) {

      err = eventd_epoll_add_dev(devnode, epoll_fd);
      if (err < 0) {
        fprintf(stderr, "not not looking at: %s\n", devnode);
        continue;
      }
      epoll_eventc++;
    }

  } /* ... */

  {
    events = calloc(epoll_eventc, sizeof(struct epoll_event));
    if (!events) {
      perror("calloc()");
      return EXIT_FAILURE;
    }
  } /* ... */

  {
    int n, i;

    while (1) {

      n = epoll_wait(epoll_fd, events, epoll_eventc, -1);
      for (i = 0; i < n; i++) {
	if (events[i].events & EPOLLIN) {
	  read_event(events[i].data.fd);
	}
      }
    }
  } /* ... */

  /*
 cleanup:
  for (i = 0; i > argc - 1; i++)
    close(fd[i]);
  free(fd);
  snd_mixer_close(mixer);
  eventd_udev_unref(&udev_ctx);
  */
  return 0;
}
