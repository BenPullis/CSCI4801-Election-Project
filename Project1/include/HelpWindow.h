#include <string>
#include <iostream>

class HelpWindow {
    public:

        /**
         * @brief
         * @return
         */
        static HelpWindow& getInstance();

        /**
         * @brief
         */
        void getHelp();

    private:

        /**
         * @brief
         */
        HelpWindow();

        /**
         * @brief
         */
        HelpWindow(HelpWindow const&);

        /**
         * @brief
         */
        void operator=(HelpWindow const&);
    
    public:
        // HelpWindow(HelpWindow const&) = delete;
        // void operator=(HelpWindow const&) = delete;
};