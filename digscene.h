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
};

#endif // DIGSCENE_H
