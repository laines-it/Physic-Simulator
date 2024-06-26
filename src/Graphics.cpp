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
    color = INITCOLOR - p->get_mass()/10;
    box(FL_OVAL_BOX);
    Fl_Toggle_Button::color(color);
};
    
ParticleImage::ParticleImage(int x, int y, int r) : Fl_Toggle_Button(x-r,y-r,2*r,2*r){
    radius = r;
    color = INITCOLOR;
    box(FL_OVAL_BOX);
    Fl_Toggle_Button::color(color);
}

//reposition 
void ParticleImage::move(int new_x, int new_y){
    position(new_x-radius, new_y-radius);
    redraw();
}

Ground::Ground(int x, int y, int w, int h, System *sys) : Fl_Group(x,y,w,h){
    system = sys;
    fl_color(Fl_Color(FL_WHITE));
    box(FL_DOWN_BOX);
}

void Ground::create_particles(std::vector<Particle*> parts){
    particles.reserve(parts.size());
    for(Particle * p : parts)
        add_particle(p);
}

void Ground::add_particle(Particle* p){
    ParticleImage *newimage = new ParticleImage(p, particles.size());
    particles.push_back(newimage);
    add(newimage);
    redraw();
}

std::vector<int> Ground::remove_selected(){
    std::vector<int> ids;
    for(int i = (int)particles.size()-1; i >= 0; --i){
        if(particles[i]->value()){
            ids.push_back(i);
            remove(particles[i]);
            delete particles[i];
            particles.erase(particles.begin()+i);
        }
    }
    redraw();
    return ids;
}

void Ground::update(std::vector<Particle*> parts){ 
    for(int i = 0; i < (int)particles.size(); ++i){
        particles[i]->move(parts[i]->get_x(), parts[i]->get_y());
    }
    redraw();
}

/*---------------------MAIN WINDOW---------------- */

Main_Window::Main_Window(int seed, int lights, int heavys) : Fl_Window(SCREENSIZEX, SCREENSIZEY, "Task2"){
    system = new System();

    addButton = new Fl_Button(GROUNDWIDTH, 0, SCREENSIZEX-GROUNDWIDTH, BUTTONHEIGHT, "Add Particle");
    addButton->callback(add_callback, (void*)this);

    stepInput = new Fl_Int_Input(GROUNDWIDTH, BUTTONHEIGHT + BUTTONMARGIN, SCREENSIZEX-GROUNDWIDTH, BUTTONHEIGHT, "steps ");
    stepInput->value("1");

    stepButton = new Fl_Button(GROUNDWIDTH, 2*(BUTTONHEIGHT + BUTTONMARGIN), SCREENSIZEX-GROUNDWIDTH, BUTTONHEIGHT, "Do Steps");
    stepButton->callback(step_callback, (void*)this);

    removeButton = new Fl_Button(GROUNDWIDTH,3*(BUTTONHEIGHT+BUTTONMARGIN), SCREENSIZEX-GROUNDWIDTH, BUTTONHEIGHT, "Remove Particles");
    removeButton->callback(remove_callback, (void*)this);

    ground = new Ground(0, 0, GROUNDWIDTH, GROUNDHEIGHT, system);
    for(int i = 0; i < lights; ++i){
        Light * l;
        do{
            l = new Light(seed);
        }while(is_colliding(l));
        add(l);
    }

    for(int i = 0; i < heavys; ++i){
        Heavy * h;
        do{
            h = new Heavy(seed);
        }while(is_colliding(h));
        add(h);
    }
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
    win->stepButton->copy_label("simulating steps...");
    win->addButton->hide();
    win->removeButton->deactivate();
    win->removeButton->hide();
    win->stepInput->deactivate();
    win->stepInput->hide();
    win->set_step_time(atoi(win->stepInput->value()));
    win->until_collision();
}

void Main_Window::until_collision(){
    double time;
    for(Particle* p : system->get_particles()){
        if(p->out_of_boundX(GROUNDWIDTH))
            system->update(p,-1);
        else if(p->out_of_boundY(GROUNDHEIGHT))
            system->update(p,-2);
    }
    pair_to_update = system->nextCollision(&time);
    step_time -= time;
    if(step_time > 0)
        remaining_time = time;
    else
        remaining_time = step_time + time;
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
        if(win->step_time > 0){
            win->system->update(win->pair_to_update);
            win->until_collision();
        }else{
            win->stepButton->activate();
            win->stepButton->copy_label("Do Steps");
            win->addButton->activate();
            win->stepButton->show();
            win->addButton->show();
            win->removeButton->activate();
            win->removeButton->show();
            win->stepInput->activate();
            win->stepInput->show();
        }
    }
}

void Main_Window::add(Particle* p){
    stepButton->activate();
    stepInput->activate();
    stepButton->copy_label("Do Steps");
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
    // delete add_dialog;
    // delete light;
    // delete heavy;
}

bool Main_Window::is_colliding(const Particle* p) const{
    for(const Particle* part : system->get_particles()){
        double dist = p->get_distance_to(part);
        double sumrad = part->get_radius() + p->get_radius();
        if(sumrad > dist){
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
        int seed = gen();
        newparticle = new Light(seed);
    }while(window->is_colliding(newparticle));

    window->add(newparticle);
}


void Main_Window::add_heavy_callback(Fl_Widget* w, void* v) {
    Main_Window* window = (Main_Window*)v;
    Heavy *newparticle;
    do{
        std::random_device rd;
        std::mt19937 gen(rd());
        int seed = gen();
        newparticle = new Heavy(seed);
    }while(window->is_colliding(newparticle));
    window->add(newparticle);
}

void Main_Window::remove_callback(Fl_Widget* w, void* v) {
    Main_Window* window = (Main_Window*)v;
    std::vector<int> to_remove = window->ground->remove_selected();
    if(to_remove.empty())
        std::cout << "Select particles to remove!" << std::endl;
    else
        for(int id : to_remove)
            window->system->remove(id);

    if(window->ground->is_empty()){
        window->stepButton->deactivate();
        window->stepInput->deactivate();
        window->stepButton->copy_label("(no particles)");
    }
}