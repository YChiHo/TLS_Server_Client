#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int create_socket(int port)
{
	int s;
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));

	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		perror("Unable to create socket");
		exit(EXIT_FAILURE);
	}

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Unable to bind");
		exit(EXIT_FAILURE);
	}

	if (listen(s, 1) < 0)
	{
		perror("Unable to listen");
		exit(EXIT_FAILURE);
	}

	return s;
}

void init_openssl()
{
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl()
{
	EVP_cleanup();
}

SSL_CTX *create_context()
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	method = SSLv23_server_method();

	ctx = SSL_CTX_new(method);
	if (!ctx)
	{
		perror("Unable to create SSL context");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	return ctx;
}

void configure_context(SSL_CTX *ctx)
{
	SSL_CTX_set_ecdh_auto(ctx, 1);

	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(ctx, "crt.pem", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	int sock;
	SSL_CTX *ctx;

	init_openssl();
	ctx = create_context();

	configure_context(ctx);

	sock = create_socket(atoi(argv[1]));
	if (sock > 0)	printf("Create Socket ! \n");

	/* Handle connections */
	while (1)
	{
		struct sockaddr_in addr;
		uint len = sizeof(addr);
		SSL *ssl;
		const char reply[] = "HTTP/1.1 200 OK\n";

		int client ;
		if ((client= accept(sock, (struct sockaddr *)&addr, &len)) < 0)
		{
			perror("Unable to accept");
			exit(EXIT_FAILURE);
		}
		else
			printf("Accepted with client\n");

		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, client);

		if (SSL_accept(ssl) <= 0)
		{
			ERR_print_errors_fp(stderr);
		}
		else
		{
			SSL_write(ssl, reply, strlen(reply));
		}

		SSL_free(ssl);
		close(client);
	}

	close(sock);
	SSL_CTX_free(ctx);
	cleanup_openssl();
}

// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <unistd.h>
// #include <vector>
// #include <map>
// #include <algorithm>
// #include <chrono>
// #include <array>

// // using std::cout;
// // using std::endl;
// // using std::map;
// // using std::mutex;
// // using std::thread;
// // using std::vector;
// // using std::pair;
// using namespace std;
// using namespace chrono;

// class thrd
// {
//   public:
// 	int i;
// 	mutex mtx;

//   public:
// 	thrd()
// 	{
// 		i = 0;
// 		i;
// 	}
// 	void tt1_function()
// 	{
// 		while (1)
// 		{
// 			cout << "This is " << this << "f1 i : " << i++ << " " << ++i << i << " " << endl;
// 		}
// 	}
// 	void tt2_function()
// 	{
// 		while (1)
// 		{
// 			cout << "This is " << this << "f2 i : " << i << " " << i-- << " " << --i << endl;
// 		}
// 	}
// 	thread Thread_Proc1(int n)
// 	{
// 		return thread([this] { tt1_function(); });
// 	}

// 	thread Thread_Proc2(int n)
// 	{
// 		return thread([this] { tt2_function(); });
// 	}
// };

// class loopi
// {
//   public:
// 	std::vector<int> vec;

//   public:
// 	loopi()
// 	{
// 		vec = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
// 	}
// 	void vec_test()
// 	{
// 		for (auto &iter : vec)
// 		{
// 			cout << "Hello world : " << iter << endl;
// 		}
// 	}
// };

// class mapper
// {
//   public:
// 	map<char, int> one;
// 	public:
// 	mapper(){
// 		one['A'] = 65;		one['B'] = 66;		one['C'] = 67;		one['D'] = 68;		one['E'] = 69;		one['F'] = 70;		one['G'] = 71;		one['H'] = 72;		one['I'] = 73;
// 		one['J'] = 74;		one['K'] = 75;		one['L'] = 76;		one['M'] = 77;		one['N'] = 78;		one['O'] = 79;		one['P'] = 80;		one['Q'] = 81;		one['R'] = 82;
// 		one['S'] = 83;		one['T'] = 84;		one['U'] = 85;		one['V'] = 86;		one['W'] = 87;		one['X'] = 88;		one['Y'] = 89;		one['Z'] = 90;
// 		one.insert(pair<char, int>('a', 97));		one.insert(pair<char, int>('b', 98));		one.insert(pair<char, int>('c', 99));		one.insert(pair<char, int>('d', 100));
// 		one.insert(pair<char, int>('e', 101));		one.insert(pair<char, int>('f', 102));		one.insert(pair<char, int>('g', 103));		one.insert(pair<char, int>('h', 104));
// 		one.insert(pair<char, int>('i', 105));		one.insert(pair<char, int>('j', 106));		one.insert(pair<char, int>('k', 107));		one.insert(pair<char, int>('l', 108));
// 		one.insert(pair<char, int>('m', 109));		one.insert(pair<char, int>('n', 110));		one.insert(pair<char, int>('o', 111));		one.insert(pair<char, int>('p', 112));
// 		one.insert(pair<char, int>('q', 113));		one.insert(pair<char, int>('r', 114));		one.insert(pair<char, int>('s', 115));		one.insert(pair<char, int>('t', 116));
// 		one.insert(pair<char, int>('u', 117));		one.insert(pair<char, int>('v', 118));		one.insert(pair<char, int>('w', 119));		one.insert(pair<char, int>('x', 120));
// 		one.insert(pair<char, int>('y', 121));		one.insert(pair<char, int>('z', 122));
// 	}

// };

// // struct EvenCountFunctor : public unary_function<int, void>
// // {
// // 	EvenCountFunctor(int &count) : evenCount(count) {};

// // 	void operator()(int number)
// // 	{
// // 		if (number % 2 == 0)
// // 		{
// // 			evenCount++;
// // 		}
// // 	}

// //   private:
// // 	int &evenCount;
// // };

// int
// main(int argc, char **argv)
// {
// 	thrd *t = new thrd();
// 	// thread tt1 = t->Thread_Proc1(1);
// 	// thread tt2 = t->Thread_Proc2(1);
// 	// tt1.join();
// 	// tt2.join();
// 	delete (t);

// 	loopi *l = new loopi();
// 	l->vec_test();
// 	delete (l);

// 	mapper *mp = new mapper();
// 	// for(auto iter = mp->one.begin(); iter != mp->one.end(); ++iter){
// 	// 	cout << (*iter).first << " = " << (*iter).second << endl;
// 	// }
// 	auto iter = mp->one.find('c');
// 	if (iter != mp->one.end())
// 		cout << "c 짝꿍 : " << iter->second << endl;
// 	delete (mp);

// 	//lambda functions. test
// 	auto ai = [](int a, int b) -> int { return a + b; };

// 	int aiR = ai(1, 2);
// 	cout << aiR << endl;

// 	// std::array<array<int, 3>, 3> arr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
// 	std::array<int, 3> arr1 = {1, 2, 3};
// 	cout << "array size : " << sizeof(arr1) << endl;
// 	for ( auto &iter : arr1 ) cout << iter << ' ';
// 	for (int i = 0 ; i < arr1.size() ; i++) cout << arr1.at(i) << ' ';
// 	cout << endl;

// 	return 0;
// }