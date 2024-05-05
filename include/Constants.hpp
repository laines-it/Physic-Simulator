#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const int SCREENSIZEX = 1280;
const int SCREENSIZEY = 720;
const int GROUNDWIDTH = 1150;
const int GROUNDHEIGHT = 720;
const int BUTTONHEIGHT = 30;
const int BUTTONMARGIN = 10;
const int MAXVELOCITY = 100;
// frames per second depend on max speed in system to avoid overflight
const int FPS = MAXVELOCITY*2;
const double TICKTIME = 1.0/FPS;
const int SCALE = 20;
const int INITCOLOR = 72;
#endif