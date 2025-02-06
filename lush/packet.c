#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>

int main() {
  int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  struct sockaddr server_addr;
  int bind_ret = bind(packet_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
  printf("mkay we got %d and %d\n", packet_socket, bind_ret);
  return 0;
}
