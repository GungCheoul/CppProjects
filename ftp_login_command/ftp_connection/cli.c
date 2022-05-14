////////////////////////////////////////////////////////////////////////////
// File Name	: cli.c
// Data		: 2020/06/05
// OS		: Ubuntu 16.04 LTS 64bits
// Author	: Jung Gung Cheoul
// Student ID	: 2016722059
// -------------------------------------------------------------------------
// Title	: System Programming Assignment #3 ( Socket Programming )
// Description	: Convert Linux command to FTP command and execute
//		  by using Socket Programming
///////////////////////////////////////////////////////////////////////////

// convert
// ========================================================================
// Input : *buff -> linux command
//	   *cmd_buff -> converted FTP command
//	   *o_temp, *n_temp -> next pointer and cutted string
//	   *arg1 -> argument of command
//	   cdst, mdst, dlst, rmst, rfst, rtst -> stat of argument
// Output : strtok_r -> cut string
//	    strcmp, strcpy, strcat -> compare, copy, concatenate string
//	    lstat -> get stat of argument
//	    S_ISDIR, S_ISREG -> check it is directory, regular file
//	    0 -> success for converting
//	    -1 -> fail for converting
// Purpose : Convert Linux command to FTP command
////////////////////////////////////////////////////////////////////////////
// sh_int
// =========================================================================
// Input : signum -> signal number
// Output : write -> write command to server
//	    exit -> quit client
// Purpose : Signal handler of SIGINT
///////////////////////////////////////////////////////////////////////////
// main
// ========================================================================
// Input : buff -> linux command
//	   rcv_buff -> result of command process
//	   cmd_buff -> converted FTP command
//	   n -> byte size of read buff
//	   sockfd -> socket file descriptor
//	   server_addr, client_addr -> server and client address
//	   write() -> write string to file descriptor
// Output : signal -> set signal with handler
//	    socket() -> open socket
//	    memset() -> reset variables
//	    connect() -> connect with server
//	    read() -> read string from file descriptor
// Purpose : Convert Linux command to FTP command and send to server
//	     receive result of command process
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFF_SIZE	1024 // buffer size

int sockfd, intfd; // socket file descriptor and fd for ^C signal
int convert(char buff[BUFF_SIZE], char cmd_buff[BUFF_SIZE]); // converting
void sh_int(int signum); // SIGINT handler

int main(int argc, char **argv)
{
	char buff[BUFF_SIZE], rcv_buff[BUFF_SIZE], cmd_buff[BUFF_SIZE];
	// linux command, result of command process, converted command
	int n;
	struct sockaddr_in server_addr, client_addr;
	
	signal(SIGINT, sh_int); // set signal
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // open socket
		printf("Can't creat socket.\n");
		return -1;
	}

	memset(cmd_buff, 0, sizeof(cmd_buff));
	memset(rcv_buff, 0, sizeof(rcv_buff));
	memset(buff, 0, sizeof(buff));
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = inet_addr(argv[1]); // 127.0.0.1
	server_addr.sin_port = htons(atoi(argv[2])); // port number
	
	if((connect(sockfd, (struct sockaddr*)&server_addr,
					sizeof(server_addr))) < 0) {
		printf("Can't connect.\n");
		return -1;
	} // connect server
	intfd = sockfd; // copy socket fd
	
	while(1) { // loop until ^C or quit
		write(STDOUT_FILENO, "> ", 2);
		n = read(STDIN_FILENO, buff, BUFF_SIZE); // insert command
		buff[n-1] = '\0';
		if(!strcmp(buff, "quit")) { // quit inserted
			write(sockfd, "QUIT", 4); // send QUIT
			exit(1); // quit client
		}
		if(convert(buff, cmd_buff) < 0) { // convert command
			printf("Convert error.\n");
			memset(rcv_buff, 0, sizeof(rcv_buff));
			continue;
		}
		// send converted command to server and receive result
		if(write(sockfd, cmd_buff, strlen(cmd_buff)+1) > 0) {
			if(read(sockfd, rcv_buff, BUFF_SIZE) > 0)
				printf("%s\n", rcv_buff);
			else {
				printf("Server disconnected.\n");
				exit(1);
			}
		}
		memset(rcv_buff, 0, sizeof(rcv_buff)); // reset rcv_buff
	}
	close(sockfd);
	return 0;
}

int convert(char *buff, char *cmd_buff) { // converting
	char *o_temp = NULL;
	char *n_temp = NULL;
	char *arg1 = NULL;
	// cut inserted command by blank
	n_temp = strtok_r(buff, " ", &o_temp);
	arg1 = strtok_r(NULL, " ", &o_temp);
	
	//////////// Start converting ////////////////////
	if(!strcmp(buff, "ls")) { // ls command
		if(arg1 != NULL) return -1;
		else strcpy(cmd_buff, "NLST");
	}
	else if(!strcmp(buff, "pwd")) { // pwd command
		if(arg1 != NULL) return -1;
		else strcpy(cmd_buff, "PWD");
	}
	else if(!strcmp(buff, "dir")) { // dir command (= ls -al)
		strcpy(cmd_buff, "LIST");
		if(arg1 != NULL) {
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
		}
	}
	else if(!strcmp(buff, "cd")) { // cd command
		struct stat cdst;
		lstat(arg1, &cdst);
		if(arg1 == NULL) return -1;
		else if(S_ISDIR(cdst.st_mode) == 0) return -1;
		else {
			strcpy(cmd_buff, "CWD");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
		}
	}
	else if(!strcmp(buff, "mkdir")) { // mkdir command
		struct stat mdst;
		lstat(arg1, &mdst);
		if(arg1 == NULL) return -1;
		else if(S_ISDIR(mdst.st_mode) == 1) return -1;
		else {
			strcpy(cmd_buff, "MKD");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
			arg1 = strtok_r(NULL, " ", &o_temp);
			while(arg1) {
				lstat(arg1, &mdst);
				if(S_ISDIR(mdst.st_mode) == 1) return -1;
				strcat(cmd_buff, " ");
				strcat(cmd_buff, arg1);
				arg1 = strtok_r(NULL, " ", &o_temp);
			}
		}
	}
	else if(!strcmp(buff, "delete")) { // delete command
		struct stat dlst;
		lstat(arg1, &dlst);
		if(arg1 == NULL) return -1;
		else if(S_ISREG(dlst.st_mode) == 0) return -1;
		else {
			strcpy(cmd_buff, "DELE");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
			arg1 = strtok_r(NULL, " ", &o_temp);
			while(arg1) {
				lstat(arg1, &dlst);
				if(S_ISREG(dlst.st_mode) == 0) return -1;
				strcat(cmd_buff, " ");
				strcat(cmd_buff, arg1);
				arg1 = strtok_r(NULL, " ", &o_temp);
			}
		}
	}
				
	else if(!strcmp(buff, "rmdir")) { // rmdir command
		struct stat rmst;
		lstat(arg1, &rmst);
		if(arg1 == NULL) return -1;
		else if(S_ISDIR(rmst.st_mode) == 0) return -1;
		else {
			strcpy(cmd_buff, "RMD");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
			arg1 = strtok_r(NULL, " ", &o_temp);
			while(arg1) {
				lstat(arg1, &rmst);
				if(S_ISDIR(rmst.st_mode) == 0) return -1;
				strcat(cmd_buff, " ");
				strcat(cmd_buff, arg1);
				arg1 = strtok_r(NULL, " ", &o_temp);
			}
		}
	}
	else if(!strcmp(buff, "rename")) { // rename command
		struct stat rfst;
		lstat(arg1, &rfst);
		char *arg2 = strtok_r(NULL, " ", &o_temp);
		struct stat rtst;
		lstat(arg2, &rtst);
		if(arg1 == NULL || arg2 == NULL) return -1;
		else if(S_ISDIR(rfst.st_mode) == 0) return -1;
		else if(S_ISDIR(rtst.st_mode) == 1) return -1;
		else {
			strcpy(cmd_buff, "RNFR");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg1);
			strcat(cmd_buff, " ");
			strcat(cmd_buff, "RNTO");
			strcat(cmd_buff, " ");
			strcat(cmd_buff, arg2);
		}
	}
	else return -1;
	//////////// End converting //////////////////////
	return 0;
}

void sh_int(int signum) { // SIGINT handler
	write(intfd, "QUIT", 4); // send QUIT to server
	exit(1); // quit client
}

