#include <cmath>

class BicycleController{
    private:
        float x, y, delta, theta, l, velocity;

    public:
        // Constructor definition
        BicycleController(float x, float y, float delta, float theta, float wheelbase, float velocity);

        // Getter declaration
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getDelta() {return this->delta;};
        float getTheta() {return this->theta;};
        float getWheelBase() {return this->l;};
        float getVelocity() {return this->velocity;};

        void setX(float x) {this->x=x;};
        void setY(float y) {this->y=y;};
        void setTheta(float theta) {this->theta = theta;};
        void setDelta(float delta) {this->delta = delta;};

        // Step function, this is needed to update every variable for the path
        // of the bicycle
        void Step(float);
        void Update(float delta, float theta, float wheelbase, float velocity);
        void PStep(float, float);

};