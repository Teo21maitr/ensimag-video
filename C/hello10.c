#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_HELLO 10

typedef struct {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} barriere_t;

static void barriere_init(barriere_t *b) {
    b->count = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
}

static void barriere_destroy(barriere_t *b) {
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
}

/* Appelé par chaque thread hello : incrémente le compteur et réveille
   les threads en attente quand le 10ième arrive. */
static void barriereArrivee(barriere_t *b) {
    pthread_mutex_lock(&b->mutex);
    b->count++;
    if (b->count == NB_HELLO)
        pthread_cond_broadcast(&b->cond);
    pthread_mutex_unlock(&b->mutex);
}

/* Bloque jusqu'à ce que les NB_HELLO threads hello soient arrivés. */
static void barriereAttendre(barriere_t *b) {
    pthread_mutex_lock(&b->mutex);
    while (b->count < NB_HELLO)
        pthread_cond_wait(&b->cond, &b->mutex);
    pthread_mutex_unlock(&b->mutex);
}

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *hello(void *arg) {
    barriere_t *b = (barriere_t *)arg;
    pthread_mutex_lock(&print_mutex);
    printf("Hello World!\n");
    pthread_mutex_unlock(&print_mutex);
    barriereArrivee(b);
    return NULL;
}

static void *done(void *arg) {
    barriere_t *b = (barriere_t *)arg;
    barriereAttendre(b);
    pthread_mutex_lock(&print_mutex);
    printf("Done!\n");
    pthread_mutex_unlock(&print_mutex);
    return NULL;
}

int main(void) {
    pthread_t hello_threads[NB_HELLO];
    pthread_t done_thread;

    barriere_t *b = malloc(sizeof(barriere_t));
    if (!b) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    barriere_init(b);

    for (int i = 0; i < NB_HELLO; i++)
        pthread_create(&hello_threads[i], NULL, hello, b);

    pthread_create(&done_thread, NULL, done, b);

    for (int i = 0; i < NB_HELLO; i++)
        pthread_join(hello_threads[i], NULL);
    pthread_join(done_thread, NULL);

    barriere_destroy(b);
    free(b);

    return EXIT_SUCCESS;
}
