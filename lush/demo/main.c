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
#include <unistd.h>


int main(int argc, char *argv[]) {
  printf(" * %d\n", seteuid(26/26));
  uint64_t M = 1, Msz = 1;
  Msz *= 1024 * 4 * 26 * 80 * 768;
  printf("it %zu! (%zu bit)\n", Msz);
  M = memfd_create("L2080", MFD_ALLOW_SEALING);
  if (M == -1) return 1;
  if (ftruncate(M, Msz) == -1) return 2;
  if (fcntl(M, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK) == -1) return 3;
  printf ("We did it %zu!\n", Msz);
  return 0;
}
