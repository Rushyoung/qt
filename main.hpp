//
// Created by rushy on 24-5-27.
//
#include <QDebug>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>
#include "main.moc"
#include "include/base.hpp"
#include "src/grap.hpp"
#ifndef QT_MAIN_HPP
#define QT_MAIN_HPP

extern Tank_info local;
extern int remote_amount;
extern tank_data* churchill_data;
extern tank_data* tiger_data;
extern tank_data* is2_data;
extern tank_data* t34_85_data;
extern tank_data* sherman_data;
extern tank_draw_data* draw_data;
extern tank_draw_data* draw_data_ai;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr): QMainWindow(parent)
    {
        scene = new QGraphicsScene(this);
        view = new QGraphicsView(scene, this);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // 设置视图的大小和位置
        view->setGeometry(0, 0, SCREEN_LENGTH, SCREEN_WIDTH);

        // 将视图添加到主窗口中
        this->setCentralWidget(view);
    }
    void draw_tank(tank_draw_data* draw_buffer, double head_degree, double turret_degree, int center_x, int center_y, int turretOffsetX, int turretOffsetY = 0){
        // 计算新的偏移量
        int newOffsetX = turretOffsetX * cos(qDegreesToRadians(head_degree)) - turretOffsetY * sin(qDegreesToRadians(head_degree));
        int newOffsetY = turretOffsetX * sin(qDegreesToRadians(head_degree)) + turretOffsetY * cos(qDegreesToRadians(head_degree));

        // 设置坦克车身的转换
        QTransform bodyTransform;
        bodyTransform.translate(center_x, center_y);
        bodyTransform.rotate(-head_degree);
        draw_buffer->body_item->setTransform(bodyTransform);

        // 设置炮塔的转换
        QTransform turretTransform;
        turretTransform.translate(center_x + newOffsetX, center_y + newOffsetY);
        turretTransform.rotate(-turret_degree);
        draw_buffer->turret_item->setTransform(turretTransform);
    }
    void addTank(tank_draw_data* drawData) {
        scene->addItem(drawData->body_item);
        scene->addItem(drawData->turret_item);
    }
    ~MainWindow(){
        delete scene;
        delete view;
    }
    QGraphicsScene* getScene(){return scene;}
public slots:
    static void updateGui(QGraphicsItem* item, double x, double y, double angle) {
        // 在这里更新你的 item 的位置和角度
        if (item) {
            item->setPos(x, y);
            item->setTransformOriginPoint(item->boundingRect().width() / 2, item->boundingRect().height() / 2);
            item->setRotation(angle);
        }
    }
    void updateAllGui() {
        // 在这里更新视图
        view->update();
    }

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
};

extern MainWindow* _window;

class Renderer : public QObject {
Q_OBJECT
public:
    Renderer() {
        QPixmap map_pix("../source/map1.png");
        map_item = new QGraphicsPixmapItem(map_pix);
        _window->getScene()->addItem(map_item);
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Renderer::render);
        timer->start(33);
    }

    ~Renderer() {
        delete map_item;
        delete timer;
    }
signals:
    void updateNeeded(QGraphicsItem* item, double x, double y, double angle);
    void updateAllGui();

public slots:

    void render() {
        // while (true) {
            tank = chan<Tank_info>("local").receive();
            tank_ai = chan<Tank_info>("ai2").receive();
            local = tank;
            if (tank_map.find(tank.id) == tank_map.end()) {
                draw_data = chan<tank_draw_data*>("local").receive_safe();
                draw_data_ai = chan<tank_draw_data*>("ai2").receive_safe();
                tank_map[tank.id] = draw_data;
            } else {
                draw_data = tank_map[tank.id];
            }

            position temp(0, 0);
            temp = position(SCREEN_LENGTH/2, SCREEN_WIDTH/2) - tank.pos;
            map_item->setPos(temp.x, temp.y);
            // 发送信号
            std::cerr << "local " << map_convert_screen(tank.pos, tank.pos).x << std::endl;
            emit updateNeeded(draw_data->body_item, map_convert_screen(tank.pos, tank.pos).x, map_convert_screen(tank.pos, tank.pos).y, tank.head_degree);
            emit updateNeeded(draw_data->turret_item, map_convert_screen(tank.pos, tank.pos).x + draw_data->offset * cos(
                    Radians(tank.head_degree)), map_convert_screen(tank.pos, tank.pos).y + draw_data->offset * sin(
                    Radians(tank.head_degree)), tank.turret_degree);
            std::cerr << "ai " << map_convert_screen(tank_ai.pos, tank.pos).x << std::endl;
            emit updateNeeded(draw_data_ai->body_item, map_convert_screen(tank_ai.pos, tank.pos).x, map_convert_screen(tank_ai.pos, tank.pos).y, tank_ai.head_degree);
            emit updateNeeded(draw_data_ai->turret_item, map_convert_screen(tank_ai.pos, tank.pos).x + draw_data_ai->offset * cos(
                    Radians(tank_ai.head_degree)), map_convert_screen(tank_ai.pos, tank.pos).y + draw_data_ai->offset * sin(
                    Radians(tank_ai.head_degree)), tank_ai.turret_degree);
        // }
    }

private:
    Tank_info tank;
    Tank_info tank_ai;
    QGraphicsPixmapItem* map_item;
    QTimer* timer;
    std::map<int, tank_draw_data*> tank_map;
};


// class Renderer : public QObject {
// Q_OBJECT
// public:
//     Renderer() {
//         QPixmap map_pix("../source/map1.png");
//         map_item = new QGraphicsPixmapItem(map_pix);
//         _window->getScene()->addItem(map_item);
//         timer = new QTimer(this);
//         connect(timer, &QTimer::timeout, this, &Renderer::render);
//         timer->start(33);
//     }

//     ~Renderer() {
//         delete map_item;
//         delete timer;
//     }

// public slots:
//     void render() {
//         local = chan<Tank_info>("local").receive();
//         tank_draw_data* local_draw = chan<tank_draw_data*> ("local").receive_safe();
//         position temp(0, 0);
//         temp = position(SCREEN_LENGTH/2, SCREEN_WIDTH/2) - local.pos;
//         map_item->setPos(temp.x, temp.y);
//         // 发送信号
//         emit updateNeeded(local_draw->body_item, map_convert_screen(local.pos, local.pos).x, map_convert_screen(local.pos, local.pos).y, local.head_degree);
//         emit updateNeeded(local_draw->body_item, map_convert_screen(local.pos, local.pos).x + local_draw->offset * cos(
//                 Radians(local.head_degree)), map_convert_screen(local.pos, local.pos).y + local_draw->offset * sin(
//                 Radians(local.head_degree)), local.turret_degree);
//     }

// private:
//     QGraphicsPixmapItem* map_item;
//     QTimer* timer;
// };
#endif //QT_MAIN_HPP
