/**
 * @file MainWindowController.h
 * @author Piotr Kluba (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-06-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <Controller.h>
#include <MainView.h>

class MainWindowController : public Controller {
 private:
  MainView* main_wiew_;

 public:
  explicit MainWindowController(MainView* main_wiew) : main_wiew_(main_wiew) {}
  void update(sf::Event& event, SessionState session_state,
              Game& game) override;
};