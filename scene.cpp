#include "scene.h"

Scene::Scene(QObject *parent)
    : QObject{parent}, player(player)
{}

void Scene::keyPress(KeyStroke key){}

QImage Scene::buildScene(){}
