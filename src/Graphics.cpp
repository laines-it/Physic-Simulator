#include <iostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <random>
#include <vector>
#include "Graphics.hpp"
#include "Light.hpp"
#include "Heavy.hpp"

ParticleImage::ParticleImage(Particle* p, int id) : Fl_Toggle_Button(p->get_x()-p->get_radius(), p->get_y()-p->get_radius(), 2*p->get_radius(),  2*p->get_radius()){
    this->id = id;
    radius = p->get_radius();
    color = INITCOLOR - p->get_mass();
    std::cout << "created particle in " << x()+radius  << "," << y()+radius << " and rad " << radius << " and mass " << p->get_mass() << "\n";
    box(FL_OVAL_BOX);
    Fl_Toggle_Button::color(color);
};
    
ParticleImage::ParticleImage(int x, int y, int r) : Fl_Toggle_Button(x-r,y-r,2*r,2*r){
    radius = r;
    color = INITCOLOR;
    box(FL_CIRCLE_BOX);
    Fl_Toggle_Button::color(color);
}

//reposition 
void ParticleImage::move(int new_x, int new_y){
    position(new_x-radius, new_y-radius);
    redraw();
}

Ground::Ground(int x, int y, int w, int h, System *sys) : Fl_Group(x,y,w,h){
    system = sys;
    std::cout << "created ground\n";
    fl_color(Fl_Color(FL_WHITE));
    box(FL_DOWN_BOX);
}

void Ground::create_particles(std::vector<Particle*> parts){
    particles.reserve(parts.size());
    for(Particle * p : parts)
        //particles.push_back(new ParticleImage(p));
        add_particle(p);
}

void Ground::add_particle(Particle* p){
    ParticleImage *newimage = new ParticleImage(p, particles.size());
    particles.push_back(newimage);
    add(newimage);
    redraw();
}

void Ground::update(std::vector<Particle*> parts){ 
    for(int i = 0; i < (int)particles.size(); ++i){
        if(!particles[i]->value()){
            particles[i]->move(parts[i]->get_x(), parts[i]->get_y());
        }
    }
    redraw();
}

/*---------------------MAIN WINDOW---------------- */

Main_Window::Main_Window() : Fl_Window(SCREENSIZEX, SCREENSIZEY, "Task2"){
    system = new System();
    ground = new Ground(0, 0, GROUNDWIDTH, GROUNDHEIGHT, system);

    stepButton = new Fl_Button(GROUNDWIDTH, 40, SCREENSIZEX-GROUNDWIDTH, 30, "Do Steps");
    stepButton->callback(step_callback, (void*)this);

    addButton = new Fl_Button(GROUNDWIDTH, 0, SCREENSIZEX-GROUNDWIDTH, 30, "Add Particle");
    addButton->callback(add_callback, (void*)this);
}

void Main_Window::simulate(double time){
    remaining_time = time;
    Fl::add_timeout(0, sim_tick, this);
};

void Main_Window::set_step_time(double time){
    step_time = time;
}

void Main_Window::step_callback(Fl_Widget* w, void* v) {
    Main_Window* win = (Main_Window*) v;
    win->stepButton->deactivate();
    win->addButton->deactivate();
    win->stepButton->hide();
    win->addButton->hide();
    win->set_step_time(120);
    win->until_collision();
}

void Main_Window::until_collision(){
    std::cout << "------------------until collision---------------------" << std::endl;
    for(Particle* p : system->get_particles()){
        std::cout << "particle " << p->get_x() << "," << p->get_y() << std::endl;
    }
    double time;
    for(Particle* p : system->get_particles()){
        if(p->out_of_boundX(GROUNDWIDTH))
            system->update(p,-1);
        else if(p->out_of_boundY(GROUNDHEIGHT))
            system->update(p,-2);
    }
    pair_to_update = system->nextCollision(&time);
    std::cout << "SIMULATING " << time << " for " << pair_to_update.first << "," << pair_to_update.second << std::endl;
    if((step_time -= time) > 0)
        remaining_time = time;
    else
        remaining_time = step_time + time;
    std::cout << "remaining time: " << remaining_time << std::endl; 
    std::cout << "step time: " << step_time << std::endl;
    sim_tick(this);
}

//TimeoutHandler function
void Main_Window::sim_tick(void * w){
    Main_Window* win = (Main_Window*) w;
    if(win->remaining_time > 0){
        win->system->move(TICKTIME);
        win->ground->update(win->system->get_particles());
        win->remaining_time -= TICKTIME;
        Fl::repeat_timeout(TICKTIME, sim_tick, win);
    }else{
        win->system->update(win->pair_to_update);
        win->until_collision();
    }
}

void Main_Window::add(Particle* p){
    ground->add_particle(p);
    system->add(p);
}

void Main_Window::add_callback(Fl_Widget* butt, void* s) {
    Fl_Window *add_dialog = new Fl_Window(450,150 , "Choose particle");

    Fl_Button *light = new Fl_Button(50, 40, 150, 75, "Light Particle");
    light->callback(add_light_callback, s);

    Fl_Button *heavy = new Fl_Button(250, 40, 150, 75, "Heavy Particle");
    heavy->callback(add_heavy_callback, s);

    add_dialog->show();
    Fl::run();
}

bool Main_Window::is_colliding(const Particle* p) const{
    for(const Particle* part : system->get_particles()){
        double dist = p->get_distance_to(part);
        double sumrad = part->get_radius() + p->get_radius();
        if(sumrad > dist){
            std::cout << "collided with " << part->get_x() << "," << part->get_y() << std::endl;
            return true;
        }
    }
    return false;
}

void Main_Window::add_light_callback(Fl_Widget* w, void* v) {
    Main_Window* window = (Main_Window*) v;    
    Light *newparticle;
    do{
        std::random_device rd;
        std::mt19937 gen(rd());
        newparticle = new Light(gen());
        std::cout << newparticle->get_x() << "," << newparticle->get_y() << std::endl;
    }while(window->is_colliding(newparticle));

    window->add(newparticle);
}


void Main_Window::add_heavy_callback(Fl_Widget* w, void* v) {
    Main_Window* window = (Main_Window*)v;
    Heavy *newparticle;
    do{
        std::random_device rd;
        std::mt19937 gen(rd());
        newparticle = new Heavy(gen());
        std::cout << newparticle->get_x() << "," << newparticle->get_y() << std::endl;
    }while(window->is_colliding(newparticle));
    window->add(newparticle);
}