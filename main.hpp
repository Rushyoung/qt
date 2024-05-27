//
// Created by rushy on 24-5-27.
//
#include "main.moc"
#include "include/base.hpp"
#include "src/grap.hpp"
#ifndef QT_MAIN_HPP
#define QT_MAIN_HPP
class Renderer : public QObject {
Q_OBJECT
private:
    Tank_info local;
public:
    void render();
    // ... 其他代码 ...

signals:
    void updateNeeded(QGraphicsItem* item, double x, double y, double angle);
};



class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr): QMainWindow(parent)
    {
        scene = new QGraphicsScene(this);
        view = new QGraphicsView(scene, this);

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
#endif //QT_MAIN_HPP
