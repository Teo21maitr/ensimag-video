#include <SDL2/SDL.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "oggstream.h"
#include "stream_common.h"

int main(int argc, char *argv[static argc+1]) {
  int res;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
  atexit(SDL_Quit);
  assert(res == 0);

  pthread_t theora_thread, vorbis_thread;

  res = pthread_create(&theora_thread, NULL, theoraStreamReader, argv[1]);
  assert(res == 0);
  res = pthread_create(&vorbis_thread, NULL, vorbisStreamReader, argv[1]);
  assert(res == 0);

  // Attendre la fin du thread vorbis
  pthread_join(vorbis_thread, NULL);

  // 1 seconde de son en avance : on laisse le temps à l'audio de se vider
  sleep(1);

  // Annuler les threads vidéo puis attendre leur fin
  pthread_cancel(theora_thread);
  pthread_cancel(theora2sdlthread);
  pthread_join(theora_thread, NULL);
  pthread_join(theora2sdlthread, NULL);

  exit(EXIT_SUCCESS);
}
