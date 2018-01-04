#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <window.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

/* DONE: make a window */
/* DONE: close window with escape */
/* DONE: draw a triangle with opengl */

static Window win;
static Window root;
static Colormap colormap;
static Display *display;
static XVisualInfo *xvisual;
static int xvisual_attrib_list[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
static int screen_num;
static XSetWindowAttributes set_window_attributes;
static XWindowAttributes get_window_attributes;
static GLXContext xcontext;
static uint display_width;
static uint display_height;
static uint width;
static uint height;
static uint win_x, win_y;
static uint win_border_width;
static char *display_name;
static XEvent xevent;
static const uint KEY_ESCAPE = 9;

static void draw_a_triangle() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f(1.0f, 0.3f, 0.1f);
  glBegin(GL_TRIANGLES);
  glVertex3f(-0.5f, -0.5f, 0.0f);
  glVertex3f(+0.5f, -0.5f, 0.0f);
  glVertex3f(+0.0f, +0.5f, 0.0f);
  glEnd();
}

int window_initialize() {
  display_name = getenv("DISPLAY");
  display = XOpenDisplay(display_name);

  if (display == NULL) {
    fprintf(stderr, "ERROR: 'cannot open display'.\n");
    return -1;
  }

  /* get the geometry of the default screen for out display. */
  screen_num = DefaultScreen(display);
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);

  /* make the new window occupy 1/9 of the screen size. */

  width = (display_width / 3);
  height = (display_height / 3);

  /* window should be placed at the top-left corner of the screen. */
  win_x = 0;
  win_y = 0;

  /* the window's border shall be 2 pixels wide. */
  win_border_width = 2;

  /* get root window */
  root = DefaultRootWindow(display);

  /* choose visual */
  xvisual = glXChooseVisual(display, 0, xvisual_attrib_list);
  if (xvisual == NULL) {
    printf("ERROR: 'NO APPROPRIATE VISUAL FOUND'.\n");
  }

  /* create colormap */
  colormap = XCreateColormap(display, root, xvisual->visual, AllocNone);

  /* set window attributes */
  set_window_attributes.colormap = colormap;
  set_window_attributes.event_mask = ExposureMask | KeyPressMask;

  /* create a simple window, as a direct child of the screen's root window */
  /* use screen's white color as the background color of the window. */
  /* place the new window's top-left corender at the given 'x,y' coordinates */
  // win = XCreateSimpleWindow(display, RootWindow(display, screen_num), 
  // win_x, win_y, width, height, win_border_width, 
  // BlackPixel(display, screen_num), WhitePixel(display, screen_num));
  win = XCreateWindow(
      display, 
      root, 
      win_x, 
      win_y, 
      width, 
      height, 
      win_border_width, 
      xvisual->depth, 
      InputOutput, 
      xvisual->visual, 
      CWColormap | CWEventMask, 
      &set_window_attributes
      );
  XSelectInput(display, win, ExposureMask | KeyPressMask);

  /* make the window actually appear on the screen. */
  XMapWindow(display, win);

  /* change title bar string */
  XStoreName(display, win, "VERY MISTERIOUS WINDOW");

  /* create context */
  xcontext = glXCreateContext(display, xvisual, NULL, GL_TRUE);

  /* make context current */
  glXMakeCurrent(display, win, xcontext);
  
  /* enable depth test */
  glEnable(GL_DEPTH_TEST);

  for (;;) {
    XNextEvent(display, &xevent);

    /* draw or redraw the window */
    if (xevent.type == Expose) {
      /* get window attributes */
      XGetWindowAttributes(display, win, &get_window_attributes);

      /* draw rectangle */
      XFillRectangle(display, win, DefaultGC(display, screen_num), 20, 20, 10, 10);

      /* set opengl viewport */
      glViewport(0, 0, get_window_attributes.width, get_window_attributes.height);

      draw_a_triangle();
      glXSwapBuffers(display, win);

      /* draw string */
      const char *message = "MESSAGE TO DRAW";
      XDrawString(display, win, DefaultGC(display, screen_num), 50, 50, message, strlen(message));
    }

    /* handle keypress event */
    if (xevent.type == KeyPress) {
      uint keycode = xevent.xkey.keycode;
      /* printf("key: [%d].\n", keycode); */
      if (keycode == KEY_ESCAPE) {
        break;
      }
    }
  }

  /* flush all pending requests to the X server, and wait until they are processed by the X server. */
  /* XSync(display, False); */

  /* make a delay for a short period. */
  /* sleep(4); */

  /* set current null */
  glXMakeCurrent(display, None, NULL);

  /* destroy the context */
  glXDestroyContext(display, xcontext);

  /* destroy the window */
  XDestroyWindow(display, win);

  /* close the connection to the X server. */
  XCloseDisplay(display);

  return 0;
}
