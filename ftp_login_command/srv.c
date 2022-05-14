#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char *convert_port(char *str);
int check_IP(struct sockaddr_in client_addr, FILE *fpcheck);
int log_auth(int client_fd);

int main(int argc, char **argv)
{
	char *data_ip, *n_temp;
	char temp[25], buff[128], reply[128], pwd[128], result_buff[128];
	struct sockaddr_in server_addr, client_addr;
	int ctrl_fd, len, client_fd;
	FILE *fpcheck;
	fpcheck = fopen("access", "r");

	ctrl_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	bind(ctrl_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(ctrl_fd, 5);
	while(1) {
		char *cmd = NULL;
		char *arg = NULL;
		char *o_temp = NULL;
		char rnfr[128];
		time_t now;
		len = sizeof(client_addr);
		client_fd = accept(ctrl_fd, (struct sockaddr*)&client_addr, &len);
		if(check_IP(client_addr, fpcheck) == 1) {
			char *time_now = ctime(&now);
			time_now[strlen(time_now)-1] = '\0';
			strcpy(reply, "220 sswlab.kw.ac.kr FTP server (version myftp[1.0] ");
			strcat(reply, time_now);
			strcat(reply, ") ready\n");
			write(client_fd, reply, strlen(reply));
			if(log_auth(client_fd) < 0) {
				close(client_fd);
				continue;
			}
		}
		else {
			write(client_fd, "431 This client can't access. Close the session.\n", 47);
			continue;
		}
		
	    while(1) {
		struct sockaddr_in data_server;
		int n, data_fd;
		data_fd = socket(AF_INET, SOCK_STREAM, 0);
		memset(&data_server, 0, sizeof(data_server));

		n = read(client_fd, buff, sizeof(buff));
		buff[n] = '\0';
		if(!strncmp(buff, "PORT", 4)) {
			data_ip = (char *)malloc(sizeof(char)*32);
			data_ip = convert_port(buff);
			data_ip = strtok_r(data_ip, " ", &o_temp);
			n_temp = strtok_r(NULL, " ", &o_temp);
			data_server.sin_family = AF_INET;
			data_server.sin_addr.s_addr = inet_addr(data_ip);
			data_server.sin_port = htons(atoi(n_temp));
			connect(data_fd, (struct sockaddr*)&data_server, sizeof(data_server));
			strcpy(reply, "200 PORT command performed successfully.\n");
			write(client_fd, reply, strlen(reply));
			n = read(client_fd, buff, sizeof(buff));
			buff[n] = '\0';
		}		
		cmd = strtok_r(buff, " ", &o_temp);
		arg = strtok_r(NULL, " ", &o_temp);
		if(!strcmp(cmd, "NLST")) {
			strcpy(reply, "150 Opening data connection for directory list.\n");
			write(client_fd, reply, strlen(reply));
			DIR *dir = opendir("./");
			struct dirent *dp = NULL;
		    if(arg == NULL) {
			while((dp = readdir(dir)) != NULL) {
				if(strncmp(dp->d_name, ".", 1) && strncmp(dp->d_name, "..", 2))
					strcat(result_buff, dp->d_name);
			}
		    }
		    else if(!strcmp(arg, "-a")) {
			while((dp = readdir(dir)) != NULL)
				strcat(result_buff, dp->d_name);
		    }
		    else if(!strcmp(arg, "-l") || !strcmp(arg, "-al")) {
			int i = 0;
			int k = 0;
			char list[11];
			const char *dname[128];
			struct stat st;
			struct passwd *pwuid;
			struct group *grpid;
			struct tm *gettm;
			while((dp = readdir(dir)) != NULL) {
				if(!strcmp(arg, "-l")) {
					if(strncmp(dp->d_name, ".", 1) & strncmp(dp->d_name, "..", 2))
						dname[i++] = dp->d_name;
				}
				else dname[i++] = dp->d_name;
			}
			while(i > 0) {
				lstat(dname[k], &st);
				for(int b=0; b<10; b++)
					list[b] = '-';
				list[10] = '\0';
				if(S_ISDIR(st.st_mode))
					list[0] = 'd';
				if(S_IRUSR & st.st_mode)
					list[1] = 'r';
				if(S_IWUSR & st.st_mode)
					list[2] = 'w';
				if(S_IXUSR & st.st_mode)
					list[3] = 'x';
				if(S_IRGRP & st.st_mode)
					list[4] = 'r';
				if(S_IWGRP & st.st_mode)
					list[5] = 'w';
				if(S_IXGRP & st.st_mode)
					list[6] = 'x';
				if(S_IROTH & st.st_mode)
					list[7] = 'r';
				if(S_IWOTH & st.st_mode)
					list[8] = 'w';
				if(S_IXOTH & st.st_mode)
					list[9] = 'x';
				pwuid = getpwuid(st.st_uid);
				grpid = getgrgid(st.st_gid);
				gettm = localtime(&st.st_mtime);
				char ltime[128];
				strftime(ltime, sizeof(ltime), "%b %d %H:%M", gettm);
				char stnlink[128];
				sprintf(stnlink, "%zu", st.st_nlink);
				char stsize[128];
				sprintf(stsize, "%zu", st.st_size);
				
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
				k++; i--;
			}
		    }
		    write(data_fd, result_buff, strlen(result_buff));
		    strcpy(reply, "226 Complete transmission.\n");
		    write(client_fd, reply, strlen(reply));
		}
		else if(!strcmp(cmd, "RETR")) {}
		else if(!strcmp(cmd, "STOR")) {}
		else if(!strcmp(cmd, "PWD")) {
			getcwd(pwd, 128);
			strcpy(reply, "257 ");
			strcat(reply, pwd);
			strcat(reply, " is current directory.\n");
			write(client_fd, reply, strlen(reply));
		}
		else if(!strcmp(cmd, "CWD")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISDIR(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't find such file or directory.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				chdir(arg);
				strcpy(reply, "250 CWD command succeeds.\n");
				write(client_fd, reply, strlen(reply));
			}
		}	
		else if(!strcmp(cmd, "CDUP")) {
			chdir("..");
			strcpy(reply, "250 CWD command succeeds.\n");
			write(client_fd, reply, strlen(reply));
		}
		else if(!strcmp(cmd, "DELE")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISREG(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't find such file or directory.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				unlink(arg);
				arg = strtok_r(NULL, " ", &o_temp);
				while(arg) {
					unlink(arg);
					arg = strtok_r(NULL, " ", &o_temp);
				}
				strcpy(reply, "250 DELE command performed successfully.\n");
				write(client_fd, reply, strlen(reply));
			}
		}
		else if(!strcmp(cmd, "RNFR")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISDIR(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't find such file or directory.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				strcpy(rnfr, arg);
				strcpy(reply, "350 File exists, ready to rename.\n");
				write(client_fd, reply, strlen(reply));
			}
		}
		else if(!strcmp(cmd, "RNTO")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISDIR(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't be renamed.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				rename(rnfr, arg);
				strcpy(reply, "250 RNTO command succeeds.\n");
				write(client_fd, reply, strlen(reply));
			}
		}
		else if(!strcmp(cmd, "MKD")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISDIR(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't create directory.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				mkdir(arg, 0755);
				arg = strtok_r(NULL, " ", &o_temp);
				while(arg) {
					mkdir(arg, 0755);
					arg = strtok_r(NULL, " ", &o_temp);
				}
				strcpy(reply, "250 MKD command performed successfully.\n");
				write(client_fd, reply, strlen(reply));
			}		
		}
		else if(!strcmp(cmd, "RMD")) {
			struct stat st;
			lstat(arg, &st);
			if(S_ISDIR(st.st_mode) == 0) {
				strcpy(reply, "550 ");
				strcat(reply, arg);
				strcat(reply, ": Can't remove directory.\n");
				write(client_fd, reply, strlen(reply));
			}
			else {
				rmdir(arg);
				arg = strtok_r(NULL, " ", &o_temp);
				while(arg) {
					rmdir(arg);
					arg = strtok_r(NULL, " ", &o_temp);
				}
				strcpy(reply, "250 RMD command performed successfully.\n");
				write(client_fd, reply, strlen(reply));
			}
		}
		//else if(!strcmp(cmd, "TYPE") && !strcmp(arg, "I")) {}
		//else if(!strcmp(cmd, "TYPE") && !strcmp(arg, "A")) {}
		else if(!strcmp(cmd, "QUIT")) {
			strcpy(reply, "221 Goodbye.\n");
			write(client_fd, reply, strlen(reply));
			close(client_fd);
			exit(1);
		}
	    }
	}	
}

char *convert_port(char *str) {
	char *addr, *temp, *n_temp, *o_temp;
	char *p_num;
	addr = (char *)malloc(sizeof(char)*32);
	
	temp = strtok_r(str, " ", &o_temp);
	n_temp = strtok_r(NULL, ",", &o_temp);
	strcpy(addr, n_temp);
	for(int i=0; i<3; i++) {
		n_temp = strtok_r(NULL, ",", &o_temp);
		strcat(addr, ".");
		strcat(addr, n_temp);
	}
	strcat(addr, " ");
	temp = strtok_r(NULL, ",", &o_temp);
	int d_temp = atoi(temp);
	int bin[16];
	for(int i=7; i>=0; i--) {
		bin[i] = d_temp % 2;
		d_temp /= 2;
	}
	temp = strtok_r(NULL, ",", &o_temp);
	d_temp = atoi(temp);
	for(int i=15; i>=8; i--) {
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
	strcat(addr, p_num);

	return addr;
}

int check_IP(struct sockaddr_in client_addr, FILE *fpcheck) {
	char *dot_ip, *n_ac, *n_cli, *o_temp, *o_temp2;
	char ac_ip[128], cli_ip[128];
	int check;
	
	dot_ip = inet_ntoa(client_addr.sin_addr);
	while(fgets(ac_ip, sizeof(ac_ip), fpcheck) != NULL) {
		ac_ip[strlen(ac_ip)-1] = '\0';
		n_ac = strtok_r(ac_ip, ".", &o_temp);
		strcpy(cli_ip, dot_ip);
		n_cli = strtok_r(cli_ip, ".", &o_temp2);
		for(int i=0; i<4; i++) {
			if(!strcmp(n_ac, n_cli) || !strcmp(n_ac, "*"))
				check = 1;
			else {
				check = 0;
				return check;
			}
			n_ac = strtok_r(NULL, ".", &o_temp);
			n_cli = strtok_r(NULL, ".", &o_temp2);
		}
	}
	return check;
}

int log_auth(int client_fd) {
	char user[128], passwd[128], reply[128];
	char *_user, *_passwd;
	int n, check, count = 1;
	FILE *u_fp, *p_fp;
	struct passwd *pw;
	u_fp = fopen("passwd", "r");
	p_fp = fopen("passwd", "r");
	
	while(1) {
		char *n_temp, *o_temp;
		memset(user, 0, sizeof(user));
		n = read(client_fd, user, sizeof(user));
		user[n] = '\0';
		n_temp = strtok_r(user, " ", &o_temp);
		_user = strtok_r(NULL, " ", &o_temp);
		while((pw = fgetpwent(u_fp)) != NULL) {
			if(!strcmp(_user, pw->pw_name)) {
				check = 1;
				break;
			}
			else check = 0;
		}
		if(check == 1) {
			strcpy(reply, "331 Password is required for ");
			strcat(reply, _user);
			strcat(reply, "\n");
			write(client_fd, reply, strlen(reply));
			break;
		}
		else {
			if(count >= 3) {
				strcpy(reply, "530 Failed to log-in\n");
				write(client_fd, reply, strlen(reply));
				return -1;
			}
			strcpy(reply, "430 Invalid username or password\n");
			write(client_fd, reply, strlen(reply));
			count++;
			continue;
		}
	}
	count = 1;
	while(1) {
		char *n_temp, *o_temp;
		memset(passwd, 0, sizeof(passwd));
		n = read(client_fd, passwd, sizeof(passwd));
		passwd[n] = '\0';
		n_temp = strtok_r(passwd, " ", &o_temp);
		_passwd = strtok_r(NULL, " ", &o_temp);
		while((pw = fgetpwent(p_fp)) != NULL) {
			if(!strcmp(_passwd, pw->pw_passwd)) {
				check = 1;
				break;
			}
			else check = 0;
		}
		if(check == 1) {
			strcpy(reply, "230 User ");
			strcat(reply, _user);
			strcat(reply, " logged in\n");
			write(client_fd, reply, strlen(reply));
			return 1;
		}
		else {
			if(count >= 3) {
				strcpy(reply, "530 Failed to log-in\n");
				write(client_fd, reply, strlen(reply));
				return -1;
			}
			strcpy(reply, "430 Invalid username or password\n");
			write(client_fd, reply, strlen(reply));
			count++;
			continue;
		}
	}
}
