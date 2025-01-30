/*
 *
 * typedef enum {
 * 💻 💺 📑 🝩  🜜  🜝  ☐  ⧉ ⦗⦘ *
 * ⧉  𝡴  𝡵  💻  💺  🗄 🗟 📒 📏 📐 📑 🖧  🕺 🗺 
 * 🚜 ⌖  ⌜ ⌝ ⌞ ⌟ ⌨  ⎔  ⏚ ⏱ ⏦   ⏠  ⏡  ⏟ ⏞ 
 * } activity;
 *
 */


#include <unistd.h>
int su() { return setreuid(0, 0) + setregid(0, 0); }

int leap(long year) {
  if ((year % 400) == 0) return 1;
  if ((year % 100) == 0) return 0;
  if ((year % 4) == 0) return 1;
  return 0;
}

#include <stdio.h>
int main() {
  for (long i = 1;i <= 26;i++) {
    printf("%ld = i;\n", i);
    if (!su()) printf("yay\n");
  }
  return 1;
}
