#include "scene.h"
#include <Qimage>

Scene::Scene(PlayerState& player, QObject *parent)
    : player(&player), QObject{parent}
{}

void Scene::keyPress(KeyStroke key){}

QPixmap Scene::buildScene(){}
