#include <stdio.h>
#include <time.h>

int main() {
  printf("%zu = sizeof(char);\n", sizeof(char));
  printf("%zu = sizeof(short);\n", sizeof(short));
  printf("%zu = sizeof(int);\n", sizeof(int));
  printf("%zu = sizeof(long);\n", sizeof(long));
  printf("%zu = sizeof(float);\n", sizeof(float));
  printf("%zu = sizeof(time_t);\n", sizeof(time_t));
  printf("%zu = sizeof(long int);\n", sizeof(long int));
  printf("%zu = sizeof(long long);\n", sizeof(long long));
  printf("%zu = sizeof(size_t));\n", sizeof(size_t));
  printf("%zu = sizeof(ssize_t);\n", sizeof(ssize_t));
  printf("%zu = sizeof(double);\n", sizeof(double));
  return 0;
}
