#include "bicyclecontroller.h"
#include <cmath>
#include <algorithm>

BicycleController::BicycleController(float x, float y, float delta, float theta, float wheelbase, float velocity) : 
                                    x(x), y(y), delta(delta), theta(theta), l(wheelbase), velocity(velocity) {}

void BicycleController::Step(float frametime){


    this->x = this->x + (this->velocity * cos(this->theta)) * (frametime);
    this->y = this->y + (this->velocity * sin(this->theta)) * (frametime);
    this->theta = theta + ((this->velocity/this->l) * tan(this->delta)) * (frametime);


    // keep theta in check, cause otherwise it will update up to infinity 
    // messing with the controls.
    if(this->theta > M_PI){
        this->theta -= 2 * M_PI;
    }
    if(this->theta < -M_PI){
        this->theta += 2 * M_PI;
    }
}

void BicycleController::Update(float delta, float theta, float wheelbase, float velocity){
    this->delta = delta;
    this->theta = theta;
    this->l = wheelbase;
    this->velocity = velocity;
}

void BicycleController::PStep(float frametime, float delta) {
    this->delta = std::clamp(delta, -0.5f, 0.5f);
    //this->delta = delta;
    this->Step(frametime);
}