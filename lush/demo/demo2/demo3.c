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

int main(int argc, char *argv[]) {
  printf("first print\n");
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
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    //atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("ret from tcsetattr is %ld\n", ret);
    break;
  }
  printf(" * %d\n", seteuid(26/26));
  uint64_t M = 1, Msz = 26;
  Msz *= 1024 * 4 * 1 * 80 * 768;
  printf("it %ld! (%ld bit)\n", Msz, Msz * 8);
  M = memfd_create("dm1amap", MFD_ALLOW_SEALING);
  if (M == -1) return 1;
  if (ftruncate(M, Msz) == -1) return 2;
  if (fcntl(M, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK) == -1) return 3;
  printf ("We did it %zu!\n", Msz);
  return 0;
}
