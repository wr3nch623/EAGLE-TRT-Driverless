class PController{
    private:
        float k_d;
        float k_y;

    public:
        PController(float k_d) : k_d(k_d) {};

        float computeDelta(float error) {return (k_d*error);};

        float getK_d() {return this->k_d;};
        void setK_d(float k) {this->k_d = k;};
};