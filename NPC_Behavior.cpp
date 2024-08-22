#include "NPC_Behavior.h"
#include <thread>
constexpr int small_enemy{2};
constexpr int big_enemy{5};
constexpr int NPC_Wounded{1};
constexpr int NPC_wounded_enemy{3};
constexpr int NPC_DIED{2};
constexpr int NPC_could_run{4};
constexpr int not_possible_to_restore_energy{1};

bool NPC::has_energy() { return energy_ > 50; }

void NPC::run()
{
   // std::cout << "Test_3" << std::endl;
    state_ = new On_guard(this);
    std::thread th{[this]()
                   {
                       //std::cout << "Test_4" << std::endl;
                       state_->guard();
                   }};
    th.detach();
}

State *NPC::get_state_died()
{
    return state_died_;
}

void NPC::on_guard()
{
    auto random{rand() % 5};
    while (random != small_enemy && random != big_enemy)
    {
        std::cout << "NPC On guard!" << std::endl;
        random = rand() % 5;
        //std::cout << "Test_6" << std::endl;
    }
    if (random == small_enemy)
    {
        //std::cout << "Test_7" << std::endl;
        std::thread th{[this]()
                       {
                           state_->see_small_enemy();
                           // std::cout << "Test_1" << std::endl;
                       }};
        th.detach();
        return;
    }
   // std::cout << "Test_8" << std::endl;
    std::thread th{[this]()
                   {
                       //std::cout << "Test_9" << std::endl;
                       state_->see_big_enemy();
                   }};
    th.detach();
}

void NPC::fight()
{
    std::cout << "NPC Fight" << std::endl;
    auto random{rand() % 6};

    while (random != NPC_Wounded)
    {
        if (random == NPC_wounded_enemy)
        {
            std::cout << "NPC wounded the enemy!" << std::endl;
            break;
        }
        std::cout << "NPC and enemy missed!" << std::endl;
        random = rand() % 3;
    }
    if (random == NPC_Wounded)
    {
        std::cout << "NPC Wounded!" << std::endl;
        energy_ -= 50;

        if (has_energy())
        {
            std::thread th{[this]()
                           {
                               state_->energy_Ok();
                           }};
            th.detach();
            return;
        }
        std::thread th{[this]()
                       {
                           state_->losing_fight();
                       }};
        th.detach();
    }
}

void NPC::run_away()
{
    std::cout << "NPC Run_away!" << std::endl;
    auto random{rand() % 5};
    while (random != NPC_DIED && random != NPC_could_run)
    {
        std::cout << "NPC running! " << std::endl;
        random = rand() % 5;
    }
    if (random == NPC_DIED)
    {
        std::cout << "NPC could not escape from a big enemy, NPC DIED!"
                  << std::endl;
        std::thread th{[this]()
                       {
                           state_->died();
                       }};
        th.detach();
        return;
    }
    std::cout << "Ran away from the big enemy " << std::endl;
    std::thread th{[this]()
                   {
                       state_->escaped();
                   }};
}

void On_guard::see_small_enemy()
{
    NPC_->change_state(new Fight(NPC_));
    NPC_->fight();
}

void On_guard::see_big_enemy()
{
    NPC_->change_state(new Run_away(NPC_));
    NPC_->run_away();
}

void On_guard::guard()
{
    if (NPC_ == nullptr)
    {
        std::cout << "Npc obj nullptr!\n";
        return;
    }
    //std::cout << "Test_5" << std::endl;
    NPC_->on_guard();
}

void Fight::losing_fight()
{
    NPC_->change_state(new Run_away(NPC_));
    NPC_->run_away();
}

void Fight::energy_Ok() { NPC_->fight(); }

void Run_away::escaped()
{
    NPC_->change_state(new On_guard(NPC_));
    NPC_->on_guard();
}

void Run_away::died()
{
    std::cout << "NPC object died!" << std::endl;
    NPC_->get_state_died()->call_Next_NPC();
    delete NPC_;
}

Pool_NPC::Pool_NPC(int countNPC, State *died)
{
    for (int i{}; i < countNPC; ++i)
    {
        poollNPC.push_back(new NPC{died});
    }
}

NPC *Pool_NPC::getNextNPC()
{
    auto npc{poollNPC.back()};
    poollNPC.pop_back();
    return npc;
}

void Game::run_NPCs()
{
    if (poollNpc.is_empty())
    {
        std::cout << "All NPC are gone!" << std::endl;
        return;
    }
    auto npc = poollNpc.getNextNPC();
    if (npc == nullptr)
    {
        return;
    }
    npc->run();
}

void Died::call_Next_NPC() { Game_->run_NPCs(); }