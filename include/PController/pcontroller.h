class PController{
    private:
        float k_d;
        float k_y;

    public:
        PController(float k_d) : k_d(k_d) {};
        PController(float k_d, float k_y) : k_d(k_d), k_y(k_y){};

        float computeDelta(float error) {return (k_d*error);};
        float computeDelta(float error, float errory) {return (k_d*error) + (k_y*errory);};

        float getK_d() {return this->k_d;};
        void setK_d(float k) {this->k_d = k;};
};