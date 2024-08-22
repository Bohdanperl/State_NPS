#pragma once
#include <iostream>
#include <vector>
class NPC;
class State;

class Pool_NPC
{
public:
    Pool_NPC(int countNPC, State *died);
    NPC *getNextNPC();
    bool is_empty() const { return poollNPC.empty(); }

private:
    std::vector<NPC *> poollNPC;
};

class NPC
{
public:
    NPC(State *state_died) : state_died_{state_died} {}
    void change_state(State *state)
    {
        // delete state_;
        state_ = state;
    };
    void on_guard();
    void run_away();
    void fight();
    bool has_energy();
    void run();
    State *get_state_died();

private:
    int energy_{100};
    State *state_{nullptr};
    State *state_died_{nullptr};
};
class Game;
class State
{
public:
    State(NPC *npc) : NPC_{npc}
    {
        // std::cout << "Call constructor State \n";
    }
    State(Game *game) : Game_{game} {}
    virtual void see_small_enemy()
    {
        std::cout << "NOT IMPLEMENTED!" << std::endl;
    }
    virtual void see_big_enemy() { std::cout << "NOT IMPLEMENTED!" << std::endl; }
    virtual void losing_fight() { std::cout << "NOT IMPLEMENTED!" << std::endl; }
    virtual void escaped() { std::cout << "NOT IMPLEMENTED!" << std::endl; }
    virtual void set_Npc(State *state)
    {
        std::cout << "NOT IMPLEMENTED!" << std::endl;
    }
    virtual void died() { std::cout << "NOT IMPLEMENTED!" << std::endl; };
    virtual void energy_Ok() { std::cout << "NOT IMPLEMENTED!" << std::endl; }
    virtual void call_Next_NPC()
    {
        std::cout << "NOT IMPLEMENTED!" << std::endl;
    };
    virtual void guard() { std::cout << "NOT IMPLEMENTED!" << std::endl; };

protected:
    NPC *NPC_{nullptr};
    Game *Game_{nullptr};
};

class On_guard : public State
{
public:
    using State::State;
    void see_small_enemy() override;
    void see_big_enemy() override;
    void guard() override;
};

class Fight : public State
{
public:
    using State::State;
    void losing_fight() override;
    void energy_Ok() override;
};

class Run_away : public State
{
public:
    using State::State;
    void escaped() override;
    void died() override;
};

class Died : public State
{
public:
    using State::State;
    void call_Next_NPC() override;
};

class Game
{
public:
    Game(int countNPC) : state_died{new Died{this}}, poollNpc{countNPC, state_died} {};
    void run_NPCs();
    bool game_end() const { return poollNpc.is_empty(); }

private:
    State *state_died{nullptr};
    Pool_NPC poollNpc;
};