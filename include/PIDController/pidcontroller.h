#include "../PController/pcontroller.h"
#include <cmath>
#include <limits>

class PIDController : public PController{

    private:
        float k_i; // integral gain 
        float k_d; // derivative gain
        float int_term; // accumulated integral term
        float derivative_term; // accumulated derivative term
        float last_error; // last error used for derivative computation
    
    public:
        // Constructor of PID that inherits from proportional controller. 
        PIDController(float k_p, float k_y, float k_i, float k_d) : PController(k_p, k_y), k_i(k_i), k_d(k_d){
            this->int_term = 0; // set integral term of the PID
            this->derivative_term = 0; // set derivative term of the PID
            this->last_error = std::numeric_limits<float>::quiet_NaN(); // Set the last error to NaN since the controller has not been used yet.
        };

        // Compute new steering angle of the bicycle.
        float computeDelta(float error, float errory, float frametime){
            this->int_term += error * this->k_i * frametime; // compute integral part of the PID 
            if (!std::isnan(this->last_error))
                this->derivative_term = (error - this->last_error) / frametime * this->k_d; // compute derivative if past error exists
            this->last_error = error;
            // returns the three components added to make it work. Proportional part is computed by the superclass.
            return PController::computeDelta(error, errory) + this->int_term + this->derivative_term; 
        }

        // Updates all values when a slider changes value
        void Update(float k_p, float k_y, float k_i, float k_d){
            PController::setK_p(k_p);
            PController::setK_y(k_y);
            this->k_i = k_i;
            this->k_d = k_d;
        }

        // Getter and setter declarations
        float getK_p() {return PController::getK_p();};
        float getK_y() {return PController::getK_y();};
        float getK_i() {return this->k_i;};
        float getK_d() {return this->k_d;};

        void setK_p(float k) {PController::setK_p(k);};
        void setK_y(float k) {PController::setK_y(k);};
        void setK_i(float k) {this->k_i = k;};
        void setK_d(float k) {this->k_d = k;};

};