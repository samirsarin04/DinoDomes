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
        QString("Triceratops"),
        QString("incorrect1"),
        QString("incorrect2"),
        QString("incorrect3")
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
    case KeyStroke::oneKey:
        qDebug() << "one press";
        break;
    case KeyStroke::twoKey:
        qDebug() << "two press";
        break;
    case KeyStroke::threeKey:
        qDebug() << "three press";
        break;
    case KeyStroke::fourKey:
        qDebug() << "four press";
        break;
    default:
        break;
    }
    player->setInput(KeyStroke::none);

    if (player->boneFound){
        player->nextBone();
    }

    // Build scene
    painter.drawPixmap(0, 0, background);

    QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);


    // TESTING FOR CYCLING DINO PARTS GOING FROM SEARCH->DIG->MUSEUM->SEARCH
    int count = 0;
    for(auto i = foundBones.begin(); i != foundBones.end(); i++){
        i.value() = i.value().scaled(75, 75);
        painter.drawPixmap((brontosaurusBaseX + 55 * count), brontosaurusBaseY, i.value());
        //qDebug() << "drawing mini dino";
        count++;
    }


    for (auto i = player->completeDinosaurs.begin(); i != player->completeDinosaurs.end(); i++){
        QMap<DinosaurBone, QPixmap> foundBones1 = player->getAllFoundBoneImages(*i);

    count = 0;
    for(auto i = foundBones1.begin(); i != foundBones1.end(); i++){
        i.value() = i.value().scaled(75, 75);
        painter.drawPixmap((tRexBaseX + 55 * count), brontosaurusBaseY + 300, i.value());
        //qDebug() << "drawing mini dino";
        count++;
    }
    }

    //painter.drawPixmap(tRexBaseX, tRexY, player->getSpecificBone(DinosaurName::tRex, DinosaurBone::head));
    //painter.drawPixmap(brontosaurusBaseX, brontosaurusBaseY, player->getSpecificBone(DinosaurName::brontosaurus, DinosaurBone::head));
    //painter.drawPixmap(triceratopsBaseX, triceratopsBaseY, player->getSpecificBone(DinosaurName::triceratops, DinosaurBone::head));

    return frame;
}

// SHOULD BE OWN CLASS?? GUESSFRAME CLASS?? ATLEAST GIVES STRUCTURE FOR SUCH A CLASSES CONSTRUCTOR
/// @brief This method will ask to the user this question
/// @param question The string containing the question
/// @param answer the string containing the answer
/// @param incOpts the other incorrect answers
void MuseumScene::openGuess(QString question, QString answer, QString* incOpts) {

}



