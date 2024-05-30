//
// Created by 小小喵姬 on 24-5-13.
//

#include "../include/base.hpp"
#include "grap.hpp"
//#include <iostream>

#define ROTATE_SPEED PI/32
#define FOR_STRAIGHT_CRITICAL_VALUE 6.0
#define FOR_TURN_CRITICAL_VALUE 8.0
#define FIRST_DECISION 5
int fire_flag = 0;
extern std::vector<std::shared_ptr<Bullet>> bullets;

int whether_first=0;//0为第一次，1为后续

int for_condition=0;//0为前进，1为转弯

int probability_judge(int for_con){
    //初次时判断AI前进还是转弯
    if(whether_first==0){
        int random_number = random(1,10);
        if(random_number<=FIRST_DECISION){
            whether_first=1;
            for_condition=0;
            return probability_judge(for_condition);
        }else{
            whether_first=1;
            for_condition=1;
            return probability_judge(for_condition);
        }
    }
    //后续判断
    else{
        for_con=for_condition;
        int random_number = random(1,10);
        if(for_con==0){
            if(random_number<=FOR_STRAIGHT_CRITICAL_VALUE){
                for_condition=0;
                return 0;//返回值0为前进，1为左转，2为右转
            }
            else if(random_number<=FOR_STRAIGHT_CRITICAL_VALUE/2+5){
                for_condition=1;
                return 1;
            }
            else{
                for_condition=1;
                return 2;
            }
        }
        else{
            if(random_number<=FOR_TURN_CRITICAL_VALUE){
                for_condition=0;
                return 0;//返回值0为前进，1为左转，2为右转
            }
            else if(random_number<=FOR_TURN_CRITICAL_VALUE/2+5){
                for_condition=1;
                return 1;
            }
            else{
                for_condition=1;
                return 2;
            }
        }
    }
}



void Tank_local::control() {

    chan<Tank_info>("local").send(Tank_info(id, pos, head_degree, turret_degree, true));
    chan<tank_draw_data*> ("local").send(draw);
    while(true){
        std::cerr << "local alive" << std::endl;
        if(!enable){chan<Tank_info>("local").send(Tank_info(id, pos, head_degree, turret_degree, true));return;}//destruct & broken
        bool changed;
        changed = false;
//        _mouse = GetMouseMsg();
//        switch(_mouse.uMsg){
//            case WM_MOUSEMOVE:
////                std::cerr << "mouse" << _mouse.x << std::endl;
//                turret_degree = atan2(_mouse.y - pos.y, _mouse.x - pos.x);
//                changed = true;
//                //wait for adjust
//            case WM_LBUTTONDOWN:
//                turret_degree = atan2(_mouse.y - pos.y, _mouse.x - pos.x);
//                changed = true;
//                //wait for fire & adjust
//        }
        //move forward
        if(GetAsyncKeyState('W')&0x8000){
//            std::cerr << "up" << std::endl;
            double radian_head= Radians(head_degree);
            if (!(pos.x + speed * cos(radian_head) >= MAP_X ||
                pos.x + speed * cos(radian_head) <= 0 ||
                pos.y + speed * sin(radian_head) >= MAP_Y ||
                pos.y + speed * sin(radian_head) <= 0)){
                pos.x += speed * cos(radian_head);
                pos.y += speed * sin(radian_head);
//                cout << "local:" << pos.x ;
//                cout << " local:" << pos.y << std::endl;
                changed = true;
            }
        }
        //move backward
        if(GetAsyncKeyState('S')&0x8000){
//            std::cerr << "down" << std::endl;
            double radian_head= Radians(head_degree);
            if (!(pos.x - speed * cos(radian_head) >= MAP_X ||
                pos.x - speed * cos(radian_head) <= 0 ||
                pos.y - speed * sin(radian_head) >= MAP_Y ||
                pos.y - speed * sin(radian_head) <= 0)){
                pos.x -= speed * cos(radian_head);
                pos.y -= speed * sin(radian_head);
                changed = true;
//                cout << "local:" << pos.x ;
//                cout << " local:" << pos.y << std::endl;
            }
        }
        //rotate-
        if(GetAsyncKeyState('A')&0x8000){
//            std::cerr << "turn-" << std::endl;
            head_degree -= Degree(ROTATE_SPEED);
            while (head_degree < 0) {
                head_degree += 360;
            }
            changed = true;
        }
        //rotate+
        if(GetAsyncKeyState('D')&0x8000){
//            std::cerr << "turn+" << std::endl;
            head_degree += Degree(ROTATE_SPEED);
            while (head_degree > 360) {
                head_degree -= 360;
            }
//            cout<< "degreeeeeeeee"<< head_degree<<std::endl;
            changed = true;
        }
//        if(GetAsyncKeyState(VK_RIGHT)&0x8000){
        if(GetAsyncKeyState('K')&0x8000){
//            std::cerr << "turn-" << std::endl;
            turret_degree -= Degree(ROTATE_SPEED);
            while (turret_degree < 0) {
                turret_degree += 360;
            }
            changed = true;
        }
//        if(GetAsyncKeyState(VK_LEFT)&0x8000){
        if(GetAsyncKeyState('J')&0x8000){
//            std::cerr << "turn-" << std::endl;
            turret_degree += Degree(ROTATE_SPEED);
            while (turret_degree < 0) {
                turret_degree += 360;
            }
            changed = true;
        }
        //send
        col.update_pos(pos.x, pos.y);
        if(GetAsyncKeyState(VK_SPACE)&0x8000) {
            if (fire_flag > 0) {
                fire_flag++;
                if (fire_flag > 100) {
                    fire_flag = 0;
                }
            }else {
                fire();
            }
        }
        if(changed){
            chan<Tank_info>("local").send(Tank_info(id, pos, head_degree, turret_degree, true));
        }
//        cout << "x" << pos.x << "y" << pos.y << "degree" << head_degree << std::endl;
        //sleep
        for (auto& bullet : bullets) { // 遍历并更新所有子弹
            if(bullet == NULL){
                continue;
            }
            bullet->move();
/*            if(bullet->co().is_coincide(this->col)){
                this->broken();
                bullet->enable= false;
            }*/
        }

        std::this_thread::sleep_for(millisecond(FRAME_TIME));


        

//        this->fire();
    }
}

void baseTank::broken() {
    enable=false;
}

position Bullet::get_Bullet_pos() {
    return position(origin_pos.x,origin_pos.y);
}

void Bullet::move(){
    if(origin_pos.x + BULLET_SPEED * cos(Radians(degree))>=MAP_X||
            origin_pos.x + BULLET_SPEED * cos(Radians(degree))<=0||
            origin_pos.y + BULLET_SPEED * sin(Radians(degree))>=MAP_Y||
            origin_pos.y + BULLET_SPEED * sin(Radians(degree))<=0)
    {enable=false;}
    else {
        origin_pos.x += BULLET_SPEED * cos(Radians(degree));
        origin_pos.y += BULLET_SPEED * sin(Radians(degree));
        col.update_pos(origin_pos.x, origin_pos.y);
    }
}


void baseTank::fire() {

        //TODO:idk what
        //TODO:time delay

            std::cerr << "fire\n" << std::endl;
            bullets.emplace_back(std::make_shared<Bullet>(this));
//            std::make_shared<Bullet>(this);



}


void Tank_ai::control() {
    std::cerr << "ai";
    chan<tank_draw_data*> ("ai2").send(draw);
    chan<Tank_info>("ai2").send(Tank_info(id, pos, head_degree, turret_degree, true));
    bool changed=false;
    while(true) {

        int move_judge=probability_judge(for_condition);

        //move forward
        if(move_judge==0){
            double radian_head= Radians(head_degree);
            cout<<"radian_head: "<<radian_head<<std::endl;
            if (pos.x + speed * cos((radian_head)) >= MAP_X ||
                pos.x + speed * cos((radian_head)) <= 0 ||
                pos.y + speed * sin((radian_head)) >= MAP_Y ||
                pos.y + speed * sin((radian_head)) <= 0) {
                continue;
            } else {
                pos.x +=5* speed * cos((radian_head));
                pos.y +=5* speed * sin((radian_head));
                cout << "AI:" << pos.x ;
                cout << " AI:" << pos.y << std::endl;
                changed = true;
            }
//            cout<<cos(Radians(radian_head))<<"    "<<sin(Radians(radian_head))<<std::endl;
        }
        //rotate+，顺时针旋转
        else if(move_judge==1){
            head_degree += Degree(ROTATE_SPEED*4);
            while (head_degree >= 360) {
                head_degree -= 360;
            }
            changed = true;
        }
        //rotate-，逆时针旋转
        else{
            head_degree -= Degree(ROTATE_SPEED*4);
            while (head_degree < 0) {
                head_degree += 360;
            }
            changed = true;
        }
         cout<<"degree: "<<head_degree<<std::endl;

        //AI与主机玩家同屏幕的时候，炮塔转向自动攻击
        if(abs((pos-local.pos).x)<=SCREEN_LENGTH/2 && abs((pos-local.pos).y)<=SCREEN_WIDTH/2){
            turret_degree = Degree(atan2(local.pos.y - pos.y, local.pos.x - pos.x));
            //wait for adjust or fire
            changed = true;
        }

        col.update_pos(pos.x, pos.y);

        //send
        if(changed){
            chan<Tank_info>("ai2").send(Tank_info(id, pos, head_degree, turret_degree, true));
        }
        //sleep
        std::this_thread::sleep_for(millisecond(200));

        for (auto& bullet : bullets) { // 遍历并更新所有子弹
            if(bullet->co().is_coincide(this->col)){
                this->broken();
                bullet->enable = false;
                chan<Tank_info>("ai2").send(Tank_info(id, pos, head_degree, turret_degree, false));
                return;
            }
        }
        std::cerr << "aippp" << pos.x << std::endl;


        std::this_thread::sleep_for(millisecond(200));
    }
}
