#include "header.hpp"


void CheckArguments(int argc, char **argv){
	if (!(argc <= 3 && argc >= 2))	{
		printf("syntax : run <hostname> <port>\n");
		abort();
	}
}

void run(const char* host, const char* getport){

    int sock;
	BIO *certbio = NULL;
    BIO *outbio = NULL;
    X509 *cert = NULL;
    X509_NAME *certname = NULL;
    SSL_CTX *ctx;
	SSL *ssl;
 	int server, readSize, ret, port = atoi(getport), ssl_write_ret;
	char buf[1024];

    init_openssl();

	certbio = BIO_new(BIO_s_file());
    outbio = BIO_new_fp(stdout, BIO_NOCLOSE);

	ctx = create_context();
	// configure_context(ctx);
	//SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
	ssl = SSL_new(ctx);

	server = connectServer(host, port);

	SSL_set_fd(ssl, server);

	if( (ret = SSL_connect(ssl)) < 0)
		cout<<"Failed to Handshake\t Error code : " << SSL_get_error(ssl, ret) << endl;

	else if (ret == 1)
	{
		cout << "ret = 1 셰이크 석세스" << endl;
		cert = SSL_get_peer_certificate(ssl);
		if (cert == NULL)
			BIO_printf(outbio, "Error: Could not get a certificate from: %s.\n", host);
		else
			BIO_printf(outbio, "Retrieved the server's certificate from: %s.\n", host);
		certname = X509_NAME_new();
		certname = X509_get_subject_name(cert);
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		//SSL_set_connect_state(ssl);
		if ((ssl_write_ret = SSL_write(ssl, buf, strlen(buf))) <= 0)
		{
			printf("\nSSL_write Error\t Error Code : %d\n", SSL_get_error(ssl, ret));
			abort();
		}
		readSize = SSL_read(ssl, buf, sizeof(buf));
		buf[readSize] = 0;
		printf("Receive:\n %s \n", buf);
		SSL_free(ssl);
	}
	else 
		cout<<"The TLS/SSL handshake was not successful but was shut down controlled and by the specifications of the TLS/SSL protocol.\t Error code : " << SSL_get_error(ssl, ret) << endl;

	close(server);
	X509_free(cert);
    SSL_CTX_free(ctx);
    cleanup_openssl();
	BIO_printf(outbio, "Finished SSL/TLS connection with server: %s.\n", host);
}

void init_openssl(){
    OPENSSL_init();
    #if OPENSSL_VERSION_NUMBER <0x10100000L
		SSL_library_init ();
	#else
		OPENSSL_init_ssl (0, NULL);
	#endif
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

void cleanup_openssl(){
    EVP_cleanup();
}

SSL_CTX *create_context(){
    const SSL_METHOD *method;
	SSL_CTX *ctx;

    method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);

    if (!ctx){
        perror("Unable to create SSL Context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void configure_context(SSL_CTX *ctx){

    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "crt.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in initAddr(const char* hostname, int port){

	struct sockaddr_in newAddr;
	memset(&newAddr, 0x00, sizeof(newAddr));									        // 구조체를 0으로 초기화 시킨다.
	newAddr.sin_family = PF_INET;
	newAddr.sin_port = htons(port);
    newAddr.sin_addr.s_addr = inet_addr( hostname );

	return newAddr;
}

int connectServer(const char *hostname, int port){
	int sd;
	struct hostent *host;
	struct sockaddr_in server_addr;
	sd = socket(PF_INET, SOCK_STREAM, 0);												// return value 실패 : -1 성공 : sd > 0
	server_addr = initAddr(hostname, port);

	if(connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0 ){			// return value 실패 : -1 성공 : 0
		cout<<"Failed to Connection"<<endl;
		close(sd);
		abort();															            //프로그램이 즉시 종료된다.
	}
    else
    	cout << "connected" << endl;
	return sd;
}

int main(int argc, char **argv){
	CheckArguments(argc, argv);
	run(argv[1], argv[2]);
}