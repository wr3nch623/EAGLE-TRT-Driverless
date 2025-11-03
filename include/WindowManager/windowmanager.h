

class WindowManager{
    private:
        float windowWidth, windowHeight;
        const float baseFontSize = 32;
    public:
        WindowManager();
        ~WindowManager();
        WindowManager(float, float);

        int MainMenu();
        int FirstTask();
        int SecondTask();
};