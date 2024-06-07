#include <unistd.h>
#include <sys/wait.h>

int ft_cd(char **arg)
{
	int acount;
	char **tmp;

	tmp = arg;
	acount = 0;
	while (*tmp)
	{
		acount++;
		tmp++;
	}
	if (acount != 2)
		return (puterror("error : Bad Argument\n"), 1);
	arg++;
	if (chdir(*arg) == -1)
	{
		puterror("error : cannot change to ");
		puterror(*arg);
		puterror("\n");
	}
	return (0);
}

int ft_strlen(char *str)
{
	int len;

	len = 0;
	while (*str)
	{
		len++;
		str++;
	}
	return (len);
}

void puterror(char *str)
{
	while (*str)
	{
		write(2, str, 1);
		str++;
	}
}

int main(int ac, char **av, char **env)
{
	int argcount;
	int pip[2];
	char **tmp;
	int pid;
	int is_pipe;

	while (*av)
	{
		tmp = av;
		argcount = 0;
		is_pipe = 0;
		while (*tmp)
		{
			if (strncmp(*tmp, "|", ft_strlen(*tmp)) == 0 || strncmp(*tmp, ";", ft_strlen(*tmp) == 0))
			{
				if (strncmp(*tmp, "|", ft_strlen(*tmp)))
				{
					if (-1 == pipe(pip))
						return (puterror("error : Fatal\n"), 1);
					is_pipe = 1;
					break;
				}
			}
			argcount++;
			tmp++;
		}

		pid = fork();
		if (pid == -1)
			return (puterror("error : Fatal\n"), 1);
		if (pid == 0)
		{
			if (is_pipe == 1)
			{
				dup2(pip[1], 1);
				close(pip[1]);
				close(pip[0]);
			}
			av[argcount] = NULL;
			if (strncmp(*av, "cd", ft_strlen(*av)) == 0)
				ft_cd(av);
			else
			{
				execve(*av, av, env);
				puterror("error : the program cannot exe ");
				puterror(*av);
				puterror("\n");
			}
			exit(1);
		}
		else
		{
			if (is_pipe == 1)
			{
				dup2(pip[0], 0);
				close(pip[0]);
				close(pip[1]);
				is_pipe = 0;
			}
			waitpid(pid, NULL, WUNTRACED);
		}	
		av = av + argcount;
	}
}