/**
 * @file Session.h
 * @author Piotr Kluba
 * @brief 
 * @version 0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <String>
#include <memory>
#include <Vector>
#include <Game.h>
#include <Map.h>


enum class SessionState {
    IN_GAME,
    START_MENU,
    IN_GAME_MENU, // TODO: fill if necessary
};


struct Save
{
public:
    std::string filename;
    int hours_played;
    int heroes_amount;
    int best_hero_level;
};

class Session
{
private:
    std::unique_ptr<Game> game_;
    void saveGameToFile(std::string filepath);
    Session();
    void loadGameFromFile(std::string filepath);
    static Session* session_;
    SessionState session_state_;
public:
    Session(Session& other) = delete;
    void operator=(const Session& other) = delete;
    ~Session();
    static Session* getInstance();
    const std::vector<Save> getSavedGames();
    const SessionState getSessionState();
    void setSessionState(SessionState new_state) {session_state_ = new_state;};
    void newGame(MapSize size, int number_of_enemies, Difficulty difficulty);

};



