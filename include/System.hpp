#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Particle.hpp"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>

#include <iostream>
#include <vector>
#include <pthread.h>

#define SCREENSIZEX 1280
#define SCREENSIZEY 720
#define GROUNDWIDTH 1150
#define GROUNDHEIGHT 720
// frames per second depend on max speed in system to avoid overflight
#define FPS MAXVELOCITY*2
#define TICKTIME 1.0/FPS
#define SCALE 20

    class System{
        std::vector<Particle*> particles;
        double time_end;
        int w, h; //Field parameters
    public:

        System();
        int add(Particle* p);

        /*  Calculates time of next collision
            in whole system (including borders) and
            reserve it in *p_time
            Returns ids of colliding particles */
        std::pair<int,int> nextCollision(double * p_time);

        std::vector<Particle*> get_particles(){ return particles; }

        void update(Particle* p, int id);
        void update(std::pair<int,int> ids);

        /* moves all particles */
        void move(double time);

        ~System(){
            for(int i = 0; i < particles.size(); i++){
                delete particles[i];
            }
        }
    };

#endif