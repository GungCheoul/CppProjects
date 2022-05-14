#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

char *convert_str_to_addr(char *str, unsigned int *port);

void main(int argc, char **argv)
{
	char *host_ip;
	char temp[25];
	unsigned int port_num;
	struct sockaddr_in server_addr, client_addr;
	char *buff;//
	char *o_temp;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(sockfd, 5);
	
	int n = read(sockfd, temp, sizeof(temp));
	temp[n] = '\0';

	host_ip = convert_str_to_addr(temp, (unsigned int*)&port_num);
	host_ip = strtok_r(host_ip, " ", &o_temp); // 127.0.0.1 IP
	buff = strtok_r(NULL, " ", &o_temp); // port#

	struct sockaddr_in data_server;
	char result_buff[128];
	DIR *dp = opendir("./");
	struct dirent *dir = NULL;

	int d_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&data_server, 0, sizeof(data_server));
	data_server.sin_family = AF_INET;
	data_server.sin_addr.s_addr = inet_addr(host_ip);
	data_server.sin_port = htons(atoi(buff));
	connect(d_sockfd, (struct sockaddr*)&data_server, sizeof(data_server));
	
}

char *convert_str_to_addr(char *str, unsigned int *port) {
	char *addr, *temp, *n_temp, *o_temp;
	char *p_num;
	
	temp = strtok_r(str, " ", &o_temp); // PORT
	n_temp = strtok_r(NULL, ",", &o_temp);
	strcat(addr, n_temp);
	for(int i=0; i<3; i++) {
		n_temp = strtok_r(NULL, ",", &o_temp);
		strcat(addr, ".");
		strcat(addr, n_temp);
	}
	strcat(addr, " ");
	
	temp = strtok_r(NULL, ",", &o_temp);
	int d_temp = atoi(temp);
	int bin[16];
	for(int i=15; i>=8; i--) {
		bin[i] = d_temp % 2;
		d_temp /= 2;
	}
	temp = strtok_r(NULL, ",", &o_temp);
	d_temp = atoi(temp);
	for(int i=7; i>=0; i--) {
		bin[i] = d_temp % 2;
		d_temp /= 2;
	}
	int pos = 0;
	int dec = 0;
	for(int i=15; i>=0; i--) {
		if(bin[i] == 1)
			dec += 1 << pos;
		pos++;
	}
	sprintf(p_num, "%d", dec);
	strcat(addr, p_num); // 127.0.0.1 (port)
	return addr;
}

