#include "NPC_Behavior.h"
#include <iostream>
#include <thread>

int main()
{
    std::cout << "Hi, this is NPC game, let`s start =) \n";
    int count_Npc{};
    std::cout << "Enter, how many NPC you want to create?\n";
    std::cin >> count_Npc;
    
    if (count_Npc <= 0)
    {
        std::cout << "ERROR!!!\n";
        return 0;
    }
    
    Game game{count_Npc};
    game.run_NPCs();
    
    std::thread th{[&game]() {
        while (!game.game_end())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }};
    
    th.join();
    std::cout << "\n";
    std::cout << "NPC game is over )= \n";
    return 0;
}
