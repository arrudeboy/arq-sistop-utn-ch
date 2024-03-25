#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(void)
{
	pid_t children_pid;

	children_pid = fork();

	if (children_pid > 0) {
		puts("Parent: Going to sleep...");
		sleep(20);
		puts("Parent: End");
	} else {
		puts("Children: Going to sleep...");
		sleep(10);
		puts("Children: End");
	}

	return 0;
}