//
// Created by rushy on 24-5-24.
//
#include "base.hpp"
#include "basetype.hpp"
#include "chanel.hpp"
//#include <graphics.h>
#include "../HiEasyX.h"
#include <vector>
#ifndef TANK_BATTLE_GRAP_HPP
#define TANK_BATTLE_GRAP_HPP
extern Tank_info local;
extern hiex::Scene main_scene;
extern hiex::Layer map_layer;
extern hiex::Layer body_layer;
extern hiex::Layer turret_layer;
extern hiex::Canvas Map_canvas;
extern hiex::ImageBlock map_block;
extern hiex::Canvas Screen;



void rotate_draw(draw_buffer *buffer, double angle, int x, int y);

void tank_turret(IMAGE* original, IMAGE* body, IMAGE* turret,
                 position body_pos, position turret_pos,
                 int bodyWidth, int turretWidth);
void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y,  int turretOffsetX, int turretOffsetY);

//void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y, int body_width,
//          int turret_width, int turretOffsetX, int turretOffsetY = 0);
void render();
position map_convert_screen(position& base, position& origin);

#endif //TANK_BATTLE_GRAP_HPP
