//
// Created by 小小喵姬 on 24-5-13.
//
#define MAP_X 2700
#define MAP_Y 2500

#define SCREEN_WIDTH 1080
#define SCREEN_LENGTH 1920

#include "basetype.hpp"
#include "chanel.hpp"
#include "net.hpp"
#ifndef TANK_BATTLE_BASE_HPP
#define TANK_BATTLE_BASE_HPP
#define AI_AMOUNT 20
#define REMOTE_MAX 2
#define BULLET_SPEED 5
#define BULLET_RADIUS 4


//extern int ai_amount;
extern bool is_host;
extern int remote_amount;
extern QPixmap* _bullet_draw;
extern int bullet_id;
//extern MainWindow* _window;

enum Ai_Type{
    Local,
    Ai,
    Remote
};

class Collision{
public:
    Collision(int x, int y, int length, double angle = 0): pos(x, y), length(length), angle(angle){}
    double edge(double _angele){
        if(type == 'C'){
            return length;
        }
        else if(type == 'S'){
            return width/(2*sin(_angele+angle));
        }
        return length;
    }
    void update_pos(int x, int y){
        pos.x = x;
        pos.y = y;
    }
    /*
     * @return:碰撞1，没碰0
     * */
    bool is_coincide(Collision other){
        double center_distance = distance(other.pos, pos);
        double accor_angle;
        if((pos.x-other.pos.x)!=0) {
            accor_angle = atan((pos.y - other.pos.y) / (pos.x - other.pos.x));//炮弹和坦克身体的相对角度
        }
        else{
            accor_angle = PI/2;
        }
        double real_distance = center_distance - other.edge (accor_angle) - edge(accor_angle);//temp
        return real_distance < 0;
    }
    int getLength(){return length;}
private:
    //时间缘故，先只做圆形碰撞箱
    struct position pos;
    double angle;//含义需明确，为坦克头的朝向
    char type;//'C'-circle  'S'-square
    int length;
    int width;
    double rad;
};

class baseTank{
public:
    /*
     * @x:逻辑x坐标（地图）
     * @y:逻辑y坐标（地图）
     * @length:圆形碰撞箱半径
     * @body_image:tank的模型图片
     * */
    baseTank(int x, int y, int length, int id, double speed, Ai_Type t, tank_type mo):pos(x, y), col(x, y, length), id(id), speed(speed), type(t), model(mo) { head_degree = 0; enable = true; turret_degree = 0;}
    virtual void control() = 0;
    void update_draw(tank_draw_data* d){
        draw = d;
    }
    int getX(){return pos.x;}
    int getY(){return pos.y;}
    int getLength(){return col.getLength();}
    struct position getPos(){return pos;}
    double getDegree(){return head_degree;}
    double getTurrent_degree(){return turret_degree;}
    void fire();
    void broken();
protected:
    tank_draw_data* draw;
    int id;
    enum Ai_Type type;
    enum tank_type model;
    struct position pos;
    Collision col;
    double head_degree;
    double turret_degree;
    double speed;
    bool enable;

};

class Tank_local : public baseTank{
public:
    Tank_local(int x, int y, int length, int id, double speed, tank_type t): baseTank(x, y, length, id, speed, Local, t), speed(speed){chan<Tank_info>("local").send(Tank_info(id, pos, head_degree, turret_degree, true));}
    void control() override;
protected:
    double speed;
};

class Tank_ai : public baseTank{
public:
    Tank_ai(int x, int y, int length, int id, double speed, tank_type t): baseTank(x, y, length, id, speed, Ai, t){}
    void control() override;
};

class Tank_remote : public baseTank{
public:
    void control() override;
};

class Bullet {
public:
    int id;
    bool enable;
    QGraphicsPixmapItem bullet_item;
    Bullet(baseTank* tank): origin_pos(tank->getPos()), degree(tank->getDegree()),
                                          col(tank->getX() + tank->getLength() * cos(Radians(tank->getTurrent_degree())),
        tank->getY() + tank->getLength() * sin(Radians(tank->getTurrent_degree())),
        BULLET_LENGTH,
        tank->getTurrent_degree()),
                                          bullet_item(*_bullet_draw),
                                          enable(true)
        {fire_timestamp = unix_time_stamp();id = bullet_id++;Addbullet();}
    position get_Bullet_pos();
    void move();
    void Addbullet();
    Collision co(){position temp = get_Bullet_pos();return Collision(temp.x, temp.y, BULLET_LENGTH);}
private:
    struct position origin_pos;
    double degree;
    Collision col;
    long long fire_timestamp;
    enum  Ai_Type type;
//    Collision collision;
};



#endif //TANK_BATTLE_BASE_HPP
