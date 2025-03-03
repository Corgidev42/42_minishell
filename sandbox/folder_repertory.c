#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

/*

*/

int main(int argc, char *argv)
{
	char cwd[128];
	struct stat stat_file;
	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(".")) == NULL)
	{
		perror("cannot open .");
		exit(1);
	}

	printf("repertoire actuel : %s\n", 	getcwd(cwd, 128));
	printf("chrdir : %d\n", chdir("/home/corgidev/workspace"));
	printf("repertoire actuel : %s\n", 	getcwd(cwd, 128));
	printf("stat : %d\n", stat("/home/corgidev/workspace/42_minishell/sandbox/test.txt", &stat_file));
	printf("lstat : %d\n", lstat("/home/corgidev/workspace/42_minishell/sandbox/test_ln", &stat_file));
	printf("fstat : %d\n", fstat(0, &stat_file));
	printf("unlink : %d\n", unlink("/home/corgidev/workspace/42_minishell/sandbox/a.out"));

	while((dp = readdir(dir)) != NULL)
	{
		printf("Nom du fichier: %s, inode: %lu, type: %u\n", dp->d_name, (unsigned long)dp->d_ino, dp->d_type);
	}

	printf("succes ? : %d\n", closedir(dir));

}
