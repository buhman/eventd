#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/select.h>
#include <math.h>

#include "alsa.h"

static snd_mixer_elem_t *elem;

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
  
  if (event.type == 1 && (event.value == 1 || event.value == 2)) {
    switch(event.code) {
    case 59:
    case 114:
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_volume_offset(elem, -1);
      break;
    case 60:
    case 115:
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_volume_offset(elem, 1);
      break;
    case 61:
    case 190:
      printf("fd %d: keycode %d: ", fd, event.code);
      eventd_elem_switch_toggle(elem);
      break;
    default:
      /*printf("%d, %d, %d\n", event.type, event.code, event.value);*/
      break;
    }
  }
}

int
main(int argc,
     char **argv)
{
  int i;
  int nfds = 0;
  int fd[argc - 1];
  char name[256] = "Unknown";

  fd_set fds;
  FD_ZERO(&fds);
  
  for (i = 0; i < argc - 1; i++) {
    
    fd[i] = open(argv[i + 1], O_RDONLY|O_NONBLOCK);
    FD_SET(fd[i], &fds);
    nfds = fmax(nfds, fd[i]);
    
    if (fd[i] < 0)
      err(1, "evdev open failed");

    if (ioctl(fd[i], EVIOCGNAME(sizeof(name)), name) < 0)
      err(1, "evdev ioctl failed");

    printf("%s(%d): %s\n", argv[i + 1], fd[i], name);
  }
  
  if (eventd_get_mixer_elem("default", "Master", &elem) < 0)
    return 1;

  {
    while (1) {
      select(nfds + 2, &fds, NULL, NULL, NULL);
      for (i = 0; i < argc - 1; i++) {
	read_event(fd[i]);
	FD_SET(fd[i], &fds);
      }
    }
  } /* ... */
  
  return 0;
}
