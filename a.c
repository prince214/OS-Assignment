#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long inside_circle = 0;
long point_in_thread;
long display_thread_count = 1;

pthread_mutex_t mutex;

void *InCricle() {

    long inside_circle_thread = 0;

    unsigned int rand_state = rand();
    long i;

    for (i = 0; i < point_in_thread; i++) {

        double x = (double)rand()/RAND_MAX*2.0-1.0;
        double y = (double)rand()/RAND_MAX*2.0-1.0;

        if (x * x + y * y < 1) {

            inside_circle_thread++;

        }

        // printf("\nPoint %ld ( %lf , %lf ) inside circle of diameter 1 unit", i+1,x, y);
    }

    // printf("Value of shared variable before updation: %ld\n", inside_circle);
    
    pthread_mutex_lock(&mutex);
    inside_circle += inside_circle_thread;
    pthread_mutex_unlock(&mutex);

    // printf("\nThread %ld is calculated \n", display_thread_count);
    // display_thread_count+=1;

    // printf("Value shared variable after updation: %ld\n", inside_circle);
}

/* Single/Multi Threaded Program to calculate Pi by using Monte Carlo method. */

int main()
{
    
    long total_points;
    int thread_count;
    int count = 1,a,b;
    pthread_mutex_init(&mutex, NULL);

    do {
        
        
        if ( count == 2 ) {

            printf("\nInvalid Input Enter Again | Thread count is too large\n");

        }   

        printf("Enter the total no. of points you want to generate ('Accuracy of pi ~ no. of points')\n");
        a = scanf("%ld",&total_points); 

        printf("\n");

        printf("No. of threads you want to use for calulation ('faster caluclation ~ no. of points ~ (depends on C.P.U.)')\n");
        b = scanf("%d",&thread_count);

        count++;

        if ( count == 3 ) {

            exit(1);

        }      

    } while( thread_count <= 0 | total_points <= 0 | a != 1 | b != 1 | thread_count > 12 );

    point_in_thread = total_points / thread_count;

    if ( point_in_thread == 0 ) {

        point_in_thread = 1;

    }

    time_t start = time(NULL);

    printf("\nCalculating ...\n\n");

    pthread_t threads[thread_count];

    int i;
    for (i = 0; i < thread_count; i++) {

        pthread_create(&threads[i], NULL, InCricle, (void *) NULL);

    }

    for (i = 0; i < thread_count; i++) {

        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("\nFinal Estimated Value of Pi: %f\n", (4. * (double)inside_circle) / ((double)point_in_thread * thread_count));
    printf("Time: %d sec\n\n", (unsigned int)(time(NULL) - start));

    return 0;
}