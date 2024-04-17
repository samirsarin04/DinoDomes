#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
    , animationX(366)
    , animationY(26)
    , animationFrameCount(0)
    , animationDimension(350)
    , animationActive(false)
    , showDinoFact(false)
{
    background = background.scaled(1080, 720);

    // Load questions from json question files
    questionsMap[DinosaurName::tRex] = loadQuestions(":/settingsFiles/tRexQuestions.json");
    questionsMap[DinosaurName::brontosaurus] = loadQuestions(":/settingsFiles/brontosaurusQuestions.json");
    questionsMap[DinosaurName::triceratops] = loadQuestions(":/settingsFiles/triceratopsQuestions.json");

    //Coordinates for each bone in the dinosaur;
    //NEEDS TO BE FILLED IN ONCE ART IS COMPLETE
    QMap<DinosaurBone, QPoint> triceratopsCoordinates;
    QMap<DinosaurBone, QPoint> tRexCoordinates;
    QMap<DinosaurBone, QPoint> brontosaurusCoordinates;

    //Triceratops head is the only calibrated value for now
    triceratopsCoordinates[DinosaurBone::head] = QPoint(710, 260);
    triceratopsCoordinates[DinosaurBone::body] = QPoint(650, 260);
    triceratopsCoordinates[DinosaurBone::tail] = QPoint(600, 260);
    triceratopsCoordinates[DinosaurBone::legs] = QPoint(550, 260);

    tRexCoordinates[DinosaurBone::head] = QPoint(150, 260);
    tRexCoordinates[DinosaurBone::body] = QPoint(200, 260);
    tRexCoordinates[DinosaurBone::tail] = QPoint(250, 260);
    tRexCoordinates[DinosaurBone::legs] = QPoint(300, 260);

    brontosaurusCoordinates[DinosaurBone::head] = QPoint(350, 260);
    brontosaurusCoordinates[DinosaurBone::body] = QPoint(400, 260);
    brontosaurusCoordinates[DinosaurBone::tail] = QPoint(450, 260);
    brontosaurusCoordinates[DinosaurBone::legs] = QPoint(500, 260);

    dinosaurCoordinates[DinosaurName::tRex] = tRexCoordinates;
    dinosaurCoordinates[DinosaurName::triceratops] = triceratopsCoordinates;
    dinosaurCoordinates[DinosaurName::brontosaurus] = brontosaurusCoordinates;


    // --------------------- QUESTIONS TEST CASE --------------------- //
    // PRINTS ON START UP (construction of MuseumScene)
    Question q = questionsMap[DinosaurName::brontosaurus][2];
    qDebug() << "Question: " << q.question;
    qDebug() << "Option 0: " << q.options[0];
    qDebug() << "Option 1: " << q.options[1];
    qDebug() << "Option 2: " << q.options[2];
    qDebug() << "Option 3: " << q.options[3];
    qDebug() << "Answer is Option: " << q.correctIndex;
    qDebug() << "Answer: " << q.options[q.correctIndex];
    qDebug() << "Response is: " << q.response << "\n";
}

QVector<MuseumScene::Question> MuseumScene::loadQuestions(QString resourcePath) {
    QVector<MuseumScene::Question> result;
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return result;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileContent);
    if (doc.isNull() || !doc.isObject()) {
        return result;
    }

    try {
        QJsonArray questions = doc.object()["questions"].toArray();
        for (const auto& question : questions) {
            Question q;
            q.question = question.toObject()["question"].toString();
            q.response = question.toObject()["response"].toString();
            q.correctIndex = question.toObject()["correctIndex"].toInt();

            QJsonArray options = question.toObject()["options"].toArray();
            QVector<QString> s;
            for (const auto& option : options)
                s.append(option.toString());
            q.options = s;
            result.append(q);
        }
        return result;
    } catch (...) {
        QVector<MuseumScene::Question> empty;
        return empty;
    }
}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchPtr = &searchScene;
}

QPixmap MuseumScene::buildScene(){
    // Detects if the scene has been switched to
    activate();

    //Does the bone animation if a new bone has been found
    animateBone();

    // process player input
    processPlayerInput();

    drawWorld();

    return frame;
}


void MuseumScene::processPlayerInput(){
    // ignores inputs if the animation is playing
    if (animationActive){
        player->setInput(KeyStroke::none);
        return;
    }

    switch (player->getInput()) {
    case KeyStroke::museumKey:
        switchToSearchScene();
        break;
    case KeyStroke::interactKey:
        closeDinoFact();
        break;
    case KeyStroke::oneKey:
        quizGuess(1);
        break;
    case KeyStroke::twoKey:
        quizGuess(2);
        break;
    case KeyStroke::threeKey:
        quizGuess(3);
        break;
    case KeyStroke::fourKey:
        quizGuess(4);
        break;
    default:
        break;
    }

    player->setInput(KeyStroke::none);

    if(!showDinoFact && player->gameOver){

        qDebug() << "RESET GAME REQUESTED";
        player->setInput(KeyStroke::none);
        return;
    }
}

void MuseumScene::quizGuess(int guess){
    if (!player->boneFound){
        return;
    }

    // SOME LOGIC TO RESPOND TO WHICH INPUT THE USER ENTERED
    switch(guess){
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }

    player->nextBone();

    if (player->isComplete(currentDinosaur)){
        qDebug() << "show final dino fact";
        showDinoFact = true;
    }
}

void MuseumScene::closeDinoFact(){
    qDebug() << "closing dino fact";
    showDinoFact = false;
}

void MuseumScene::animateBone(){

    if (!player->boneFound){
        return;
    }

    // TEMPORARILY DISABLES THE ANIMATION
    animationActive = false;
    return;

    // BOUNCES THE BONE UP AND DOWN AND THEN RETURNS IT TO ITS SPOT

    animationActive = true;

    if(animationFrameCount == 300){
        animationActive = false;
        return;
    }

    if(animationFrameCount == 0){
        player->soundEffects.enqueue(SoundEffect::findBone);
    }

    if (animationFrameCount < 25){
        animationY += 2;
        animationFrameCount++;
        return;
    }

    if (animationFrameCount < 50){
        animationY -= 2;
        animationFrameCount++;
        return;
    }

    if (animationFrameCount < 75){
        animationY += 2;
        animationFrameCount++;
        return;
    }

    if (animationFrameCount < 100){
        animationY -= 2;
        animationFrameCount++;
        return;
    }

    if (abs(animationX - dinosaurCoordinates[player->currentDinosaur][player->currentBone].x()) != 0){
        if (animationX - dinosaurCoordinates[player->currentDinosaur][player->currentBone].x() > 0) {
            animationX -=2;
        } else{
            animationX +=2;
        }
    }

    if (animationY - dinosaurCoordinates[player->currentDinosaur][player->currentBone].y() != 0){
        animationY +=2;
    }

    if (animationDimension != 190){
        animationDimension -=1;
    }

    animationFrameCount++;
}

void MuseumScene::drawWorld(){
    // Build scene
    painter.drawPixmap(0, 0, background);

    //painter.drawPixmap(tRexBaseX, tRexBaseY, player->getSpecificBone(DinosaurName::tRex, DinosaurBone::head));
    //painter.drawPixmap(brontosaurusBaseX, brontosaurusBaseY, player->getSpecificBone(DinosaurName::brontosaurus, DinosaurBone::head));
    //painter.drawPixmap(triceratopsBaseX, triceratopsBaseY, player->getSpecificBone(DinosaurName::triceratops, DinosaurBone::body));

    // ONLY DRAWS THE BONE THAT IS BEING ANIMATED CURRENTLY
    if (player->boneFound){
        painter.drawPixmap(animationX, animationY, player->getCurrentBone().scaled(animationDimension, animationDimension));
    }

    if (player->boneFound && !animationActive){
        // DRAW THE QUIZ SCENE
        qDebug() << "showing the quiz logic";
        return;
    }

    if (showDinoFact){
        // show the final dino fact
        qDebug() << "showing the final dino fact";
        return;
    }

    if(player->gameOver){
        //LOGIC FOR WHAT TO DO IF THE GAME IS OVER
        qDebug() << " THATS A WRAP";
    }

    //qDebug() << "NOT showing the quiz logic";
    //painter.drawPixmap(710, 260, player->getSpecificBone(DinosaurName::triceratops, DinosaurBone::head).scaled(190, 190));


    // QMap<DinosaurBone, QPixmap> foundBones = player->getAllFoundBoneImages(player->currentDinosaur);

    // // TESTING FOR CYCLING DINO PARTS GOING FROM SEARCH->DIG->MUSEUM->SEARCH
    // int count = 0;

    // if(player->boneFound){
    //     //painter.drawPixmap((brontosaurusBaseX + 55 * count), brontosaurusBaseY, player->getCurrentBone().scaled(150,150));
    //     count++;
    // }


    // for(auto i = foundBones.begin(); i != foundBones.end(); i++){
    //     i.value() = i.value().scaled(75, 75);
    //     painter.drawPixmap((brontosaurusBaseX + 55 * count), brontosaurusBaseY, i.value());
    //     //qDebug() << "drawing mini dino";
    //     count++;
    // }


    // for (auto i = player->completeDinosaurs.begin(); i != player->completeDinosaurs.end(); i++){
    //     QMap<DinosaurBone, QPixmap> foundBones1 = player->getAllFoundBoneImages(*i);

    //     count = 0;
    //     for(auto i = foundBones1.begin(); i != foundBones1.end(); i++){
    //         i.value() = i.value().scaled(75, 75);
    //         painter.drawPixmap((tRexBaseX + 55 * count), brontosaurusBaseY + 300, i.value());
    //         //qDebug() << "drawing mini dino";
    //         count++;
    //     }
    // }

}

void MuseumScene::activate(){
    if (activated){
        return;
    }

    qDebug() << "ACTIVATING SCENE";

    currentDinosaur = player->currentDinosaur;
    animationFrameCount = 0;
    animationX = 366;
    animationY = 26;
    animationDimension = 350;
    activated = true;
    animationActive = false;
    showDinoFact = false;

    if (player->boneFound){
        qDebug() << "LOADING IN THE QUIZ QUESTION";
        // QUEUE UP THE QUIZ LOGIC THAT YOU WILL NEED
    }

}

void MuseumScene::deactivate(){
    activated = false;
}

void MuseumScene::switchToSearchScene(){
    if (player->boneFound || showDinoFact){
        return;
    }

    player->soundEffects.enqueue(SoundEffect::door);
    *currentScene = searchPtr;
    deactivate();
}

