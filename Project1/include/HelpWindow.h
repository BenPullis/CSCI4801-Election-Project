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
<<<<<<< HEAD

        /**
         * @brief
         */
        HelpWindow();

        /**
         * @brief
         */
        HelpWindow(HelpWindow const&);
=======
        HelpWindow() = default;

        HelpWindow(HelpWindow const&) = default;
>>>>>>> origin/Ben-Dev

        /**
         * @brief
         */
        void operator=(HelpWindow const&);
    
    public:
<<<<<<< HEAD

        /**
         * @brief
         * @param
         */
        HelpWindow(HelpWindow const&) = delete;

        /**
         * @brief
         * @param
         */
        void operator=(HelpWindow const&) = delete;

=======
        // HelpWindow(HelpWindow const&) = delete;
        // void operator=(HelpWindow const&) = delete;
>>>>>>> origin/Ben-Dev
};