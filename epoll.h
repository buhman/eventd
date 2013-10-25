#pragma once

int
eventd_epoll_add_dev(const char *devnode,
		     int epoll_fd);
