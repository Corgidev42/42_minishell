#include <stdio.h>
#include <unistd.h>

int	main()
{
	for (int i = 0; i < 100; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}
	return (0);
}