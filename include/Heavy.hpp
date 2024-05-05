#ifndef HEAVY_HPP
#define HEAVY_HPP

#include "Particle.hpp"

class Heavy : public Particle{
    double radius;
    int mass;
public:
    Heavy(int& seed);

    Heavy(double x, double y, double vx, double vy);

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