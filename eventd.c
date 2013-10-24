#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

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
      eventd_elem_volume_offset(elem, -1);
      break;
    case 60:
    case 115:
      eventd_elem_volume_offset(elem, 1);
      break;
    case 61:
    case 190:
      eventd_elem_switch_toggle(elem);
      break;
    default:
      //printf("%d, %d, %d\n", event.type, event.code, event.value);
      break;
    }
  }
}

int
main(int argc,
     char **argv)
{
  int i;
  int fd[argc - 1];
  char name[256] = "Unknown";

  for (i = 0; i < argc - 1; i++) {
    
    fd[i] = open(argv[i + 1], O_RDONLY|O_NONBLOCK);
    
    if (fd < 0)
      err(1, "evdev open failed");

    if (ioctl(fd[i], EVIOCGNAME(sizeof(name)), name) < 0)
      err(1, "evdev ioctl failed");

    printf("%s: %s\n", argv[i + 1], name);
  }
  
  if (eventd_get_mixer_elem("default", "Master", &elem) < 0)
    return 1;
  
  while (1)
    for (i = 0; i < argc - 1; i++)
      read_event(fd[i]);

  return 0;
}
