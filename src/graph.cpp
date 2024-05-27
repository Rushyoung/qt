//
// Created by rushy on 24-5-24.
//
#include <QtCore>
#include <QtGui/qpainter.h>
#include "grap.hpp"
MainWindow::~MainWindow() {
    delete scene;
    delete view;
}


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // 设置视图的大小和位置
    view->setGeometry(0, 0, SCREEN_LENGTH, SCREEN_WIDTH);

    // 将视图添加到主窗口中
    this->setCentralWidget(view);
}


/*
 * @img:源图像
 * @angle:角度（弧度）
 * @centerX:屏幕坐标系
 * @centerY:屏幕坐标系
 * */
//void rotate_draw(draw_buffer *buffer, double angle, int x, int y) {
//    // 计算图像的中心点
//    int imgCenterX = buffer->dst->getwidth() / 2;
//    int imgCenterY = buffer->dst->getheight() / 2;
//
//    // 设置坐标系的原点为旋转中心
////    setorigin(centerX, centerY);
//    buffer->block->SetPos(x - imgCenterX, y - imgCenterY);
//
////    cout << "center" << x - imgCenterX << std::endl;
//    // 旋转图像和掩码
////    cout << "rotate" << angle - buffer->degree_now << std::endl;
//    buffer->dst->RotateImage_Alpha(angle - buffer->degree_now);
//    buffer->degree_now = angle;
//
//
//    //easyx废案
////    rotateimage(mask, mask, Radians(angle), WHITE);
//
////    // 使用 SRCAND ROP 代码将旋转后的掩码图像绘制到屏幕上
////    putimage(-imgCenterX, -imgCenterY, mask, SRCAND);
////
////    // 使用 SRCPAINT ROP 代码将旋转后的图像绘制到屏幕上
////    putimage(-imgCenterX, -imgCenterY, img, SRCPAINT);
//
//    // 恢复坐标系的原点
////    setorigin(0, 0);
//}

void render(){
    MainWindow window;
    window.show();
    QGraphicsScene* Screen = window.getScene();
    QPixmap map_pix("/source/map1.png");
    QGraphicsPixmapItem map_item(map_pix);
    Screen->addItem(&map_item);
//    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/images/tank.png"));

//    Map_canvas.Load_Image_Alpha(_T("../source/map1.png"));
//    map_block.SetCanvas(&Map_canvas);
//    map_layer.push_back(&map_block);
//    main_scene.push_back(&map_layer);
//    main_scene.push_back(&body_layer);
//    main_scene.push_back(&turret_layer);
//    cout << "construct"<<std::endl;
//    std::vector<Tank_info> ai;
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
    local_draw->scene = Screen;
    Screen->addItem(local_draw->body_item);
    Screen->addItem(local_draw->turret_item);
//    body_layer.push_back(&local_draw->body_block);
//    turret_layer.push_back(&local_draw->turret_block);
    //render begin

    while(true){
        local = chan<Tank_info>("local").receive();
//        cout<<local.pos.y << std::endl;
        position temp(0, 0);
        temp = position(0, 0);
        temp = position(SCREEN_LENGTH/2, SCREEN_WIDTH/2) - local.pos;

//        if (wnd.BeginTask()) {
//
//            draw_tank(local_draw, local.head_degree, local.turret_degree, map_convert_screen(local.pos, local.pos).x,
//                      map_convert_screen(local.pos, local.pos).y,
//                      local_draw->offset, 0);
//
////            draw_tank(local_draw, -i++ * PI / 4, i++ * PI / 4, map_convert_screen(local.pos, local.pos).x,
////                      map_convert_screen(local.pos, local.pos).y,
////                      local_draw->offset, 0);
//
//
////            map_block.SetPos(-2000,-2000);
//            map_block.SetPos(temp.x, temp.y);
//
////            draw_tank(local_draw, PI/2, 0.0, map_convert_screen(local.pos, local.pos).x,
////                      map_convert_screen(local.pos, local.pos).y,
////                      local_draw->offset, 0);
//
//            main_scene.Render(Screen.GetImagePointer());
//            wnd.EndTask();
//            wnd.Redraw();
//        }
//        hiex::DelayFPS(60);
//        std::this_thread::sleep_for(millisecond(FLASH_TIME));
        std::this_thread::sleep_for(millisecond(33));
    }


}


//转换为使用origin为屏幕中心的坐标系
position map_convert_screen(position& base, position& origin){
    position dst(SCREEN_LENGTH/2, SCREEN_WIDTH/2);
    return (dst + (origin - base));
}



void MainWindow::draw_tank(tank_draw_data* draw_buffer, double head_degree, double turret_degree, int center_x, int center_y, int turretOffsetX, int turretOffsetY) {
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
// void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y,  int turretOffsetX, int turretOffsetY) {
//     // 计算新的炮塔偏移量
// //    cout << "ddd" << head_degree << std::endl;
//     int newOffsetX = turretOffsetX * cos(head_degree) - turretOffsetY * sin(head_degree);
// //    int newOffsetY = turretOffsetX * sin(head_degree) + turretOffsetY * cos(head_degree);
//
//     // 将 body 和 turret 绘制到屏幕上
//
//
//
//
//
//     //easyx ruined
//     rotate_draw(&buffer->body_info, head_degree, center_x, center_y);
//     rotate_draw(&buffer->turret_info,  turret_degree, center_x+newOffsetX, center_y);
// }


//void draw_tank(tank_draw_data* buffer, double head_degree, double turret_degree, int center_x, int center_y,  int turretOffsetX, int turretOffsetY) {
//    // 计算新的炮塔偏移量
//    int newOffsetX = turretOffsetX * cos(head_degree) - turretOffsetY * sin(head_degree);
//
//    // 将 body 和 turret 绘制到屏幕上
//    rotate_draw(&buffer->body_info, head_degree, center_x + buffer->body_info.dst->getwidth() / 2, center_y + buffer->body_info.dst->getheight() / 2);
//    rotate_draw(&buffer->turret_info,  turret_degree, center_x + newOffsetX + buffer->turret_info.dst->getwidth() / 2, center_y + buffer->turret_info.dst->getheight() / 2);
//}

//
//void tank_turret(IMAGE* original, IMAGE* body, IMAGE* turret,
//                 position body_pos, position turret_pos,
//                 int bodyWidth, int turretWidth) {
//    int bodyHeight = bodyWidth;
//    int turretHeight = turretWidth;
//    // 从 original 中裁剪出 body 和 turret 的图像
//    SetWorkingImage(original);
//    getimage(body, body_pos.x, body_pos.y, bodyWidth, bodyHeight);
//    getimage(turret, turret_pos.x, turret_pos.y, turretWidth, turretHeight);
//    SetWorkingImage(NULL);
//}


