// Daniel Darwin
// dd1223
// PA 1
// I referenced this https://www.geeksforgeeks.org/socket-programming-cc/ to get tips on tcp sockets


#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include <string.h>
#include <unistd.h>
#include <typeinfo>
#include <vector>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]){

  /////////////////////////TCP ACK//////////////////////////////////
  struct hostent *s;
  s = gethostbyname(argv[1]);

  string filename = argv[2];

  struct sockaddr_in server;
  int mysocket = 0, newsocket = 0;
  socklen_t slen = sizeof(server);
  char payload[512]="2020";
  int random_port, port;
  port = atoi(argv[2]);

  // Create tcp socket
  if ((mysocket=socket(AF_INET, SOCK_STREAM, 0))==-1)
    cout << "Error in creating socket.\n";

  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  bcopy((char *)s->h_addr,
	(char *)&server.sin_addr.s_addr,
	s->h_length);

  // Connect to server
  if (connect(mysocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        cout << "Connection Failed" << endl;
    }
  // Send ack
  if (send(mysocket, payload, 8, 0)==-1)
      cout << "Error in sendto function.\n";
  // Receive port back
  recv(mysocket, &random_port, 512, 0);
  close(mysocket);
  close(newsocket);

  ////////////////////////Start UDP ////////////////////////
  //cout << "UDP " << endl;
  int udp_socket = 0;
  char end_data[]="ENDF";
  int endd = 0;
  struct sockaddr_in new_server;
  socklen_t new_slen = sizeof(new_server);

  // Create udp data
  if ((udp_socket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    cout << "Error in creating socket.\n";

  memset((char *) &new_server, 0, sizeof(new_server));
  new_server.sin_family = AF_INET;
  new_server.sin_port = htons(random_port);
  bcopy((char *)s->h_addr,
	(
  char *)&new_server.sin_addr.s_addr,
	s->h_length);

  // Open file
  ifstream file;
  file.open(argv[3]);
  char c = 'C';
  char data[512]="";
  char udp_data[512];
  while (!file.eof()) {
    //cout << c <<endl;
    for (int j = 0; j < 4; j++) {
        file.get(c); //Get each character and add it to an array
        data[j]= c;
        //cout << c;
    }
    //  Send data to server
    if(sendto(udp_socket,data, 8, 0, (struct sockaddr *)&new_server, new_slen)==-1);
    // Received the capitalized data back
    if (recvfrom(udp_socket, udp_data, 8, 0, (struct sockaddr *)&new_server, &new_slen)==-1)
        cout << "Failed to receive.\n";
    cout << udp_data;
    cout << endl;
  }
  // Send end ack bit to show end of file
  if (sendto(udp_socket, end_data, 8, 0, (struct sockaddr *)&new_server, new_slen)==-1)
  file.close();


  close(udp_socket);
  return 0;
}
