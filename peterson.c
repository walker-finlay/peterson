#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// In n-process peterson's ...
/* q represents the stage each thread is at */
/* turn is set by each process when they arrive at a stage */
static int *q, *turn, n;
struct thread_info { /* Passed as an arg to thread_start() */
    pthread_t   thread_id;
    int         thread_num;
};

/**
 * Thread start function
 */ 
static void * thread_start(void *arg)
{
    struct thread_info *tinfo = arg;
    int i, j, k;
    i = tinfo->thread_num;

    // Begin Peterson's -----------------------------------
    for (j = 1; j < n; j++) {
        q[i] = j;
        turn[j] = i;
        L: for (k = 1; k <= n; k++) {
            if (k == i) continue;
            if ((q[k] >= j) && (turn[j] == i)) goto L;
        }        
    }
    q[i] = n;
    // ######## Critical Section ########
    int m;
    for (m = 0; m < 10; m++) {
        printf("%d", m);
    }
    printf("\n");
    // ####### End crit. section ########
    q[i] = 0;
    // End Peterson's -------------------------------------

    return (void *)0;
}

/**
 * Messing with Peterson's algorithm for n processes
 */ 
int main(int argc, char **argv) 
{
    int opt, tnum;
    extern char *optarg;
    struct thread_info *tinfo;
    void *res;

    /* Get number of threads from command line */
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) 
        {
        case 'n':
            n = atoi(optarg);
            break;
        default:
            break;
        }
    }

    /* Allocate memory and initialize to zero */
    q = calloc(n, sizeof(int));
    turn = calloc(n-1, sizeof(int));
    tinfo = calloc(n, sizeof(struct thread_info));

    /* Create threads */
    for (tnum = 0; tnum < n; tnum++) {
        tinfo[tnum].thread_num = tnum+1;
        pthread_create(&tinfo[tnum].thread_id, NULL, 
                &thread_start, &tinfo[tnum]);
    }

    /* Joinem */
    for (tnum = 0; tnum < n; tnum++) {
        pthread_join(tinfo[tnum].thread_id, &res);
        free(res);
    }

    /* Clean up and exit */
    free(q);
    free(turn);
//    free(tinfo); // Where does this get freed?
    exit(EXIT_SUCCESS);
}
