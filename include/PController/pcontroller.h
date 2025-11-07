class PController{
    private:
        float k_d; // This is the proportional gain for heading correction (steering control)
        float k_y; // This is the proportional gain for lateral drift correction (y axis)

    public:
        // Constructors
        PController(float k_d) : k_d(k_d) {};
        PController(float k_d, float k_y) : k_d(k_d), k_y(k_y){};

        /* Functions that compute delta for the steering of the vehicle. The first
           Is only heading correction, the second also corrects sideway drift.
        */
        float computeDelta(float error) {return (k_d*error);};
        float computeDelta(float error, float errory) {return (k_d*error) + (k_y*errory);};

        // Getter and setter declarations
        float getK_p() {return this->k_d;};
        float getK_y() {return this->k_y;};
        void setK_p(float k) {this->k_d = k;};
        void setK_y(float k) {this->k_y = k;};
        
};