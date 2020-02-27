
/*
** this function create a daemon and executes it
**  using the hexdump execution format of a program
** not did but normally this hexdump must be crypted
** and decrypted with randoms keys
*/

/* fwrite fread fopen uses a structure and their are higuer 
** level programation than write, open, etc. More info here
** https://www.programering.com/a/MDOzIzNwATc.html
*/

/*
** argv is a 2d matrix that has in pos 0 the name of the deamon and in
** position [1] : NULL
*/
char            execute_deamon(char **argv, const char *hexdump, char **env)// skip params for encryptation have to documentate before 
{
    int         pid;
    FILE        *fp;

    if (1) // encryptation decriptation things
    {
        if ((fp = fopen(argv[0], "wb+")))
        {
            // need error verification
            (void)fwrite("my encryted-decryted hexdump", sizeof(char), "my clac len", fp);
            (void)fclose(fp);
            if (!(pid = fork()))
            {
                (void)setsid(); // setsid changes zombie process by orphan process if parent die (daemon)
                (void)chmod(argv[0], 777);
                (void)execve(argv[0], &argv[0], env);
            }
        }
        else
            return (FAILURE);
    }
    return (SUCCESS);
}