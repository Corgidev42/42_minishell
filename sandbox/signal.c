#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	control_c_handler(int sig)
{
	printf("Control-C pressed\n");
}

int	main()
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = control_c_handler;
	
	// sigemptyset is used to initialize the signal set given by set to empty, with all signals excluded from the set.
	sigemptyset(&act.sa_mask);

	// sigaddset is used to add the signal signum to the signal set given by set.
	// act.sa_mask is the signal set that is blocked during the execution of the signal handler.
	sigaddset(&act.sa_mask, SIGQUIT);
	
	// sa_flags is used to set the flags for the signal handler.
	act.sa_flags = 0;
	
	// sigaction is used to change the action taken by a process on receipt of a specific signal.
	sigaction(SIGINT, &act, NULL);
	
	signal(SIGQUIT, SIG_IGN);
	
	int i = 5;
	while (1)
	{
		printf("Hello\n");
		sleep(1);
		if (i-- == 0 || getchar() == EOF)
		{
			kill(getpid(), SIGINT);
			break;
		}
	}
	return (0);
}