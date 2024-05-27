#include <QCoreApplication>
#include <QDebug>
#include "main.hpp"
#include <QGuiApplication>
#include <QApplication>
#include "main.moc"



Tank_info local;
int remote_amount = 2;
tank_data* churchill_data;
tank_data* tiger_data;
tank_data* is2_data;
tank_data* t34_85_data;
tank_data* sherman_data;




void Renderer::render() {
    // ... 其他代码 ...
    Tank_info ai[AI_AMOUNT];
    Tank_info remote[REMOTE_MAX];
    for(int i = 0; i < AI_AMOUNT; i++){
        ai[i] = chan<Tank_info>("ai" + std::to_string(i)).receive();
    }
    for(int i = 0; i < remote_amount; i++){
        remote[i] = chan<Tank_info>("remote" + std::to_string(i)).receive();
    }
    local = chan<Tank_info>("local").receive();
    tank_draw_data* local_draw = chan<tank_draw_data*> ("local").receive_safe();
    while(true){
        local = chan<Tank_info>("local").receive();

        // 发送信号
        emit updateNeeded(local_draw->body_item, map_convert_screen(local.pos, local.pos).x, map_convert_screen(local.pos, local.pos).y, local.head_degree);
        emit updateNeeded(local_draw->body_item, map_convert_screen(local.pos, local.pos).x + local_draw->offset * cos(
                Radians(local.head_degree)), map_convert_screen(local.pos, local.pos).y + local_draw->offset * sin(
                Radians(local.head_degree)), local.turret_degree);

        std::this_thread::sleep_for(millisecond(33));
    }
}


MainWindow* _window;

struct Tank_collection {
    tank_data* data;
    baseTank* tank_class;
    tank_draw_data drawData;
    std::thread tank_thread;
    Tank_collection(tank_data* data, baseTank* tank_class)
            : data(data), tank_class(tank_class),
              drawData(data->body, data->turret, data->offset),
              tank_thread(&baseTank::control, this->tank_class)
    {tank_class->update_draw(&drawData);
        _window->addTank(&this->drawData);}
};
Tank_collection* create_tank(tank_type t, int id, Ai_Type ai);

/*class GuiUpdater : public QObject {
    Q_OBJECT
public:
    GuiUpdater(MainWindow* window) : window(window) {}

public slots:
    void updateGui(const Tank_info& local) {
        // 在这里更新 GUI
        QGraphicsScene* Screen = window->getScene();
        // ... 其他代码 ...
    }

private:
    MainWindow* window;
};*/


int main(int argc, char *argv[]) {
    // QCoreApplication a(argc, argv);
    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    MainWindow window;
    _window = &window;
/*    GuiUpdater guiUpdater(&window);
    guiUpdater.moveToThread(app.thread());*/
    Renderer renderer;
    QObject::connect(&renderer, &Renderer::updateNeeded, &window, &MainWindow::updateGui);

    
    tank_data _churchill_data(churchill);
    tank_data _tiger_data(tiger);
    tank_data _is2_data(is2);
    tank_data _t34_85_data(t34_85);
    tank_data _sherman_data(sherman);
    churchill_data = &_churchill_data;
    tiger_data = &_tiger_data;
    is2_data = &_is2_data;
    t34_85_data = &_t34_85_data;
    sherman_data = &_sherman_data;
    std::vector<Tank_collection*> tanks;
    tanks.push_back(create_tank(sherman, 1, Local));

    // std::thread r(&render);
    // r.join();
    return app.exec();
}
Tank_collection* create_tank(tank_type t, int id, Ai_Type ai){
    baseTank* tank_class = nullptr;
    tank_data* data;
    switch (t) {
        case churchill:
            data = churchill_data;
            break;
        case sherman:
            data = sherman_data;
            break;
        case tiger:
            data = tiger_data;
            break;
        case is2:
            data = is2_data;
            break;
        case t34_85:
            data = t34_85_data;
            break;
    }
    switch (ai) {
        case Local:
            tank_class = new Tank_local(random(0, MAP_X), random(0, MAP_Y), data->body_width/2,id, 2, t);
            return new Tank_collection(data, tank_class);
    }
}
