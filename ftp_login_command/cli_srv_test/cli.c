#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *convert_addr_to_str(unsigned long ip_addr, unsigned int port);

void main(int argc, char **argv)
{
	/* insert ls, send PORT cmd & control connection, rcv control conn */
	/* send NLST, rcv contrl conn, result data by data conn */
	char *hostport;
	struct sockaddr_in temp;
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&temp, 0, sizeof(temp));
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = inet_addr(argv[1]);
	temp.sin_port = htons(atoi(argv[2]));
	connect(sockfd, (struct sockaddr*)&temp, sizeof(temp));

	char buff[128];
	write(STDOUT_FILENO, "> ", 2);
	int n = read(STDIN_FILENO, buff, sizeof(buff));
	buff[n] = '\0';

	hostport = convert_addr_to_str(temp.sin_addr.s_addr, temp.sin_port);
	printf("converting to %s\n", hostport);
	if(!strcmp(buff, "ls"))
		write(sockfd, hostport, strlen(hostport)); // send PORT
	n = read(sockfd, buff, sizeof(buff)); // ACK from control server
	printf("%s\n", buff);
	write(sockfd, "NLST", 4); // send NLST by control
	
	struct sockaddr_in data_server, data_client;
	char result_buff[128];
	int d_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&data_server, 0, sizeof(data_server));
	data_server.sin_family = AF_INET;
	data_server.sin_addr.s_addr = htonl(INADDR_ANY);
	data_server.sin_port = htons(20000);
	bind(d_sockfd, (struct sockaddr*)&data_server, sizeof(data_server));
	listen(d_sockfd, 5); //
	
	int len = sizeof(data_client); // accept data conn
	int d_clientfd = accept(d_sockfd, (struct sockaddr*)&data_client, &len);
	n = read(d_clientfd, result_buff, sizeof(result_buff)); // ls result
	result_buff[n] = '\0';
	printf("%s\n", result_buff); // ls result

	close(sockfd);
	close(d_clientfd);
	close(d_sockfd);
	exit(1);
	/* control connectino and data connection */
}

char *convert_addr_to_str(unsigned long ip_addr, unsigned int port) {
	char *addr, *c_height, *c_leight;
	char *n_ip, *o_ip;
	struct in_addr sin_ip;
	sin_ip.s_addr = ip_addr;
	char *dd_ip = inet_ntoa(sin_ip);
	int portnum = 20000;
	
	int bin[16];
	for(int i=15; i>=0; i--) {
		bin[i] = portnum % 2;
		portnum /= 2;
	}
	int pos = 0;
	int height = 0; // high 8bit
	for(int i=7; i>=0; i--) {
		if(bin[i] == 1)
			height += 1 << pos;
		pos++;
	}
	sprintf(c_height, "%d", height); // char type
	pos = 0;
	int leight = 0; // low 8bit
	for(int i=15; i>=8; i--) {
		if(bin[i] == 1)
			leight += 1 << pos;
		pos++;
	}
	sprintf(c_leight, "%d", leight); // char type
	
	strcpy(addr, "PORT ");
	n_ip = strtok_r(dd_ip, ".", &o_ip);
	while(n_ip) { // PORT 127,0,0,1,
		strcat(addr, n_ip);
		strcat(addr, ",");
		n_ip = strtok_r(NULL, ".", &o_ip);
	}
	strcat(addr, c_height);
	strcat(addr, ",");
	strcat(addr, c_leight);

	return addr;
}
