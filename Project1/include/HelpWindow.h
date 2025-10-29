#include <string>
#include <iostream>

class HelpWindow {
    public:
        static HelpWindow& getInstance();

        std::string getHelp();
    private:
        HelpWindow() = default;

        HelpWindow(HelpWindow const&) = default;

        void operator=(HelpWindow const&);
    
    public:
        // HelpWindow(HelpWindow const&) = delete;
        // void operator=(HelpWindow const&) = delete;
};