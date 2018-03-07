#include<iostream>
//#include<string>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>

#ifdef __linux__
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netdb.h>

#elif __WIN32__
#include<Winsock2.h>

#endif

//openssl header
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<openssl/rand.h>
#include<openssl/bio.h>
//#include<openssl/applink.c>


#define TCP_BUFFER 10000
using namespace std;

void CheckArguments(int argc, char **argv);
void run(const char* host, const char* getport);
void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
struct sockaddr_in initAddr(const char* hostname, int port);
int connectServer(const char *hostname, int port);
