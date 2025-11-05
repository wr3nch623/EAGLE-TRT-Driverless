#include "../PController/pcontroller.h"
#include <cmath>
#include <limits>

class PIDController : public PController{
    private:
        float k_i;
        float k_d;
        float int_term;
        float derivative_term;
        float last_error;
    
    public:
        PIDController(float k_p, float k_y, float k_i, float k_d) : PController(k_p, k_y), k_i(k_i), k_d(k_d){
            this->int_term = 0;
            this->derivative_term = 0;
            this->last_error = std::numeric_limits<float>::quiet_NaN();;
        };

        float computeDelta(float error, float errory, float frametime){
            this->int_term += error * this->k_i * frametime;
            if (!std::isnan(this->last_error))
                this->derivative_term = (error - this->last_error) / frametime * this->k_d;
            this->last_error = error;
            return PController::computeDelta(error, errory) + this->int_term + this->derivative_term;
        }
};