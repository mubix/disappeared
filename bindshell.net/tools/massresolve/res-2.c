/*
 * MassResolve (C) 2000 John Anderson (john@ev6.net) This program performs
 * reverse dns lookups for network blocks or an input file and supports multi
 * threading.
 * 
 * Minor tweaks (2007)
 */

#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <setjmp.h>

char           *
rlookup(unsigned long ip, char *src)
{
	static char     hostname[256];
	struct hostent *host;
	struct in_addr  ami;
	struct sockaddr_in addr;
	char            scheisse[100];

	addr.sin_addr.s_addr = ip;
	host =
		gethostbyaddr((char *) &addr.sin_addr, sizeof(addr.sin_addr), AF_INET);

	ami.s_addr = ip;

	if (host == NULL) {
		ami.s_addr = ip;
		printf("%s does not resolve.\n", src);
	} else {
		printf("%s resolves to %s\n", src, host->h_name);
	}
	return 0;
}

int             maxforq = 0;

int
main(int argc, char **argv)
{
	int             pid, k, j, frk, i, mforks;
	char           *pt;
	char            sv[3];
	char            mehost[26];
	FILE           *inp;

	mforks = 64;		/* default */
	if (argv[1] == NULL) {
		printf("Usage: %s <subnet or filename to resolve> <forks>\n", argv[0]);
		exit(0);
	}
	if (argc>=3) {
		mforks = atoi(argv[2]);
	}
	/*
	 * printf ("Resolving all hosts in %s with %d threads\n", argv[1],
	 * mforks);
	 */
	if ((inp = fopen(argv[1], "r")) != NULL) {
		while (fgets(mehost, sizeof(mehost), inp)) {
			sscanf(mehost, "%s\n", mehost);
			frk = fork();
			if (frk == 0) {
				fflush(stdout);
				rlookup(inet_addr(mehost), mehost);
				exit(0);
			} else {/* frk = 0 */
				maxforq++;
				if (maxforq > mforks) {
					wait(NULL);	/* waiting until a child
							 * died */
					maxforq--;
				}
			}

		}
		fclose(inp);
		exit(0);
	}
	pt = strchr(argv[1], '.');
	if (pt == NULL) {	/* ANET */
		for (k = 0; k <= 255; k++) {
			for (j = 0; j <= 255; j++) {
				for (i = 1; i <= 255; i++) {

					frk = fork();
					if (frk == 0) {
						snprintf(mehost, sizeof(mehost), "%s.%u.%u.%u",
							 argv[1], k, j, i);
						fflush(stdout);
						rlookup(inet_addr(mehost), mehost);
						exit(0);
					} else {	/* frk = 0 */
						maxforq++;
						if (maxforq > mforks) {
							wait(NULL);	/* waiting until a child
									 * died */
							maxforq--;
						}
					}
				}
			}
		}
		fprintf(stderr, "%u ", j);
	} else {
		pt++;
		pt = strchr(pt, '.');
		if (pt == NULL) {	/* BNET */
			for (j = 0; j <= 255; j++) {
				for (i = 1; i <= 255; i++) {

					frk = fork();
					if (frk == 0) {
						snprintf(mehost, sizeof(mehost), "%s.%u.%u",
							 argv[1], j, i);
						fflush(stdout);
						rlookup(inet_addr(mehost), mehost);
						exit(0);
					} else {
						maxforq++;
						if (maxforq > mforks) {
							wait(NULL);	/* waiting until a child
									 * died */
							maxforq--;
						}
					}
				}
			}
			fprintf(stderr, "%u ", j);
		} else {	/* CNET */
			pt++;
			pt = strchr(pt, '.');
			if (pt != NULL) {	/* Single host? */
				rlookup(inet_addr(argv[1]), argv[1]);
			} else {
				for (i = 1; i <= 255; i++) {
					frk = fork();
					if (frk == 0) {
						snprintf(mehost, sizeof(mehost), "%s.%u", argv[1], i);
						fflush(stdout);
						rlookup(inet_addr(mehost), mehost);
						exit(0);
					} else {
						maxforq++;
						if (maxforq > mforks) {
							wait(NULL);	/* waiting until a child
									 * died */
							maxforq--;
						}
					}
				}
				fprintf(stderr, "!! %u !!", k);
			}
		}
	}
}
