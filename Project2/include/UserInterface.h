#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

class UserInterface {
    public:

        /**
         * @brief Fetches the singleton instance of the UserInterface.
         * @return A reference to the UserInterface Singleton.
         */
        static UserInterface& getInstance();

        /**
         * @brief Starts node.js server and opens webpage (see /web directory in project root) containing the user interface.
         */
        void getWindow();

    private:

        /**
         * @brief Constructor method for the help window.
         */
        UserInterface();

        /**
         * @brief Copy override to ensure validity of Singleton.
         */
        UserInterface(UserInterface const&);

        /**
         * @brief Assignment override to ensure validity of Singleton.
         */
        void operator=(UserInterface const&);
};