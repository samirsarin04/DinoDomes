#include "museumscene.h"
#include "searchscene.h"

MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/museum_background.png")
{
    background = background.scaled(1080, 720);
    QPixmap tRexMuseumFact(":/background.png");
}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchPtr = &searchScene;
}

QPixmap MuseumScene::buildScene(){
    // handle input
    switch (player->getInput()) {
    case KeyStroke::museumKey:
        qDebug() << "SWITCHING FROM MUSEUM TO SEARCH";
        *currentScene = searchPtr;
        break;
    case KeyStroke::interactKey:
        qDebug() << "requesting dino info \n TEMP: begin guess";
        openGuess();
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    // Build scene

    // ADD MORE AND PUT IN .H AS INSTANCE
    int tRexX = 200;
    int tRexY = 250;

    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(tRexX, tRexY, player->getSpecificBone(DinosaurName::tRex, DinosaurBone::head));

    return frame;
}

void MuseumScene::openGuess() {

}



