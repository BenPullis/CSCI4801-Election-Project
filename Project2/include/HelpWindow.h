#include <string>
#include <iostream>

class HelpWindow {
    public:

        /**
         * @brief Fetches the singleton instance of the HelpWindow.
         * @return A reference to the HelpWindow Singleton.
         */
        static HelpWindow& getInstance();

        /**
         * @brief Prints out the help window.
         */
        void getHelp();

    private:

        /**
         * @brief Constructor method for the help window.
         */
        HelpWindow();

        /**
         * @brief Copy override to ensure validity of Singleton.
         */
        HelpWindow(HelpWindow const&);

        /**
         * @brief Assignment override to ensure validity of Singleton.
         */
        void operator=(HelpWindow const&);
};