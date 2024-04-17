/**
 * @file Hero.h
 * @author Piotr Kluba
 * @brief Funcionality responsible for the single hero
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRC_GAME_LOGIC_HERO_H_
#define SRC_GAME_LOGIC_HERO_H_

#include <Field.h>

#include <memory>
#include <string>

class Hero {
 private:
  std::string name_;
  std::shared_ptr<Field> field_at_;

 public:
  Hero(std::string name, Field* spawn_field)
      : name_(name), field_at_(std::make_shared<Field>(*spawn_field)){};
  ~Hero();
  const std::string getHeroName() { return name_; };
  const std::pair<int, int> getHeroCoords();
};

#endif