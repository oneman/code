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

int main() {
  int ret = 0;
  ret = main26();
  return ret;
}
