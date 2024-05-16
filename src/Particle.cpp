#include "Particle.hpp"

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>

#include <iostream>
#include <math.h>

double Particle::timeToBorderX(int w) const {
    if(get_velocity_x() > 0){
        return (w - get_x() - get_radius()) / get_velocity_x();
    }
    if(get_velocity_x() < 0){
        return -(get_x() - get_radius()) / get_velocity_x();
    }
    return -1;
}
double Particle::timeToBorderY(int h) const {
    if(get_velocity_y() > 0){
        return (h - get_y() - get_radius()) / get_velocity_y();
    }
    if(get_velocity_y() < 0){
        return -(get_y() - get_radius()) / get_velocity_y();
    }
    return -1;
}

bool Particle::out_of_boundX(int w) const{
    if(get_velocity_x() > 0){
        return get_x() + get_radius() > w;
    }
    if(get_velocity_x() < 0){
        return get_x() - get_radius() < 0;
    }
    return false;
}

bool Particle::out_of_boundY(int h) const{
    if(get_velocity_y() > 0){
        return get_y() + get_radius() > h;
    }
    if(get_velocity_y() < 0){
        return get_y() - get_radius() < 0;
    }
    return false;    
}

double Particle::get_distance_to(const Particle* other) const {
    double dx = other->get_x() - get_x();
    double dy = other->get_y() - get_y();
    return sqrt(dx*dx + dy*dy);
}

double Particle::collide_time(const Particle* p) const {
    /* all formulas were calculed earlier */
    double dX = get_x() - p->get_x();
    double dY = get_y() - p->get_y();
    double A = get_velocity_x() - p->get_velocity_x();
    double B = get_velocity_y() - p->get_velocity_y();
    double D = get_radius() + p->get_radius() + 0.01; //with amortization factor

    double discriminant = (D*D*(A*A+B*B) - (B*dX-A*dY)*(B*dX-A*dY));
    
    if(discriminant > 0){
        //there is a solution
        double t = (-(dX*A + dY*B) - sqrt(discriminant));
        /*  0 solution can be, for example,
                if 2 particles collided independently
                at the same time */
        return (t >= 0) ? (t / (A*A + B*B)) : -1;
    }
    return -1;
}
