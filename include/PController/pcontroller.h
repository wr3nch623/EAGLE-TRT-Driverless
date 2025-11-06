class PController{
    private:
        float k_d;
        float k_y;

    public:
        // Constructors
        PController(float k_d) : k_d(k_d) {};
        PController(float k_d, float k_y) : k_d(k_d), k_y(k_y){};

        /* Functions that compute delta for the steering of the vehicle. The first
           Is only heading correction, the second also corrects sideway drift.
        */
        float computeDelta(float error) {return (k_d*error);};
        float computeDelta(float error, float errory) {return (k_d*error) + (k_y*errory);};

        float getK_d() {return this->k_d;};
        void setK_d(float k) {this->k_d = k;};
};