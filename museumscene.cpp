#include "museumscene.h"
#include "searchscene.h"

MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/museum_background.png")
    , tRexBaseX(200)
    , tRexBaseY(250)
    , brontosaurusBaseX(400)
    , brontosaurusBaseY(250)
    , triceratopsBaseX(600)
    , triceratopsBaseY(250)
{
    // initialize dinoGuess
    QString tRexArr[4] = {
        QString("tRex"),
        QString("incorrect1"),
        QString("incorrect2"),
        QString("incorrect3")
    };
    QString brontosaurusArr[4] = {
        QString("Brontosaurus"),
        QString("incorrect1"),
        QString("incorrect2"),
        QString("incorrect3")
    };
    QString triceratopsArr[4] = {

    };
    dinoGuess = QMap<DinosaurName, QString*> {
        {DinosaurName::tRex, tRexArr},
        {DinosaurName::brontosaurus, brontosaurusArr},
        {DinosaurName::triceratops, triceratopsArr}
    };

    // initialize quizQuestions
    // quizQuestions = QMap<QString, QString*> {
    //     {/*Fact, {Question, Answer, Incorrect1, Incorrect2, Incorrect3}*/},
    // };

    background = background.scaled(1080, 720);    
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
        // openGuess();
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    // Build scene
    painter.drawPixmap(0, 0, background);
    painter.drawPixmap(tRexBaseX, tRexBaseY, player->getSpecificBone(DinosaurName::tRex, DinosaurBone::head));
    painter.drawPixmap(brontosaurusBaseX, brontosaurusBaseY, player->getSpecificBone(DinosaurName::brontosaurus, DinosaurBone::head));
    painter.drawPixmap(triceratopsBaseX, triceratopsBaseY, player->getSpecificBone(DinosaurName::triceratops, DinosaurBone::head));

    return frame;
}

// SHOULD BE OWN CLASS?? GUESSFRAME CLASS?? ATLEAST GIVES STRUCTURE FOR SUCH A CLASSES CONSTRUCTOR
/// @brief This method will ask to the user this question
/// @param question The string containing the question
/// @param answer the string containing the answer
/// @param incOpts the other incorrect answers
void MuseumScene::openGuess(QString question, QString answer, QString* incOpts) {

}



