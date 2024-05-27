#include <QCoreApplication>
#include <QDebug>

#include "include/base.hpp"

tank_data churchill_data(churchill);
tank_data tiger_data(tiger);
tank_data is2_data(is2);
tank_data t34_85_data(t34_85);
tank_data sherman_data(sherman);




struct Tank_collection {
    tank_data* data;
    baseTank* tank_class;
    tank_draw_data drawData;
    std::thread tank_thread;
    Tank_collection(tank_data* data, baseTank* tank_class)
            : data(data), tank_class(tank_class),
              drawData(&data->body, &data->turret, &body_layer, &turret_layer, data->offset),
              tank_thread(&baseTank::control, this->tank_class)
    {tank_class->update_draw(&drawData);}
};

Tank_collection* create_tank(tank_type t, int id, Ai_Type ai){
    baseTank* tank_class = nullptr;
    tank_data* data;
    switch (t) {
        case churchill:
            data = &churchill_data;
            break;
        case sherman:
            data = &sherman_data;
            break;
        case tiger:
            data = &tiger_data;
            break;
        case is2:
            data = &is2_data;
            break;
        case t34_85:
            data = &t34_85_data;
            break;
    }
    switch (ai) {
        case Local:
            tank_class = new Tank_local(random(0, MAP_X), random(0, MAP_Y), data->body_width/2,id, 2, t);
            return new Tank_collection(data, tank_class);
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "Hello World";
    return QCoreApplication::exec();
}
