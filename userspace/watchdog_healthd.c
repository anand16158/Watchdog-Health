#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define WDT_DEV "/dev/watchdog"

static int fd;

static void cleanup(int sig)
{
	printf("healthd: stopping watchdog pings\n");
	close(fd);
	_exit(0);
}

int main(void)
{
	fd = open(WDT_DEV, O_WRONLY);
	if (fd < 0) {
		perror("open watchdog");
		return 1;
	}

	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);

	printf("healthd: watchdog started\n");

	while (1) {
		write(fd, "\0", 1);   /* keepalive ping */
		sleep(2);
	}
}
