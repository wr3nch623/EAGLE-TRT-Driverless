
class StanleyController{
    private:
        float K; // control error

    public:

        StanleyController(float k): K(k){};

        // computes steering angle based on the formula given in the readme
        float computeDelta(float error, float yerror, float velocity){
            return error + atan((this->K * yerror) / velocity);
        };

        // getter and setters
        float getK() {return this->K;};
        void setK(float k) {this->K = k;};
};