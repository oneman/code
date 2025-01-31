#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
  srand(time(0));
  int random_bit = rand() % 2;
  printf("%d\n", random_bit);
  return 0;
}
