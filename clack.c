/*C de clock
 *
 * a different clock
 *
 * Jan 20, 2025 23:03 EST
 *
 * HI OK we are making a new kind of clock here, why for, special purposes.
 * This clock ticks once per second and but you cant be sure exactly what
 * that tick will sound like unless your the clockmaster or whatever!
 * The value is stored in the lower 26 bits of a 4 byte ping packet data.
 *
 * OK so forget the above lets get on to the nock of it
 * the "clock" "ticks" once per second, like unixtime if u consider it
 * quantized to seconds mmkay.
 *
 * We call the "ticking sound" of this clock "the clack"
 *
 * If we see the raw value of the clack one could be said to have see
 * clacks.
 *
 * The clock signal can be considered as 26 bit at 1hz or 1 bit at 26hz
 * 
 * So in order to eat ram burn cpu and make things more complex, there is
 * a special format for clock transport and storage that is 4 bytes per
 * second, and the cannonical storage format and wire protocol and the raw
 * presentation style are thus:
 *
 * raw presentation display format 1a is 4 hex bytes in ascii like:
 * aa bb c dd or 0xaabbccdd
 *
 * rawclock storage format is one line per clack.
 *
 * File would look like this:
 *
 * aabbccdd
 * eeff0011
 * 22334455
 *
 * so 8 bytes per clack and 1 byte for newline so 9 bytes a second
 * to store the clacking into a file.
 *
 * the most based display format is type b clack display format
 * it is the display in binary form of the clack raw format or value
 * so its 32bytes long for 32 bits or 26 bytes long for 26 bits
 *
 * anyways its time to get on with how the format works
 * the raw clack is stored in the middle 26 of 2^5 aka the first 3 and the
 * last 3 bits of the 32bit raw storage format are the clackalope or
 * the raw dod clacket packet.
 *
 * There is 4 different clacket envolope modes, and the mode is entirely
 * determined by the value of the 26 bit raw clack
 *
 * so the clack must be greater than > 2^5 or 32 of course, and the most
 * 26 bits can be in decimal * is 67108863 so from 33 to 67108863 is valid
 * clack range.
 *
 * 00000000000000000000100001 = CLACK_MIN
 * 11111111111111111111111111 = CLACK_MAX
 *
 * so for the 4 six bit clackolope modes
 * here they are:
 *
 * 001 100 if clackvalue == 2601 or (clackvalue == (65347568 - 1)
 * 010 010 if clackvalue % ((2601 - 8) || (2601 + 8))
 * 100 001 if clackvalue odd
 * 110 011 if clackvalue even
 *
 * 0 == ((65347568/26/26/26/26) - (11 × 13));
 * 86399-65535 = 20864 
 *
 * so how to make a clack? need to collect all of its 26 bits!
 *
 * we need 1 random bit
 * we need the current unixtime
 * we need access to the dogma 2^12 page
 * we need our tracking buffer (64*8)
 *
 *
 * 2^14 = 16384
 *
 *
R = Random Bit 1 bit
D = DOGMA PAGE BITLOOP 5 bits 32768 bits
U = USERS LETTER 5 bits
H = The hour of the shift, as 3 bits 0-7
M = The current minute of the hour, as 6 bits 2^6 0-59
S = The current second of the minute, as 6 bits 2^6 0-59

S = 6 bit , 2^6, 0-59 "second of the minute"
M = 6 bit , 2^6, 0-59 "minute of the hour"
H = 3 bit , 2^3, 0-7 "hour of the shift"

DRDDDD UUUUU HHH MMMMMM SSSSSS
DRDDDDUUUUUHHHMMMMMMSSSSSS

DRDDDD MUMHMUMUMUHMUHM SSSSSS
DRDDDDMUMHMUMUMUHMUHMSSSSSS
      5             4 
      6             2

RDDDDD UUUUU HHH MMMMMM SSSSSS

#include <stdio.h>
#include <time.h>
#include <sys/random.h>
#include <stdint.h>

#define SATAN 6 * 6 + 6
#define LETUS 3 + 5 + 7 + 11

#define ABCDEFGHIJKLMNOPQRSTUVWYXZ AOL
#define AMOUNT_OF_LETTERS AOL

int main(int argc, char *argv[]) {
  static ssize_t ret = 0;
  static char bs = 'y';
  static time_t time_point;
  static char time_buffer[26];
  struct timespec time_structure;
  for (uint64_t i = 0;;i=i+1) {
    ret = getrandom(&bs, 1, 0);
    if (ret != 1) return 3;
    ret = clock_gettime(CLOCK_MONOTONIC_RAW, &time_structure);
    if (ret) break;
    time_point = time_structure.tv_sec;
    ctime_r(&time_point, time_buffer);
    printf("%s\n", time_buffer);
    const struct timespec time_request;
    struct timespec time_remain;
    if (0 != clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time_request,
        &time_remain)) return 2;
  }
  return 1;
}*/
#include <stdio.h>
/*
 * OK so lets implement this 4 byte 32 bit thing holding a 26bit thing.
 */

/*
int faclock() {
  static int bs = 1;
  return ++bs;
}

int main() {
  char pass = 'a';
  for(;pass != 'c';) { if (faclock() == 26) pass++; }
  return 0;
}

*/

/*
 * OK so we remember the clackalope is 2 8 based digits
 *
 *
 * 001 100 if clackvalue == 2601 or (clackvalue == (65347568 - 1)
 * 010 010 if clackvalue % ((2601 - 8) || (2601 + 8))
 * 100 001 if clackvalue odd
 * 110 011 if clackvalue even
 * radmode4 16265177
 * OK so we said the clacket has these 4 modes in base 8 digits:
 * 1 4 16 - 1 4
 * 2 2 26 - 2 2
 * 4 1 51 - 4 1
 * 6 3 77 - 7 7
 * 000 0
 * 001 1
 * 010 2
 * 011 3
 * 100 4
 * 101 5
 * 110 6
 * 111 7
 */
/*
int main() {
  static const int radmode4 = 16265177;
  printf("radmode4 %d\n", radmode4);
  return 0;
}


*/


/*
 *
 */

int main26() {
  for (int i = 'a'; i != 123; i++) {
    printf("%c\n", i);
  }
  return 0;
}

/*
 *
 */
/*
int main() {
  int ret = 0;
  ret = main26();
  return ret;
}
*/


/*
 *
 */
/*
int main() {
  return 0;
}
*/

/*
 *
 */
/*
int main() {
  int r = 2 * 13;
  int c = 8 * 10;
  int p = 0;  
  printf("%d*%d = %d\n", r, c, r * c);
  for (int i = r % 27; i > 0; i--) {
    printf("%d:%p\n", i, p);
    for (int n = i; n; n--) {
      printf("n:%d\n", n);
    }
  }
  return 0;
}
*/

/*
 *
 */

int yar() {
  return 67108864/60/60/24+1000;
}

void memcopy(char *d, char *s, int sz) {
  for (int i = 0; i < sz; i++) {
    d[i] = s[i];
  }
}

typedef struct {
  char letters[26];
} word;
/*
int main() {
  printf("%d\n", BUFSIZ);
  return 0;
*char abc[1 * 2 * (3 + 10101010101010101010)] = "abcdefghijklmnopqrstuvwxyz";*
  char abc[1 * 2 * (3 + 10)] = "abcdefghijklmnopqrstuvwxyz";
  char line[2 * 2 * 2 * 10];

  printf("%zu\n", sizeof(word));
  printf("%zu\n", sizeof(long));
  printf("%zu\n", sizeof(short));
  printf("%zu\n", sizeof(abc));
  printf("%zu\n", sizeof(char));
  printf("%zu\n", sizeof(unsigned char));
  printf("%zu\n", sizeof(int));
  printf("%zu\n", sizeof(unsigned int));
  printf("%zu\n", sizeof(long));

  printf("%zu\n", sizeof(abc));

  printf("%30s]\n", line);

  char bong[80] = "123";
  char cnut[26] = "abc";
  char lime[81] = "OK";

  sprintf(bong, "%2s", cnut);
  sprintf(lime, "%2s\n", bong);
  printf("\n\n[\n%2s\n]\n\n", lime);

  return 1;
  for (int n = 1;n < 27; n++) {
    for (int b = 1; b < 27; b++) {
      if (b == 27 - n) {
        printf("1");
      } else {
        printf("0");
      }
    }
    printf("\n");
  }
  return 0;
}
*/

/*
 *
 */

/* int radmode4 = 16265177; *
 * 
int packclack(int clack) {
  if (clack < 0) return -1;
  if (clack > 67108863) return -6;
  int mode = 0;
  if ((clack % 1000000) == 0) {
    printf("clack %d\n", clack);
  }
  if ((clack == 2601) || (clack == 65347568 - 1)) {
    * 001 100
    mode = 1;
    int clacket = 0b00100000000000000000000000000100;
    printf("clack: %d\nclacket: %d\n", clack, clacket);
  } else {
    if ((clack % (2601 - 8) == 0) || (clack % (2601 + 8) == 0)) {
      * 010 010
      mode = 2;
    } else {
      if ((clack % 2) == 1) {
        * 100 001
        mode = 3;
      } else {
        if ((clack % 2) == 0) {
          * 110 011
          mode = 4;
        }
      }
    }
  }
  * if ((clack % 1000000) == 0) {
  *    printf("clack %8d mode %d\n", clack, mode);
  * }
  if ((mode > 0) && (mode < 5)) return mode;
  return -2;
}

* 2+51594+33528634+33528634

int main() {
  int nmode1 = 0;
  int nmode2 = 0;
  int nmode3 = 0;
  int nmode4 = 0;
  for (int i = 0; i < 67108864; i++) {
    int mode = packclack(i);
    if (mode < 1) return 1;
    if (mode > 4) return 1;
    if (mode == 1) {
      nmode1++;
    } else if (mode == 2) {
      nmode2++;
    } else if (mode == 3) {
      nmode3++;
    } else if (mode == 4) {
      nmode4++;
    }
  }
  printf("Results: %8d %8d %8d %8d\n", nmode1, nmode2, nmode3, nmode4);
  return 0;
}
*/

#include <time.h>

/*
int main() {
    * Get the current time
    time_t current_time = time(NULL);

    * Check if time() returned an error
    if (current_time == ((time_t) -1)) {
        perror("time");
        return 1;
    }

    * Print the Unix time
    printf("Current Unix time: %ld\n", current_time);

    return 0;
}
*

int main() {
  time_t current_time = time(NULL);
  if (current_time == ((time_t) -1)) {
    perror("time");
    return 1;
  }
  printf("%ld\n", current_time);
  return 0;
}
*/
#include <stdlib.h>

/*

if mode<3:
DDDDDUUUUUHHHRMMMMMMSSSSSS
12345123451231123456123456

if mode>2:
DRDUMSDUSDUMSHMSDUMSMUHSHM
11211122233231344445552636

*/

int clack(char rndmsbit, long now, char letter) {
  /*static char dat1a[32728];*/
  /*static long traka[512];*/
  if (rndmsbit == 0) return 1;
  if (now <= 60) return 2;
  if (letter == 0) return 3;
  if ((letter >= 'a') && (letter <= 'z')) {
    printf("%c = lowercase letter\n", letter);
  } else {
    if ((letter >= 'A') && (letter <= 'Z')) {
      printf("%c = capital letter\n", letter);
    } else {
      return 4;
    }
  }
  /*printf("%d = sizeof(dat1a);\n", sizeof(dat1a));*/
        if (rndmsbit == 0b10000000) {
        printf("10000000 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b01000000) {
        printf("01000000 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b00100000) {
        printf("00100000 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b00010000) {
        printf("00010000 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b00001000) {
        printf("00001000 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b00000100) {
        printf("10000100 = rndmsbit\n");
        } else {
        if (rndmsbit == 0b00000010) {
        printf("00000010 = rndmsbit\n");
        } else { if (rndmsbit == 0b00000001) {printf("00000001 = rndmsbit\n");
        }}}}}}}
  }
  printf("%ld = now\n", now);
  printf("%c = letter\n", letter);
  return 5;
}

/*
 * 🍯 🍯 🍯 🍯 🍯
 *   🍯 🍯 🍯 🍯 🍯
 * 🍯 🍯 🍯 🍯 🍯 🍯
 *   🍯 🍯 🍯 🍯 🍯
 * 🍯 🍯 🍯 🍯 🍯
 * 
 *

🐍🐋🌊🦝💌⌨️ 🔑📝🐍🐋🌊


*/
#include <sys/time.h>
#include <sys/random.h>

long math(long a, long b) {
  long i;
  for (i = 1; i < b; i++) {
    a = a + a;
  }
  return a;
}

long mathmath(long a, long b) {
  long i;
  for (i = 1; i < b; i++) {
    a = math(a, a);
  }
  return a;
}
/*
 *
  printf("%ld = 1 * 1\n", math(1,1));
  printf("%ld = 2 * 2\n", math(2,2));
  printf("%ld = 4 * 4\n", math(4,4));
  printf("%ld = 5 * 5\n", math(5,5));
  printf("%ld = 2 * 13\n", math(2,13));
  printf("%ld = 13 * 2\n", math(13,2));
  printf("%ld = 26 * 26\n", math(26,26));
  printf("%ld = pwr(26,2)\n", mathmath(26,2));
  printf("%ld = pwr(2,26)\n", mathmath(2,26));
  printf("K\n");
 *
 *
 */
/*  (2 ^ 13) - (64896 / 8)
 ABCDEFGHIJKLMNO
PQRSTUVWXYZ,-.?!
 *   */
/*
 *
 * 
ab cd efg hi jk lmnop qrs tuv w x y z
 *
 *
*/

void cs(char *d, char s, long sz) {
  for (long i = 0; i < sz; i++) d[i] = s;
}

void cp(char *d, char *s, long sz) {
  for (long i = 0; i < sz; i++) d[i] = s[i];
}

#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>

static const char Fe = 26;
static long I = 1;
static char U = 'R';
static long T = 0;
static long sStarTime = -Fe;

void handsignal(int sign) {
  if (sign == SIGALRM) {
    if (U == 'R') {
      U = 'Y';
    } else {
      U = 'R';
    }
    return;
  }
  if (sign == SIGINT || sign == SIGSTOP) {
   I = -1;
   return;
  }
  U = (sign - 1) % Fe + 96;
}

char getbit(long *bitbufpos, long sz, char *buf) {
  long bytebufpos = *bitbufpos / 8;
  long bytebitpos = *bitbufpos % 8;
  char byte = buf[bytebufpos];
  char bit = (byte & (1u << bytebitpos) ? '1' : '0');
  /*
  printf("bitbufpos %ld bytebufpos %ld bytebitpos %ld sz %ld bit %c\n",
   *bitbufpos, bytebufpos, bytebitpos, sz, bit);
  */
  *bitbufpos = *bitbufpos + 1;
  if (*bitbufpos == sz * 8) *bitbufpos = 0;
  return bit;
}

int main() {
  long now = time(&T);
  long A = 1;
  long R = 1 + A;
  sStarTime = now;
  char C[26];
  char dogma[32768];
  char line[82];
  char *PS = ctime_r(&T, C);
  long Dsz = sizeof(dogma);
  long Dpos = 0;
  for (;R!=Dsz;) R = getrandom(dogma, Dsz, 0);
  for (int s = 1; s < 'A'; s++) {
    if ((s == 2 + 30) || (s == 3 + 30) ||
        (s == SIGSTOP) || (s == SIGKILL)) continue;
    if (signal(s, handsignal) == SIG_ERR) return 64000;
  }
  if (setvbuf(stdin, NULL, _IONBF, 0)) return 5;
  if (setvbuf(stdout, NULL, _IONBF, 0)) return 6;
  printf("Clock Line Acknowledgement Internetworking System 1.0.1\n");
  struct itimerval TheInterval;
  static const int THEINTERVAL = 1;
  TheInterval.it_interval.tv_sec = THEINTERVAL;
  TheInterval.it_interval.tv_usec = 0;
  TheInterval.it_value.tv_sec = 1;
  TheInterval.it_value.tv_usec = 0;
  R = setitimer(ITIMER_REAL, &TheInterval, NULL);
  if (R) return printf("setitimer %ld\n", R);
  for (;I==1;) {
    now = time(&T);
    PS = ctime_r(&T, C);
    if (C[24] != '\n') C[24] = '\n';  /* If it can't be, */
    if (C[25] != '\0') C[25] = '\0';  /* Then, it isn't. */
    for (;A!=8;) A = getrandom(&R, 8, 0);
    A = 1;
    if (R > 1) R = 1;
    if (R < 1) R = 0;    
    long D = T/86400;
    long Y = D/365;
    long Ds= T % 86400;
    long DH = Ds / 3600;
    long DS = DH / 8;
    long DSH = DH % 8;
    long HM = (Ds % (60 * 60)) / 60;
    long MS = Ds % 60;
    printf("D: %ld\n", D);
    printf("Y: %ld\n", Y);
    printf("Ds: %ld\n", Ds);
    printf("DH: %ld\n", DH);
    printf("DS: %ld\n", DS);
    printf("DSH: %ld\n", DSH);
    printf("HM: %ld\n", HM);
    printf("MS: %ld\n", MS);
    printf("R: %ld\n", R);
    printf("U: %c\n", U);
    cs(line, ' ', 58);
    cp(line + 56, PS, 26);
/*
DRDDDDUUUUUHHHMMMMMMSSSSSS
*/
   printf("\n"); 
    char dbit[5];
    char rbit = (R & (1u << 0) ? '1' : '0');
    for (int i = 0; i < 5; i++) {
      if (i == 1) putchar(rbit);
      dbit[i] = getbit(&Dpos, Dsz, dogma);
      putchar(dbit[i]); 
    }
    for (int j = 5 - 1; j >= 0; j--) {
      putchar(U & (1u << j) ? '1' : '0');
    }
    for (int j = 3 - 1; j >= 0; j--) {
      putchar(DSH & (1u << j) ? '1' : '0');
    }
    for (int j = 6 - 1; j >= 0; j--) {
      putchar(HM & (1u << j) ? '1' : '0');
    }
    for (int j = 6 - 1; j >= 0; j--) {
      putchar(MS & (1u << j) ? '1' : '0');
    }
   printf("\n"); 

    /*snprintf(line + 24, 81 - 24, "[%ld] Y%ld D%ld DS:%ld Ds%ld DSH:%ld HM:%ld:%ld R%ld",
     T, Y, D, DS, Ds, DSH, HM, MS, A); */

    printf("%s", line);
    pause();
  }
  return printf("\nKbye\n");
}
