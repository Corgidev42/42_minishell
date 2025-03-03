#include <stdio.h>
#include <stdlib.h>

#include <term.h>
#include <curses.h>

int	main()
{
	char	*termtype;
	int	success;

	termtype = getenv("TERM");
	if (termtype == NULL)
	{
		printf("Specify a terminal type with 'TERM=<type>'.\n");
		return (1);
	}
	success = tgetent(NULL, termtype);
	if (success < 0)
	{
		printf("Could not access the termcap database.\n");
		return (1);
	}
	else if (success == 0)
	{
		printf("Terminal type '%s' not found in the termcap database.\n", termtype);
		return (1);
	}

	char *cl_cap = tgetstr("cl", NULL);
	tputs(cl_cap, 1, putchar);

	printf("Terminal type: %s\n", termtype);

	printf("Number of columns: %d\n", tgetnum("co"));
	printf("Number of rows: %d\n", tgetnum("li"));
	
	char *mb_cap = tgetstr("mb", NULL);
	tputs(mb_cap, 1, putchar);
	printf("Terminal has meta key: %d\n", tgetflag("km"));

	char *me_cap = tgetstr("me", NULL);
	tputs(me_cap, 1, putchar);

	char *cm_cap = tgetstr("cm", NULL);
	tputs(tgoto(cm_cap, 10, 10), 1, putchar);

	return (0);
}