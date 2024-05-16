#include "Light.hpp"
#include "System.hpp"
#include <iostream>
#include <cmath>
#include <random>

double Light::radius = 10;
int Light::mass = 10;

Light::Light(int& seed){ 
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> RandX(radius, GROUNDWIDTH - radius);
    std::uniform_int_distribution<int> RandY(radius, GROUNDHEIGHT - radius);
    std::uniform_real_distribution<double> RandVX(-MAXVELOCITY, MAXVELOCITY);
    std::uniform_real_distribution<double> RandVY(-MAXVELOCITY, MAXVELOCITY);
    x = RandX(gen);
    y = RandY(gen);
    velocity_x = RandVX(gen);
    velocity_y = RandVY(gen);
    seed = gen(); //seed changes after initialization
}

Light::Light(double x, double y, double vx, double vy):Particle(x, y, vx, vy) {}

void Light::move(double time) {
    x += velocity_x * time;
    y += velocity_y * time;
}

void Light::update(int mode, Particle* partner){
    switch (mode){
        case 1:
            //x_border
            velocity_x = -velocity_x;
            break;
        case 2:
            //y_border
            velocity_y = -velocity_y;
            break;
        default:
            //other particle
            if(partner != nullptr){
                double dx = partner->get_x() - x;
                double dy = partner->get_y() - y;
                double distance = sqrt(dx*dx + dy*dy);
                double cosinus = dx / distance;
                double sinus = dy / distance;

                double v1x = velocity_x;
                double v1y = velocity_y;
                double v2x = partner->get_velocity_x();
                double v2y = partner->get_velocity_y();

                double p1 = v1x * cosinus + v1y * sinus;
                double p2 = v2x * cosinus + v2y * sinus;

                double m1 = mass;
                double m2 = partner->get_mass();

                double optimizedP = (2.0 * (p1 - p2)) / (m1 + m2);

                double v1x_final = v1x - optimizedP * m2 * cosinus;
                double v1y_final = v1y - optimizedP * m2 * sinus;
                double v2x_final = v2x + optimizedP * m1 * cosinus;
                double v2y_final = v2y + optimizedP * m1 * sinus;

                set_velocity_x(v1x_final);
                set_velocity_y(v1y_final);
                partner->set_velocity_x(v2x_final);
                partner->set_velocity_y(v2y_final);
            }  
            break;
    }
}
