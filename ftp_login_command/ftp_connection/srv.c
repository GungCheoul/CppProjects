////////////////////////////////////////////////////////////////////////////
// File Name	: srv.c
// Date		: 2020/06/05
// OS		: Ubuntu 16.04 LTS 64bits
// Author	: Jung Gung Cheoul
// Student ID	: 2016722059
// -------------------------------------------------------------------------
// Title	: System Programming Assignment #3 ( Socket Programming )
// Description 	: Execute command and send result to client
//		  show connected client info
////////////////////////////////////////////////////////////////////////////

// process
// ========================================================================
// Input : *buff -> FTP command
//	   *result_buff -> result of command process
//	   pid -> child process ID
//	   st -> stat of argument
//	   list[] -> permission data of file/dir
//	   *pwuid -> user ID
//	   *grpid -> group ID
//	   *gettm -> time info
//	   temp[128] -> working directory buffer
//	   *dname[128] -> copy of file/dir name
//	   *dir -> directory stream
//	   *dp -> dirent structure
// Output : memset() -> reset buffer
//	    strtok_r() -> cut string
//	    strcmp, strcat -> compare, concatenate string
// 	    opendir, readdir -> open, read directory
//	    getcwd() -> get current working directory
//	    lstat() -> get stat info of argument
// 	    S_IS... -> permission of argument
//	    getpwuid -> get user ID
//	    getgrgid -> get group ID
//	    localtime -> local time
//	    chdir() -> change directory
//	    mkdir() -> make directory
//	    unlink(), rmdir() -> remove file, directory
//	    rename -> rename file/directory
// Purpose : Process command and save to result_buff

// pinfo
// =======================================================================
// Input : cpid[] -> client's child process ID array
//	   c_addr[] -> client address's array
//	   end -> check time end
//	   count -> connected client number
//	   t_count -> count of start time
// Output : printf -> display number of client and info
// Purpose : Display number of client and info

// sh_chld
// =========================================================================
// Input : cpid[] -> client's child process ID
// Output : wait() -> wait until child process ends
// Purpose : SIGCHLD signal handler

// sh_alrm
// =========================================================================
// Input : cpid_copy -> same with cpid
// Output : memcpy -> copy cpid
// 	    alarm -> execute after 10 sec
//	    pinfo -> child process info
// Purpose : SIGALRM signal handler

// main
// =========================================================================
// Input : read -> read buffer from file descriptor
//	   buff -> FTP command
//	   result_buff -> result of command process
// 	   len -> length of client address
//	   n -> byte size of read buff
//	   server_fd, client_fd -> server, client file descriptor
//	   server_addr -> server address
// Output : write -> write buffer to file descriptor
//	    signal() -> set signal
//	    socket() -> open socket
//	    memset() -> reset variable
//	    bind() -> bind server file descriptor and address
//	    listen() -> listen for client's connection
//	    accept() -> accept client's connection
//	    time() -> check service time
//	    alarm() -> alarm after 10 sec
//	    fork() -> creat new child process
// Purpose : Execute command and send result to client
//	     display current client number and infos

// sh_int
// ========================================================================
// Input : signum -> signal number
// Output : kill -> kill all process
//	    close -> disconnect all clients
// Purpose : Kill and disconnect all clients, processes
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define BUFF_SIZE	1024

int cpid[10]; // child process ID
struct sockaddr_in client_addr; // client address
int count = 0; // client number
int t_count[10] = {0,}; // time count
int c_addr[10] = {0,}; // client address array
int sh_clifd[10] = {0,};
int c = 0; // array count
time_t start, end; // time structure for count service time
int process(char *buff, char *result_buff, pid_t pid); // process command
void pinfo(); // process info
void sh_chld(int signum); // SIGCHLD handler
void sh_alrm(int signum); // SIGALRM handler
void sh_int(int signum); // SIGINT handler

int main(int argc, char **argv)
{
	char buff[BUFF_SIZE], result_buff[BUFF_SIZE];
	int len, n;
	int server_fd, client_fd;
	struct sockaddr_in server_addr;
	
	// set signal
	signal(SIGCHLD, sh_chld);
	signal(SIGALRM, sh_alrm);
	signal(SIGINT, sh_int);

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // open
		printf("Server: Can't create socket.\n");
		return -1;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	// bind file descriptor and address
	if(bind(server_fd, (struct sockaddr*)&server_addr,
				sizeof(server_addr)) < 0) {
		printf("Server: Can't bind.\n");
		return -1;
	}
	listen(server_fd, 5); // listen for connection
	while(1) {
		pid_t pid;
		len = sizeof(client_addr);
		// new client connected
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr,
							&len);
		start = time(NULL); // check run time
		t_count[c] = (int)start-1; // array of start time of run time
		count++; // current client number
		alarm(10); // alarm after 10 sec
		sh_clifd[c] = client_fd;
		pid = fork(); // create new child process
		c_addr[c] = ntohs(client_addr.sin_port); // child process's port number
		cpid[c++] = pid; // child process ID
		if(pid == 0) { // child process
			while(1) {
			n = read(client_fd, buff, BUFF_SIZE); // read command
			buff[n] = '\0';
			if(!strcmp(buff, "QUIT")) { // QUIT command
				close(client_fd); // quit client
				exit(1); // quit child process
			}
			process(buff, result_buff, getpid()); // process command
			write(client_fd, result_buff, strlen(result_buff)); // send result to client
			}
		}
		else if(pid > 0) { // child process
			printf("\n==========Client info==========\n");
			printf("client IP: %s\n", inet_ntoa(client_addr.sin_addr));
			printf("client port: %d\n", ntohs(client_addr.sin_port));
			printf("=================================\n");
			printf("Child Process ID : %d\n", pid);
			pinfo(); // process info
			
		}
		close(client_fd);
	}
	return 0;
}

int process(char *buff, char *result_buff, pid_t pid) { // process command
	struct stat st;
	char list[11]; // permission of file/dir
	struct passwd *pwuid; // user ID
	struct group *grpid; // group ID
	struct tm *gettm; // time info
	char temp[128];
	const char *dname[128];
	DIR *dir = opendir("./"); // open current directory
	struct dirent *dp = NULL;
	
	memset(result_buff, 0, sizeof(result_buff)); // reset buffer
	char *o_temp = NULL;
	char *n_temp = NULL;
	char *arg1 = NULL;
	// cut buff by blank
	n_temp = strtok_r(buff, " ", &o_temp);
	arg1 = strtok_r(NULL, " ", &o_temp);
	
	//////////////////// Command execution //////////////////////
	if(!strcmp(buff, "NLST")) { // NLST command
		printf("> %s		[%d]\n", buff, pid);
		while((dp = readdir(dir)) != 0) {
			if(strncmp(dp->d_name, ".", 1) & strncmp(dp->d_name, "..", 2)) { // except '.' and '..'
				strcat(result_buff, dp->d_name);
				strcat(result_buff, "   ");
			}
		}
	}
	else if(!strcmp(buff, "PWD")) { // PWD command
		printf("> %s		[%d]\n", buff, pid);
		getcwd(temp, 128); // current working directory
		strcat(result_buff, temp);
	}
	else if(!strcmp(buff, "LIST")) { // LIST command = NLST -al
		int i = 0;
		int k = 0;
		if(arg1 != NULL) {
			printf("> %s %s		[%d]\n", buff, arg1, pid);
			dir = opendir(arg1);
		}
		else
			printf("> %s		[%d]\n", buff, pid);
		while((dp = readdir(dir)) != 0)
			dname[i++] = dp->d_name;
		while(i > 0) { // display file or dir's info
			lstat(dname[k], &st);
			for(int j=0; j<10; j++)
				list[j] = '-'; // reset authority
			list[10] = '\0';
			if(S_ISDIR(st.st_mode)) // if directory
				list[0] = 'd';
			if(S_IRUSR & st.st_mode) // if user can read
				list[1] = 'r';
			if(S_IWUSR & st.st_mode) // if user can write
				list[2] = 'w';
			if(S_IXUSR & st.st_mode) // if user can execute
				list[3] = 'x';
			if(S_IRGRP & st.st_mode) // if group can read
				list[4] = 'r';
			if(S_IWGRP & st.st_mode) // if group can write
				list[5] = 'w';
			if(S_IXGRP & st.st_mode) // if group can execute
				list[6] = 'x';
			if(S_IROTH & st.st_mode) // if other can read
				list[7] = 'r';
			if(S_IWOTH & st.st_mode) // if other can write
				list[8] = 'w';
			if(S_IXOTH & st.st_mode) // if other can execute
				list[9] = 'x';
			
			pwuid = getpwuid(st.st_uid); // get user ID
			grpid = getgrgid(st.st_gid); // get group ID
			gettm = localtime(&st.st_mtime); // get local time
			char ltime[128];
			// change format of local time
			strftime(ltime, sizeof(ltime), "%b %d %H:%M", gettm);
			char stnlink[128];
			// change nlink to char type
			sprintf(stnlink, "%zu", st.st_nlink);
			char stsize[128];
			// change file/dir size to char type
			sprintf(stsize, "%zu", st.st_size);
			
			// save to result_buff
			strcat(result_buff, list);
			strcat(result_buff, "  ");
			strcat(result_buff, stnlink);
			strcat(result_buff, "  ");
			strcat(result_buff, pwuid->pw_name);
			strcat(result_buff, " ");
			strcat(result_buff, grpid->gr_name);
			strcat(result_buff, "  ");
			strcat(result_buff, stsize);
			strcat(result_buff, "  ");
			strcat(result_buff, ltime);
			strcat(result_buff, " ");
			strcat(result_buff, dname[k]);
			strcat(result_buff, "\n");
			k++;
			i--;
		}
	}
	else if(!strcmp(buff, "CWD")) { // CWD command
		printf("> %s %s		[%d]\n", buff, arg1, pid);
		chdir(arg1); // change directory
		getcwd(temp, 128);
		strcat(result_buff, temp);
		strcat(result_buff, "\n");
	}
	else if(!strcmp(buff, "MKD")) { // MKD command
		printf("> %s %s", buff, arg1);
		strcat(result_buff, "Make dir: ");
		strcat(result_buff, arg1);
		mkdir(arg1, 0755); // make directory by 0755
		arg1 = strtok_r(NULL, " ", &o_temp);
		while(arg1) { // more than 1 argument
			printf(" %s", arg1);
			strcat(result_buff, " ");
			strcat(result_buff, arg1);
			mkdir(arg1, 0755); // make all
			arg1 = strtok_r(NULL, " ", &o_temp);
		}
		printf(" [%d]\n", pid);
	}
	else if(!strcmp(buff, "DELE")) { // DELE command
		printf("> %s %s", buff, arg1);
		strcat(result_buff, "Remove file: ");
		strcat(result_buff, arg1);
		unlink(arg1); // delete file
		arg1 = strtok_r(NULL, " ", &o_temp);
		while(arg1) { // more than 1 argument, delete all
			printf(" %s", arg1);
			strcat(result_buff, " ");
			strcat(result_buff, arg1);
			unlink(arg1);
			arg1 = strtok_r(NULL, " ", &o_temp);
		}
		printf(" [%d]\n", pid);
	}
	else if(!strcmp(buff, "RMD")) { // RMD command
		printf("> %s %s", buff, arg1);
		strcat(result_buff, "Remove dir: ");
		strcat(result_buff, arg1);
		rmdir(arg1); // remove directory
		arg1 = strtok_r(NULL, " ", &o_temp);
		while(arg1) { // more than 1 argument, remove all
			printf(" %s", arg1);
			strcat(result_buff, " ");
			strcat(result_buff, arg1);
			rmdir(arg1);
			arg1 = strtok_r(NULL, " ", &o_temp);
		}
		printf(" [%d]\n", pid);
	}
	else if(!strcmp(buff, "RNFR")) { // RNFR&RNTO command
		char *arg2 = NULL;
		char *arg3 = NULL;
		arg2 = strtok_r(NULL, " ", &o_temp); // RNTO
		arg3 = strtok_r(NULL, " ", &o_temp); // second argument
		printf("> %s %s\n  %s %s	[%d]\n", buff, arg1, arg2, arg3, pid);
		strcat(result_buff, "Rename from: ");
		strcat(result_buff, arg1);
		strcat(result_buff, "\n");
		strcat(result_buff, "Rename to: ");
		strcat(result_buff, arg3);
		rename(arg1, arg3); // rename arg1 -> arg3
	}
	else return -1;
	return 0;
}

void pinfo() { // process info
	end = time(NULL); // end time
	printf("Current Number of Client : %d\n", count); // client number
	printf("  PID	PORT	TIME\n");
	for(int i=0; i<count; i++) // display all client's infp
		printf(" %d	%d	%d\n",
		cpid[i], c_addr[i], (int)end-t_count[i]);
}

void sh_chld(int signum) { // SIGCHLD hanlder
	int status; // wait status
	int r_pid = wait(NULL); // release process ID
	printf("Client(%d)'s Release\n\n", r_pid); // release process ID
	// removing relased process ID info
	int j = 0;
	for(int i=0; i<count; i++) {
		if(r_pid == cpid[i]) { // find same PID, update
			cpid[i] = cpid[i+1];
			c_addr[i] = c_addr[i+1];
			t_count[i] = t_count[i+1];
			j = i + 1;
		}
	}
	for(int i=j; i<count; i++) { // pull all info forward
		cpid[i] = cpid[i+1];
		c_addr[i] = c_addr[i+1];
		t_count[i] = t_count[i+1];
	}
	count--; // decrease number of client
	c--; // decrease array size
	pinfo(); // display remaining info
}

void sh_alrm(int signum) { // SIGALRM handler
	pinfo(); // display remaining info
	alarm(10); // next alarm, repeat by 10 sec
}

void sh_int(int signum) { // SIGINT handler
	for(int i=0; i<c; i++) { // release all clients and processes
		kill(cpid[i], SIGKILL); // kill all processes
		close(sh_clifd[i]); // disconnect all clients
	}
	exit(1); // quit server
}
