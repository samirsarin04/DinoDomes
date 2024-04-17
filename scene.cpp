#include "scene.h"
#include <Qimage>

Scene::Scene(PlayerState& player, Scene** currentScene, QObject *parent)
    : QObject{parent}
    , player(&player)
    , world(b2Vec2(0, 0))
    , currentScene(currentScene)
    , frame(1080, 720)
    , painter(&frame)
    , timeStep(1.0f / 60.0f)
    , activated(false)
{}

QPixmap Scene::buildScene(){
    return QPixmap();
}

void Scene::activate(){

}

void Scene::deactivate(){

}

void Scene::processPlayerInput(){

}
