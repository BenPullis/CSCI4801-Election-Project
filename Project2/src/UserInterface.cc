#include "UserInterface.h"

UserInterface::UserInterface() {}

UserInterface& UserInterface::getInstance(){
    static UserInterface instance;
    return instance;
}

void UserInterface::getWindow() {
    std::thread serverThread([](){
        std::system("node ../web/server.js");
    });
    serverThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::system("xdg-open http://localhost:8081");
    
    std::cin.get();
}
