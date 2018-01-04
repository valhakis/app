#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <window.h>
#include <temporary.h>

#include <X11/Xlib.h>

/* http://slist.lilotux.net/linux/xlib/simple-window.c */

int main(int argc, char *argv[]) {
  temporary_initialize();
  window_initialize();

  /* char char_x = 'c'; */

  /* if (argc > 1) { */
  /*   char_x = argv[1][0]; */
  /* } */

  /* int num = 0, num_x = 0; */

  /* while (1) { */
  /*   if (num > 100000000) { */
  /*     printf("[%c]: okei what is '%d' that\n", char_x, num_x); */

  /*     num_x += 1; */
  /*     num = 0; */
  /*   } */

  /*   num += 1; */
  /* } */

  return 0;
}
