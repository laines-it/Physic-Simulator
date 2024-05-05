#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Particle.hpp"

class Light : public Particle{
    static double radius;
    static int mass;
public:
    Light(int& seed);

    Light(double x, double y, double vx, double vy);
    
    double get_radius() const {
        return radius;
    }
    int get_mass() const {
        return mass;
    }

    //changes x and y
    void move(double time);

    void update(int mode, Particle* partner = nullptr);
    
};


#endif