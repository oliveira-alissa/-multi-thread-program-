#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_DARTS 500000
#define NUMBER_OF_THREADS 4

int dots_inside_sphere = 0;
pthread_mutex_t mutex;

void *runner(void *param);

int main(int argc, const char *argv[]) {
  int points_per_thread = NUMBER_OF_DARTS / NUMBER_OF_THREADS;
  double est_pi;
  double cpu_time_used;
  clock_t start, end;

  pthread_t runners[NUMBER_OF_THREADS];
  pthread_mutex_init(&mutex, NULL);

  start = clock();

  int i;

  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_create(&runners[i], 0, runner, &points_per_thread);
  }

  for (i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(runners[i], NULL);
  }
  double cube_volume = 2.0 * 2.0 * 2.0;
  double volume_of_sphere = cube_volume * ((double)dots_inside_sphere / NUMBER_OF_DARTS);

  end = clock();

  // est_pi = 4.0 * dots_inside_sphere / NUMBER_OF_DARTS;

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  double actual_volume_of_sphere = 4.0 * M_PI * (1 * 1 * 1)/3.0;
  double difference = volume_of_sphere - actual_volume_of_sphere;
  
  printf("Estimated volume of the sphere: %f\n", volume_of_sphere);
  printf("Time taken: %f seconds\n", cpu_time_used);
  printf("Actual Volume of sphere: %f\n", actual_volume_of_sphere);
  printf("Difference between estimated and actual volume of the sphere: %f\n"
         , difference);
// printf("Estimated Pi from %d darts = %f\n", NUMBER_OF_DARTS, est_pi);
  return 0;
}

void *runner(void *param) {
  int points = *((int *)param);
  int hits = 0;
  double x, y, z, distance;

  int i;

  for (i = 0; i < points; i++) {
    x = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    y = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    z = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    
    x = pow(x, 2.0);
    y = pow(y, 2.0);
    z = pow(z, 2.0);

    distance = sqrt(x + y + z);

    if (distance <= 1.0) {
      hits++;
    }
  }

  printf("hits = %d\n", hits);

  pthread_mutex_lock(&mutex);
  dots_inside_sphere = dots_inside_sphere + hits;
  pthread_mutex_unlock(&mutex);

  pthread_exit(0);
}
