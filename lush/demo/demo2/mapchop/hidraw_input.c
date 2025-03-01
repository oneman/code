#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

void sprintx(char *dst, const char *src, int n)  {
  const char xx[]= "0123456789ABCDEF";
  for (; n > 0; --n) {
    unsigned char c = *src++;
    *dst++ = xx[c >> 4];
    *dst++ = xx[c & 0x0f];
  }
} 

int main(int argc, char **argv) {
  int id = 0, sz = 0, ret = 0;
  int keyboard = 0, mouse = 0;
  struct hidraw_report_descriptor rpt;
  char *dev = "/dev/hidraw0";
  memset(&rpt, 0, sizeof(rpt));
  if (argc > 1) dev = argv[1];
  id = open(dev, O_RDONLY | O_NONBLOCK);
  if (id < 0) {
    perror("Unable to open device");
    return 1;
  }
  ret = ioctl(id, HIDIOCGRDESCSIZE, &sz);
  if (ret < 0) {
    perror("HIDIOCGRDESCSIZE");
    return 1;
  }
  if (sz < 4) {
    perror("wtf");
    return 1;
  }
  rpt.size = sz;
  ret = ioctl(id, HIDIOCGRDESC, &rpt);
  if (ret < 0) {
    perror("HIDIOCGRDESC");
    return 1;
  }
  unsigned char *s = rpt.value;
  if ((s[0] != 5) || (s[1] != 1) || (s[2] != 9)
   || ((s[3] != 2) && (s[3] != 6))) {
    perror("not the right kind of input");
    return 1;
  }
  if (s[3] == 2) mouse = 1;
  if (s[3] == 6) keyboard = 1;
  int ed;
  struct epoll_event ev;
  struct epoll_event events[1];
  memset(&ev, 0, sizeof(ev));
  ed = epoll_create1(EPOLL_CLOEXEC);
  if (ed < 0) {
    perror("Unable to create epoll");
    return 1;
  }
  ev.events = EPOLLIN;
  ev.data.fd = id;
  ret = epoll_ctl(ed, EPOLL_CTL_ADD, id, &ev);
  if (ret == -1) {
    perror("Unable to ctl epoll");
    return 1;
  }
  for (;;) {
    ret = epoll_wait(ed, events, 1, -1);
    if (ret != 1) {
      perror("epoll_wait problem");
      return 1;
    }
    if (mouse) {
      char b[4];
      ret = read(id, b, 4);
      if (ret != 4) {
        perror("read mouse problem");
        return 1;
      }
      char out[8];
      sprintx(out, b, 4);
      write(1, out, 8);
      write(1, "\n", 1);
    }
    if (keyboard) {
      char b[8];
      ret = read(id, b, 8);
      if (ret != 8) {
        perror("read keyboard problem");
        return 1;
      }
      char out[16];
      sprintx(out, b, 8);
      write(1, out, 16);
      write(1, "\n", 1);
    }
  }
  /* yeah bye */
  close(id);
  close(ed);
  return 0;
}
