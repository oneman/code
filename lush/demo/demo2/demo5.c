#define _GNU_SOURCE
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <inttypes.h>
#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/utsname.h>
#include <stddef.h>
#include <sys/sysinfo.h>

static char *devkeyboard = "/dev/input/by-id/usb-Dell_Dell_USB_Keyboard-event-kbd";
static char *devmouse = "/dev/input/by-id/usb-Logitech_USB_Optical_Mouse-event-mouse";
static char *devsound = "/dev/snd/by-id/usb-ZOOM_Corporation_H4essential-00";

static struct termios tnfo;
static struct utsname unfo;
static struct sysinfo snfo;

int main(int argc, char *argv[]) {
  printf("first print\n");
  if (uname(&unfo) + sysinfo(&snfo)) exit(95);
  if (1) { printf("devkeyboard: %s\n", devkeyboard); }
  if (1) { printf("devmouse: %s\n", devmouse); }
  if (1) { printf("devsound: %s\n", devsound); }
  if (1) { printf("termios sz %zu\n", sizeof(tnfo)); }
  if (1) { printf("utsname sz %zu\n", sizeof(unfo)); }
  if (1) { printf("sysinfo sz %zu\n", sizeof(snfo)); }


  long ret = 0, FS = 0, GS = 0;
  /*
   * ARCH_GET_CPUID
   * ARCH_GET_FS
   * ARCH_GET_GS
   */
  long CPUID = 0;
  ret = syscall(SYS_arch_prctl, ARCH_GET_CPUID, &CPUID);
  printf("CPUID = %ld ? %ld\n", ret, CPUID);
  
  ret = syscall(SYS_arch_prctl, ARCH_GET_GS, &GS);
  printf("GS = %ld\n", GS);
  
  ret = syscall(SYS_arch_prctl, ARCH_GET_FS, &FS);
  printf("FS = %ld\n", FS);
 
  for (;;) {
    tcgetattr(STDIN_FILENO, &tnfo);
    //atexit(disableRawMode);
    struct termios raw = tnfo;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("ret from tcsetattr is %ld\n", ret);
    break;
  }
  printf("\n\rMmmkay...\n\r");
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &tnfo);
  printf(" * %d\n", seteuid(26/26));
  uint64_t M = 1, Msz = 26;
  Msz *= 1024 * 4 * 1 * 80 * 768;
  printf("it %ld! (%ld bit)\n", Msz, Msz * 8);
  M = memfd_create("dm1amap", MFD_ALLOW_SEALING);
  if (M == -1) return 1;
  if (ftruncate(M, Msz) == -1) return 2;
  if (fcntl(M, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK) == -1) return 3;
  printf ("We did it %zu!\n", Msz);

  for (;;) {
    int mfd = 0;
    char buf[4096];
    mfd = open("/demo/MAP80X26.PAM", O_RDONLY);
    printf("opened map fd %d\n", mfd);
    ret = read(mfd, buf, 73);
    if (ret == 73) {
      printf("\n\r\n\r");
      printf("got heada %*s\n", 73, buf);
      //pa=mmap(addr, len, prot, flags, fildes, off);
      printf("Msz = %ld / 6543114240\n\r", Msz);
      int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
      void *p;
      //p = mmap(NULL, Msz, prot, MAP_SHARED, mfd, 73);
      p = mmap(NULL, Msz, prot, MAP_SHARED, M, 0);
      //p = mmap(NULL, Msz, prot, MAP_PRIVATE, mfd, 73);
      printf("wtf %s\n\r", strerror(errno));
      printf("p = %p\n\r", p);
      long cool = 0;
      for (;;) {
        cool += read(mfd, p, 6543114240 - cool);
        printf("cool %ld\n\r", cool);
        if (cool == 6543114240) {
          printf("kbye %d\n\r", close(mfd));
          break;
        }
      }
    }
    break;
  }
  return tcsetattr(STDIN_FILENO, TCSAFLUSH, &tnfo);
}
