#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define PORT	40000
int main()
{
/*	int dec = 20000;
	int bin[16];
	int n = sizeof(bin) / sizeof(int);
	for(int i=15; i>=0; i--) {
		bin[i] = dec % 2;
		dec /= 2;
	}
	for(int i=0; i<n; i++)
		printf("%d", bin[i]);
	printf("\n");
	int d = 0;
	int p = 0;
	for(int i=7; i>=0; i--) {
		if(bin[i] == 1)
			d += 1 << p;
		p++;
	}
	printf("%d\n", d); */
	char *temp;
	char *t;
	strcpy(temp, "127,0,0,1");
	temp = strtok_r(temp, ",", &t);
	printf("%s\n", temp);
	temp = strtok_r(NULL, ",", &t);
	printf("%s\n", temp);
//	char *tt = strtok_r(temp, ",", &t);
//	while(tt){
//		printf("%s\n", tt);
//		tt = strtok_r(NULL, ",", &t);
//	}
}
