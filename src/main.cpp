#include <iostream>
#include "Light.hpp"
#include "Heavy.hpp"
#include "Graphics.hpp"

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>

pthread_t thread;
double global_time;

Main_Window *window;

int main(int argc, char **argv) {
  window = new Main_Window();
  window->show(argc, argv);
  Fl::run();
  return 0;
}