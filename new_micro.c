#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int fd = 0;

//perr
static void perr(char *str)
{
    while (*str)
        write(2, str++, 1);
}
//cd
static int cd(char **av, int i)
{
    if (i != 2)
        return (perr("error: cd: bad arguments\n"), 1);
    else if (chdir(av[1]) == -1)
        return (perr("error: cd: cannot change directory to "), perr(av[1]), perr("\n"), 1);
    return (0);
}

//exec
static int exec(char **av, char **env, int i)
{
    int fds[2];
    int status;
    int pip = (av[i] && !strcmp(av[i], "|"));
    if (pip && pipe(fds) == -1)
        return (perr("error: fatal\n"), 1);
    int pid = fork();
    if (!pid)
    {
        av[i]= 0;
        if (pip && (dup2(fds[1], 1) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
            return (perr("error: fatal\n"), 1);
        execve(*av, av, env);
        return (perr("error: cannot execute "), perr(*av), perr("\n"), 1);
    }
    waitpid(pid, &status, 0);
    if (pip && (dup2(fds[0], 0) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
        return (perr("error: fatal\n"), 1);
    return WIFEXITED(status) && WEXITSTATUS(status);
}
//main
int main(int ac, char **av, char **env)
{
    int status = 0;
    (void)ac;
    while (*av && *(av + 1))
    {
        av++;
        int i = 0;
        while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (!strcmp(*av, "cd"))
            status = cd(av, i);
        else if (i)
            status = exec(av, env , i);
        av += i;
    }
    return (status);
}