#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <iostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <unistd.h>
#include <vector>
#include "System.hpp"

/* Particles' images are toggle buttons. 
   User can select a one to, for example, delete it */
class ParticleImage : public Fl_Toggle_Button{
  //ToggleButton is false by default
  int id;
  int color;
  double radius;
public:
  ParticleImage(Particle* p, int id);

  ParticleImage(int x, int y, int r);

  int getColor() const { return color; }
  int getId() const { return id; }

  //reposition 
  void move(int new_x, int new_y);
};

/* Ground is a place, where particles are located */
class Ground : public Fl_Group {
  System *system;
  std::vector<ParticleImage*> particles;
  Fl_Group *group;
public:
  Ground(int x, int y, int w, int h, System *system);

  void create_particles(std::vector<Particle*> parts);

  void add_particle(Particle* p);

  void update(std::vector<Particle*> parts);

};

class Main_Window : public Fl_Window {
  Ground *ground;
  System *system;
  std::pair<int,int> pair_to_update;
  double remaining_time;
  double step_time;
  Fl_Button *stepButton;
  Fl_Button *addButton;
  Fl_Button *removeButton;
public:
  Main_Window(int seed, int lights, int heavys);

  Ground* getGround(){ return ground; }

  System* getSystem(){ return system; }

  void simulate(double time);
  void until_collision();
  void set_step_time(double time);

  static void step_callback(Fl_Widget* w, void* v);
  static void add_callback(Fl_Widget* w, void* v);
  static void remove_callback(Fl_Widget* w, void* v);
  static void add_light_callback(Fl_Widget* w, void* v);
  static void add_heavy_callback(Fl_Widget* w, void* v);
  
  void removeParticle();

  void add(Particle* p);

  //checks if the particle collides with others
  bool is_colliding(const Particle* p) const;

  static void sim_tick(void * win);

  ~Main_Window(){
    delete ground;
    delete system;
  }
};

#endif