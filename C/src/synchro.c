#include "ensitheora.h"
#include "synchro.h"
#include <pthread.h>

pthread_mutex_t hashmap_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t taille_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  taille_cond  = PTHREAD_COND_INITIALIZER;
static bool            taille_recue = false;

static pthread_mutex_t fenetre_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  fenetre_cond  = PTHREAD_COND_INITIALIZER;
static bool            fenetre_prete = false;

static pthread_mutex_t prod_cons_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  non_plein       = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  non_vide        = PTHREAD_COND_INITIALIZER;
static int             nb_textures     = 0;

void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&taille_mutex);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    taille_recue = true;
    pthread_cond_signal(&taille_cond);
    pthread_mutex_unlock(&taille_mutex);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&taille_mutex);
    while (!taille_recue)
        pthread_cond_wait(&taille_cond, &taille_mutex);
    pthread_mutex_unlock(&taille_mutex);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&fenetre_mutex);
    fenetre_prete = true;
    pthread_cond_signal(&fenetre_cond);
    pthread_mutex_unlock(&fenetre_mutex);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&fenetre_mutex);
    while (!fenetre_prete)
        pthread_cond_wait(&fenetre_cond, &fenetre_mutex);
    pthread_mutex_unlock(&fenetre_mutex);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&prod_cons_mutex);
    while (nb_textures == (int)NBTEX)
        pthread_cond_wait(&non_plein, &prod_cons_mutex);
    pthread_mutex_unlock(&prod_cons_mutex);
}

void finDeposerTexture() {
    pthread_mutex_lock(&prod_cons_mutex);
    nb_textures++;
    pthread_cond_signal(&non_vide);
    pthread_mutex_unlock(&prod_cons_mutex);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&prod_cons_mutex);
    while (nb_textures == 0)
        pthread_cond_wait(&non_vide, &prod_cons_mutex);
    pthread_mutex_unlock(&prod_cons_mutex);
}

void finConsommerTexture() {
    pthread_mutex_lock(&prod_cons_mutex);
    nb_textures--;
    pthread_cond_signal(&non_plein);
    pthread_mutex_unlock(&prod_cons_mutex);
}
