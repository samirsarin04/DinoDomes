#ifndef DIGSCENE_H
#define DIGSCENE_H

#include "scene.h"

class DigScene : public Scene
{
public:
    explicit DigScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    void initializePointers(SearchScene &searchScene, MuseumScene &museumScene);
    virtual QPixmap buildScene();
private:
    SearchScene* searchPtr;
    MuseumScene* museumPtr;
    QMap<DinosaurBone, QPixmap> tRexFacts;
    //keeps track of how long the brush animation has lasted and which frame it is on. Set to -1 if animation is not currently playing
    int animationFrame;
    int brushPosition;
    void displayBone(DinosaurBone bone);
};

#endif // DIGSCENE_H
