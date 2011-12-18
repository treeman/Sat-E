#pragma once

#include "Tree.hpp"

extern void draw_bar( sf::IntRect box, float perc, Tree::Color foreground,
    Tree::Color background, Tree::Color outline );

extern void draw_outline( sf::IntRect box, Vec2i offset );

