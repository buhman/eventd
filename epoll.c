#include <stdio.h>
#include <fcntl.h>

#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <linux/input.h>

int
eventd_epoll_add_dev(const char *devnode,
		     int epoll_fd)
{
  int err;
  struct epoll_event ev;
  char name[256] = "Unknown";
  
  ev.events = EPOLLIN;
  
  {
    ev.data.fd = open(devnode, O_RDONLY|O_NONBLOCK|O_CLOEXEC);
    if (ev.data.fd < 0) {
      perror("open()");
      return -1;
    }
  } /* ... */

  {
    err = ioctl(ev.data.fd, EVIOCGNAME(sizeof(name)), name);
    if (err < 0) {
      perror("ioctl()");
      return err;
    }

    printf("%s(%d): %s\n", devnode, ev.data.fd, name);
  } /* ... */
      
  {
    err = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    if (err < 0) {
      perror("epoll_ctl()");
      return err;
    }
  } /* ... */

  return 0;
}
