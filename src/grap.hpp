//
// Created by rushy on 24-5-24.
//
#include "../include/base.hpp"
#include "../include/basetype.hpp"
#include "../include/chanel.hpp"
#include "D:/code/qt/cmake-build-debug/qt_autogen/include/main.moc"
#include <QCoreApplication>
//#include <graphics.h>

#include <vector>
#include <QtWidgets/qmainwindow.h>

#ifndef TANK_BATTLE_GRAP_HPP
#define TANK_BATTLE_GRAP_HPP
extern Tank_info local;


class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void draw_tank(tank_draw_data* draw_buffer, double head_degree, double turret_degree, int center_x, int center_y, int turretOffsetX, int turretOffsetY = 0);
    ~MainWindow() override;
    QGraphicsScene* getScene(){return scene;}

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
};

//void rotate_draw(draw_buffer *buffer, double angle, int x, int y);

//void tank_turret(IMAGE* original, IMAGE* body, IMAGE* turret,
//                 position body_pos, position turret_pos,
//                 int bodyWidth, int turretWidth);
void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y,  int turretOffsetX, int turretOffsetY);

//void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y, int body_width,
//          int turret_width, int turretOffsetX, int turretOffsetY = 0);
void render();
position map_convert_screen(position& base, position& origin);

#endif //TANK_BATTLE_GRAP_HPP
