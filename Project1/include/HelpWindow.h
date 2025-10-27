#include <string>
#include <iostream>

class HelpWindow {
    public:
        static HelpWindow& getInstance();

        std::string getHelp();
    private:
        HelpWindow();

        HelpWindow(HelpWindow const&);

        void operator=(HelpWindow const&);
    
    public:
        HelpWindow(HelpWindow const&) = delete;
        void operator=(HelpWindow const&) = delete;
};