#include "System.hpp"

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>

#include <iostream>
#include <vector>


System::System(){}

int System::add(Particle* p){
    particles.push_back(p);
    return particles.size();
}

void System::remove(int id){
    delete particles[id];
    particles.erase(particles.begin() + id);
}

std::pair<int,int> System::nextCollision(double * p_time){
    if(!particles.empty()){
        int id1, id2;
        double t;
        double time = SCREENSIZEX;
        for(int i = 0; i < (int)particles.size(); ++i){
            t = particles[i]->timeToBorderX(GROUNDWIDTH);
            if(0 <= t && t < time){
                time = t;
                id1 = i;
                id2 = -1;
            }
            t = particles[i]->timeToBorderY(GROUNDHEIGHT);
            if(0 <= t && t < time){
                time = t;
                id1 = i;
                id2 = -2;
            }
            if(i < (int)particles.size() - 1)
                for(int j = i+1; j < (int)particles.size(); ++j){
                    double t = particles[i]->collide_time(particles[j]);
                    if(0 <= t && t < time){
                        time = t;
                        id1 = i;
                        id2 = j;
                    }
                }
        }

        *p_time = time;
        return std::pair<int,int>(id1,id2);

    }
    return std::pair<int, int>(-1, -1);
}

void System::move(double time){
    for(int i = 0; i < (int)particles.size(); ++i){
        particles[i]->move(time);
    }
}

void System::update(Particle* p, int id){
    p->update(-id, id>=0? particles[id] : nullptr);
}

void System::update(std::pair<int,int> ids){
    particles[ids.first]->update(-ids.second, ids.second>=0? particles[ids.second] : nullptr);
}