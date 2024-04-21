#ifndef DIGSCENE_H
#define DIGSCENE_H

#include "scene.h"
#include <QFontMetrics>

class DigScene : public Scene
{
public:
    explicit DigScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    void initializePointers(SearchScene &searchScene, MuseumScene &museumScene);
    virtual QPixmap buildScene();
private:
    SearchScene* searchPtr;
    MuseumScene* museumPtr;
    QMap<DinosaurBone, QString> tRexFacts;
    QMap<DinosaurBone, QString> brontosaurusFacts;
    QMap<DinosaurBone, QString> triceratopsFacts;
    QPixmap digPrompt;
    QPixmap goToMuseum;

    //keeps track of how long the brush animation has lasted and which frame it is on. Set to -1 if animation is not currently playing
    int animationFrame;
    int brushPosition;
    bool animationLock = false;
    bool showBone = false;
    QFontMetrics fontMetrics;

    void loadFacts(QString fileName, QMap<DinosaurBone, QString> &result);
    void displayBone(float percentTransparency);
protected:
    virtual void activate();
    virtual void deactivate();
};

#endif // DIGSCENE_H
