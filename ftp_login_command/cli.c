#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *convert_port(unsigned long ip_addr, unsigned int port);
int log_in(int ctrl_fd);

int main(int argc, char **argv)
{
	char *ctrlport;
	struct sockaddr_in temp, server_addr;
	char buff[128], reply[128], cmd[128];
	int n, ctrl_fd;
	int dsize = 0;
	
	ctrl_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&temp, 0, sizeof(temp));
	memset(&server_addr, 0, sizeof(server_addr));
	temp.sin_family = AF_INET;
	temp.sin_addr.s_addr = inet_addr(argv[1]);
	temp.sin_port = htons(40000);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	connect(ctrl_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	ctrlport = convert_port(temp.sin_addr.s_addr, temp.sin_port); // PORT

	n = read(ctrl_fd, buff, sizeof(buff)); // ACK
	buff[n] = '\0';
	if(!strncmp(buff, "220", 3)) {
		write(STDOUT_FILENO, buff, strlen(buff));
		if(log_in(ctrl_fd) < 0) {
			close(ctrl_fd);
			return 0;
		}
	}
	else if(!strncmp(buff, "431", 3)) {
		write(STDOUT_FILENO, buff, strlen(buff));
		close(ctrl_fd);
		return 0;
	}

	while(1) {
		char *n_temp = NULL;
		char *o_temp = NULL;
		char *arg = NULL;
		char result_buff[128];
		int len, data_fd, d_clientfd;
		write(STDOUT_FILENO, "ftp> ", 5);
		n = read(STDIN_FILENO, buff, sizeof(buff));
		buff[n-1] = '\0';
		n_temp = strtok_r(buff, " ", &o_temp);
		arg = strtok_r(NULL, " ", &o_temp);
		if(!strcmp(buff, "ls") || !strcmp(buff, "get") || !strcmp(buff, "put")) {
			write(ctrl_fd, ctrlport, strlen(ctrlport));
			
			struct sockaddr_in data_server, data_client;
			data_fd = socket(AF_INET, SOCK_STREAM, 0);
	
			memset(&data_server, 0, sizeof(data_server));
			data_server.sin_family = AF_INET;
			data_server.sin_addr.s_addr = htonl(INADDR_ANY);
			data_server.sin_port = htons(40000);
			bind(data_fd, (struct sockaddr*)&data_server, sizeof(data_server));
			listen(data_fd, 5);
	
			len = sizeof(data_client);
			d_clientfd = accept(data_fd, (struct sockaddr*)&data_client, &len);
			n = read(ctrl_fd, buff, sizeof(buff));
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff)); // 200
		}

		if(!strcmp(buff, "ls")) {
			strcpy(cmd, "NLST");
			if(arg) {
				strcat(cmd, " ");
				strcat(cmd, arg);
			}
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, buff, sizeof(buff)); // 150
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			n = read(d_clientfd, result_buff, sizeof(result_buff));
			result_buff[n] = '\0';
			dsize += n;//
			write(STDOUT_FILENO, result_buff, strlen(result_buff));
			n = read(ctrl_fd, buff, sizeof(buff)); // 226
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			strcpy(reply, "OK. ");
			
			close(d_clientfd);
		}
		else if(!strcmp(buff, "get")) {
			strcpy(cmd, "RETR ");
			strcat(cmd, arg);
			write(ctrl_fd, cmd, strlen(cmd));

			n = read(ctrl_fd, buff, sizeof(buff)); // 150
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			n = read(ctrl_fd, buff, sizeof(buff)); // 226
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			
			close(d_clientfd);
		}
		else if(!strcmp(buff, "put")) {
			strcpy(cmd, "STOR ");
			strcat(cmd, arg);
			write(ctrl_fd, cmd, strlen(cmd));

			n = read(ctrl_fd, buff, sizeof(buff)); // 150
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			n = read(ctrl_fd, buff, sizeof(buff)); // 226
			buff[n] = '\0';
			write(STDOUT_FILENO, buff, strlen(buff));
			
			close(d_clientfd);
		}
		else if(!strcmp(buff, "pwd")) {
			write(ctrl_fd, "PWD", 3);
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "cd")) {
			if(!strcmp(arg, ".."))
				strcpy(cmd, "CDUP");
			else {
				strcpy(cmd, "CWD ");
				strcat(cmd, arg);
			}
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "delete")) {
			strcpy(cmd, "DELE ");
			strcat(cmd, arg);
			arg = strtok_r(NULL, " ", &o_temp);
			while(arg) {
				strcat(cmd, " ");
				strcat(cmd, arg);
				arg = strtok_r(NULL, " ", &o_temp);
			}
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "rename")) {
			char *arg2 = strtok_r(NULL, " ", &o_temp);
			strcpy(cmd, "RNFR ");
			strcat(cmd, arg);
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
			
			strcpy(cmd, "RNTO ");
			strcat(cmd, arg2);
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "mkdir")) {
			strcpy(cmd, "MKD ");
			strcat(cmd, arg);
			arg = strtok_r(NULL, " ", &o_temp);
			while(arg) {
				strcat(cmd, " ");
				strcat(cmd, arg);
				arg = strtok_r(NULL, " ", &o_temp);
			}
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "rmdir")) {
			strcpy(cmd, "RMD ");
			strcat(cmd, arg);
			arg = strtok_r(NULL, " ", &o_temp);
			while(arg) {
				strcat(cmd, " ");
				strcat(cmd, arg);
				arg = strtok_r(NULL, " ", &o_temp);
			}
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "bin")) {
			strcpy(cmd, "TYPE I");
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "ascii")) {
			strcpy(cmd, "TYPE A");
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		}
		else if(!strcmp(buff, "type")) {
		    if(!strcmp(arg, "binary")) {
			strcpy(cmd, "TYPE I");
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		    }
		    else if(!strcmp(arg, "ascii")) {
			strcpy(cmd, "TYPE I");
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
		    }
		}
		else if(!strcmp(buff, "quit")) {
			strcpy(cmd, "QUIT");
			write(ctrl_fd, cmd, strlen(cmd));
			n = read(ctrl_fd, reply, sizeof(reply));
			reply[n] = '\0';
			write(STDOUT_FILENO, reply, strlen(reply));
			close(ctrl_fd);
			exit(1);
		}
	}
}

char *convert_port(unsigned long ip_addr, unsigned int port) {
	char *addr, c_portnum[10];
	char *n_ip, *o_ip;
	struct in_addr sin_ip;
	sin_ip.s_addr = ip_addr;
	
	addr = (char *)malloc(sizeof(char)*20);
	char *dot_ip = inet_ntoa(sin_ip);
	int portnum = ntohs(port);
	
	int bin[16];
	for(int i=15; i>=0; i--) {
		bin[i] = portnum % 2;
		portnum /= 2;
	}
	int pos = 0;
	int height = 0;
	for(int i=7; i>=0; i--) {
		if(bin[i] == 1)
			height += 1 << pos;
		pos++;
	}
	pos = 0;
	int leight = 0;
	for(int i=15; i>=8; i--) {
		if(bin[i] == 1)
			leight += 1 << pos;
		pos++;
	}
	sprintf(c_portnum, "%d,%d", height, leight);
	
	strcpy(addr, "PORT ");
	n_ip = strtok_r(dot_ip, ".", &o_ip);
	while(n_ip) {
		strcat(addr, n_ip);
		strcat(addr, ",");
		n_ip = strtok_r(NULL, ".", &o_ip);
	}
	strcat(addr, c_portnum);
	
	return addr;
}

int log_in(int ctrl_fd) {
	int n;
	char user[128], *passwd, buff[128];
	char username[128], password[128];
	
	strcpy(username, "USER ");
	strcpy(password, "PASS ");
	while(1) {
		write(STDOUT_FILENO, "Name : ", 7);
		n = read(STDIN_FILENO, user, sizeof(user));
		user[n-1] = '\0';
		strcat(username, user);
		write(ctrl_fd, username, strlen(username)); // check username
		n = read(ctrl_fd, buff, sizeof(buff)); // 331, 430
		buff[n] = '\0';
		if(!strncmp(buff, "331", 3)) {
			write(STDOUT_FILENO, buff, strlen(buff));
		    while(1) {
			passwd = getpass("Password : ");
			strcat(password, passwd);
			write(ctrl_fd, password, strlen(password));
			n = read(ctrl_fd, buff, sizeof(buff));
			buff[n] = '\0';
			if(!strncmp(buff, "230", 3)) {
				write(STDOUT_FILENO, buff, strlen(buff));
				return 1;
			}
			else if(!strncmp(buff, "430", 3))
				write(STDOUT_FILENO, buff, strlen(buff));
			else {
				write(STDOUT_FILENO, buff, strlen(buff));
				return -1;
			}
		    }
		}
		else if(!strncmp(buff, "430", 3))
			write(STDOUT_FILENO, buff, strlen(buff));
		else {
			write(STDOUT_FILENO, buff, strlen(buff));
			return -1;
		}
	}
}
