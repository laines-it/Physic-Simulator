#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <math.h>

    //abstract parent for Light, Heavy
    class Particle{
    protected:
        double x, y; //coords relative to System coords
        double velocity_x, velocity_y; //projection of velocity
    public:
        Particle() = default;
        Particle(double x, double y, double velocity_x, double velocity_y):x(x), y(y), velocity_x(velocity_x), velocity_y(velocity_y){}

        /* Getters/setters*/
        double get_x() const { return x; }
        double get_y() const { return y; }
        double get_velocity_x() const { return velocity_x; }
        double get_velocity_y() const { return velocity_y; }
        void set_velocity_x(double vx) { velocity_x = vx; }
        void set_velocity_y(double vy) { velocity_y = vy; }

        /* pure virtual params */
        virtual double get_radius() const = 0;
        virtual double get_mass() const = 0;

        double get_distance_to(const Particle* other) const;

        /* params: w,h - width and height of System */
        double timeToBorderX(int w) const;
        double timeToBorderY(int h) const;

        /* returns time until collision; -1 if won't collide */
        double collide_time(const Particle* p) const;

        /* changes x and y */
        virtual void move(double time) = 0;

        bool out_of_boundX(int w) const;
        bool out_of_boundY(int h) const;

        /* mode: 
            0 = free
            1 = x_border
            2 = y_border
            3 = particle  */
        virtual void update(int mode, Particle* partner = nullptr) = 0;
    };
#endif