/*
 ============================================================================
 Name        : approxPi.c
 Author      : Joao Digoo
 Version     : v2 - Threads
 Copyright   : Your copyright notice
 Description : C program for MonteCarlo
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/* ------------ CONSTANTS ------------*/
const long DEFAULT_POINTS = 10000000000;
const char TOTAL_POINTS_STAT[] = "Total number of points: ";
const char POINTS_INSIDE_CIRCLE_STAT[] = "Points within the circle: ";
const char PI_ESTIMATION_STAT[] = "Pi estimation: ";
/* ------------ GLOBAL VARS ------------*/
long points_circle = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Is done
void *getCirclePoints(void *pointsPerP) {
	long curr_points_circle = 0;

	unsigned int seed = time(NULL) ^ pthread_self();

	long points = (long) pointsPerP;
	for(long i = 0; i < points; i++) {
		double x = (double)rand_r(&seed)/(double)RAND_MAX;
		double y = (double)rand_r(&seed)/(double)RAND_MAX;

		//printf("(x, y) = (%f, %f)\n", x, y);

		//double x = (double)rand()/(double)RAND_MAX;
		//double y = (double)rand()/(double)RAND_MAX;


		if( (x*x + y*y) <= 1)
			curr_points_circle++;

	}

	pthread_mutex_lock(&mutex);
	points_circle += curr_points_circle;
	pthread_mutex_unlock(&mutex);


	return 0;
}

int main(int argc, char *argv[]) {

	long thread_count = 1;
	long points_count = DEFAULT_POINTS;
	long pointsPerP = points_count;

	if(argc < 3) {
		printf("Please enter the number of points and threads to run. \n> Ex: java approxPi 1000 1\n\n");

	} else {
		thread_count = atoi(argv[2]);

		points_count = atoi(argv[1]);

		pointsPerP = points_count / thread_count;
	}


	pthread_t tids[thread_count];

	int i;
	for(i = 0; i < thread_count; i++) {
		pthread_create(&tids[i], NULL, getCirclePoints, (void *) pointsPerP);
	}

	for(i = 0; i < thread_count; i++) {
		pthread_join(tids[i], NULL);
	}


	double pi_value = 4*((double)points_circle)/((double)points_count);

	printf("Total number of points: %ld\n", points_count);
	printf("Points within the circle: %ld\n", points_circle);
	printf("Pi estimation: %f \n", pi_value);



	pthread_mutex_destroy(&mutex);


	return EXIT_SUCCESS;
}
