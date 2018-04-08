#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long incircle = 0;
long points_per_thread;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *InCricle() {

    long incircle_thread = 0;

    unsigned int rand_state = rand();
    long i;
    for (i = 0; i < points_per_thread; i++) {

        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

        if (x * x + y * y < 1) {

            incircle_thread++;
            printf("Point ( %lf , %lf ) inside circle of radius 1 unit\n", x, y);

        }
    }

    // printf("Value of shared variable before updation: %ld\n", incircle);
    
    pthread_mutex_lock(&mutex);
    incircle += incircle_thread;
    pthread_mutex_unlock(&mutex);

    // printf("Value shared variable after updation: %ld\n", incircle);
}

/* Calculate Pi by the Monte Carlo method. */

int main()
{
    
    long totalpoints;
    int thread_count;
    int count = 0,a,b;

    do {
        printf("Enter the total no. of points you want to generate ('Accuracy of pi ~ no. of points')\n");
        a = scanf("%ld",&totalpoints); 

        printf("No. of threads you want to use for calulation ('faster caluclation ~ no. of points ~ (depends on C.P.U.)')\n");
        b = scanf("%d",&thread_count);

    } while( thread_count <= 0 | totalpoints <= 0 | a != 1 | b != 1 );

    points_per_thread = totalpoints / thread_count;

    time_t start = time(NULL);

    srand((unsigned)time(NULL));

    pthread_t *threads = malloc(thread_count * sizeof(pthread_t));

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int i;
    for (i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], &attr, InCricle, (void *) NULL);
    }

    for (i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(threads);

    printf("Pi: %f\n", (4. * (double)incircle) / ((double)points_per_thread * thread_count));
    printf("Time: %d sec\n", (unsigned int)(time(NULL) - start));

    return 0;
}