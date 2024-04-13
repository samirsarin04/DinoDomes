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
    void openGuess(QString question, QString answer, QString* incOpts);

    QMap<DinosaurBone, QPixmap> tRexFacts;
    // Dino maps to QString array where index 0 is answer, and 1 - 3 are other options
    QMap<DinosaurName, QString*> dinoGuess;
    // Fact maps to QString array where index 0 is question, 1 is answer, and 2 - 4 are other options
    QMap<QString, QMap<QString, QString*>> quizQuestions;
    SearchScene* searchPtr;
    QPixmap background;
    int tRexBaseX;
    int tRexBaseY;
    int brontosaurusBaseX;
    int brontosaurusBaseY;
    int triceratopsBaseX;
    int triceratopsBaseY;
};

#endif // MUSEUMSCENE_H
