// Daniel Darwin
// dd1223
// PA 1

#include<iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <time.h>        // used for random number generation
#include <string.h> // using this to convert random port integer to string
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cctype>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

  struct sockaddr_in server;
  struct sockaddr_in client;
  int mysocket = 0, newsocket = 0, i = 0,n_port = 0, port = 0;
  socklen_t clen = sizeof(client);
  char payload[512];
  n_port = atoi(argv[1]);

  if ((mysocket=socket(AF_INET, SOCK_STREAM, 0))==-1)
    cout << "Error in socket creation.\n";

  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(n_port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind
  if (bind(mysocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    cout << "Error in binding.\n";

  //Listen for client on specified port
  if (listen(mysocket, 3) < 0)
    cout << "Error in listening";

  // Accept connection from  client
  if((newsocket = accept(mysocket, (struct sockaddr *)&client, &clen)) == -1)
    cout << "Error in accept";

  // receive data from ack from client
  recv(newsocket, payload, 512, 0);
  //cout << "Received data: " << payload << endl;

  //Find random port and send it to client
  srand(time(NULL));
  port = rand() % 64511 + 1024;
  send(newsocket, &port, 8, 0);
  cout << "Handshake detected. Selected random port " << port << endl;
  // close tcp socket
  close(mysocket);
  close(newsocket);
  ///////////////////////////////////UDP?////////////////////////////////////////////
  //cout << "UDP " << endl;

  int udp_socket = 0;
  ofstream dataReceived;
  int end_file = 0;
  char udp_data[512];

  // Create socket
  if ((udp_socket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    cout << "Error in socket creation.\n";

  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind
  if (bind(udp_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    cout << "Error in binding.\n";

  dataReceived.open("dataReceived.txt");
  // Receive data until end of file
  while (end_file == 0) {

      // Receive data
      if (recvfrom(udp_socket, udp_data, 8, 0, (struct sockaddr *)&client, &clen)==-1)
        cout << "Failed to receive.\n";

      // Check for end of file
      if(strcmp(udp_data, "ENDF") == 0)
        break;

      // Convert data to upper case
      for (int i = 0; i < 4; i++)
        udp_data[i] = toupper(udp_data[i]);

      // Send data back
      if (sendto(udp_socket, udp_data, 8, 0, (struct sockaddr *)&client, clen)==-1)
            cout << "Error in sendto function.\n";
      dataReceived << udp_data << endl;

  }
  char ack[]="Got all that data, thanks!";
  if (sendto(udp_socket, ack, 64, 0, (struct sockaddr *)&client, clen)==-1){
    cout << "Error in sendto function.\n";
  }
  dataReceived.close();
  close(udp_socket);


  return 0;
}
