#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <pthread.h>
#include <stdbool.h>

extern bool fini;

/* Les extern des variables pour la synchro ici */
extern pthread_mutex_t hashmap_mutex;

/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
