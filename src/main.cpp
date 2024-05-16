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

int main(int argc, char **argv) {
  if(argc < 4) {
    std::cout << "Usage: " << argv[0] << " <seed> <number of lights> <number of heavys>" << std::endl;
  }else{
    int seed = atoi(argv[1]);
    int lights = atoi(argv[2]);
    int heavys = atoi(argv[3]);
    Main_Window * window = new Main_Window(seed, lights, heavys);
    window->show(argc, argv);
    Fl::run();
    delete window;
  }
  return 0;
}