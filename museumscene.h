#ifndef MUSEUMSCENE_H
#define MUSEUMSCENE_H

#include "scene.h"

class MuseumScene : public Scene
{
public:
    explicit MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    virtual QPixmap buildScene();
    void initializePointers(SearchScene &searchScene);
private:
    SearchScene* searchptr;
};

#endif // MUSEUMSCENE_H
