#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "museumscene.h"
#include "searchscene.h"

MuseumScene::MuseumScene(PlayerState& player, Scene** currentScene, QObject *parent)
    : Scene{player, currentScene, parent}
    , background(":/images/museum_background.png")
    , triceratopsSilhouette(":/images/triceratopsSilhouette.png")
    , tRexSilhouette(":/images/tRexSilhouette.png")
    , brontosaurusSilhouette(":/images/brontosaurusSilhouette.png")
    , quizBackground(":/images/quizBackground.png")
    , pressAnyKey(":/images/pressAnyKey.png")
    , pressF(":/images/pressF.png")
    , pressNumbers(":/images/press14.png")
    , youWin(":/images/youWin.png")
    , body("Copperplate Gothic Bold", 20)
    , title("Copperplate Gothic Bold",25)
    , credits("Copperplate Gothic Bold", 35)
    , factFont("Arial Black", 14)
    , animationX(380)
    , animationY(12)
    , animationFrameCount(0)
    , animationDimension(275)
    , animationActive(false)
    , showDinoFact(false)
    , closeQuiz(false)
    , startCredits(false)
    , winSoundPlayed(false)
    , playerAnswered(-1)
    , quizNumber(2)
{
    background = background.scaled(1080, 720);

    // Load questions from json question files
    questionsMap[DinosaurName::tRex] = loadQuestions(":/settingsFiles/json/tRexQuestions.json", DinosaurName::tRex);
    questionsMap[DinosaurName::brontosaurus] = loadQuestions(":/settingsFiles/json/brontosaurusQuestions.json", DinosaurName::brontosaurus);
    questionsMap[DinosaurName::triceratops] = loadQuestions(":/settingsFiles/json/triceratopsQuestions.json", DinosaurName::triceratops);

    dinosaurCoordinates[DinosaurName::tRex] = QPoint(452, 320);
    dinosaurCoordinates[DinosaurName::triceratops] = QPoint(700, 320);
    dinosaurCoordinates[DinosaurName::brontosaurus] = QPoint(220, 320);

    dinosaurBaseCoordinates[DinosaurName::tRex] = QPoint(410, 275);
    dinosaurBaseCoordinates[DinosaurName::triceratops] = QPoint(625, 275);
    dinosaurBaseCoordinates[DinosaurName::brontosaurus] = QPoint(105, 195);

    factsHeader[DinosaurName::tRex] = "Tyrannosaurus Rex";
    factsHeader[DinosaurName::triceratops] = "Triceratops";
    factsHeader[DinosaurName::brontosaurus] = "Brontosaurus";
}

void MuseumScene::initializePointers(SearchScene &searchScene){
    searchPtr = &searchScene;
}

QPixmap MuseumScene::buildScene(){
    // Detects if the scene has been switched to
    activate();

    // process player input
    processPlayerInput();

    // Draws all of the elements in the world
    drawWorld();

    return frame;
}

void MuseumScene::processPlayerInput(){
    // ignores inputs if the animation is playing

    if (animationActive){
        player->setInput(KeyStroke::none);
        return;
    }

    if (player->gameOver && !showDinoFact && player->getInput() != KeyStroke::none){
        startCredits = true;
        player->setInput(KeyStroke::none);
        return;
    }

    // Allows any key press to return player to search scene
    if (!player->boneFound && player->getInput() != KeyStroke::none && !showDinoFact && player->getInput() != KeyStroke::moveLeftKey){
        switchToSearchScene();
        player->setInput(KeyStroke::none);
        return;
    }

    switch (player->getInput()) {
    case KeyStroke::interactKey:
        closeDinoFact();
        break;
    case KeyStroke::oneKey:
        quizGuess(0);
        break;
    case KeyStroke::twoKey:        
        quizGuess(1);
        break;
    case KeyStroke::threeKey:
        quizGuess(2);
        break;
    case KeyStroke::fourKey:
        quizGuess(3);
        break;
    default:
        break;
    }

    player->setInput(KeyStroke::none);
}

void MuseumScene::quizGuess(int guess){
    if (!player->boneFound){
        return;
    }

    if(playerAnswered != -1){
        return;
    }


    if (guess == currentQuestion.correctIndex){
        player->soundEffects.enqueue(SoundEffect::correct);
    } else {
        player->soundEffects.enqueue(SoundEffect::wrong);
    }

    playerAnswered = guess;
    closeQuiz = true;
}

void MuseumScene::closeDinoFact(){
    if (closeQuiz){
        player->nextBone();
        closeQuiz = false;
        if (player->isComplete(currentDinosaur)){
            showDinoFact = true;
            player->soundEffects.enqueue(SoundEffect::pageTurn);
        }
        return;
    }
    showDinoFact = false;
}

void MuseumScene::animateBoneLogic(){

    if (!player->boneFound){
        return;
    }

    // TEMPORARILY DISABLES THE ANIMATION
    // animationActive = false;
    // return;

    // BOUNCES THE BONE UP AND DOWN AND THEN RETURNS IT TO ITS SPOT

    animationActive = true;

    if(animationFrameCount == 450){
        animationActive = false;
        return;
    }

    if (animationFrameCount == 445){
        player->soundEffects.enqueue(SoundEffect::pageTurn);
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

    if (abs(animationX - dinosaurCoordinates[player->currentDinosaur].x()) != 0){
        if (animationX - dinosaurCoordinates[player->currentDinosaur].x() > 0) {
            animationX -=4;
        } else{
            animationX +=4;
        }
    }

    if (animationY - dinosaurCoordinates[player->currentDinosaur].y() != 0){
        animationY +=4;
    }

    if (animationDimension != 190){
        animationDimension -=1;
    }

    animationFrameCount++;
}

void MuseumScene::drawBoneAnimation(){
    animateBoneLogic();

    if (player->boneFound && animationFrameCount < 185){
        painter.drawPixmap(animationX, animationY, player->getDigBone().scaled(animationDimension, animationDimension, Qt::KeepAspectRatio));
    }

    if (player->boneFound && animationFrameCount >= 185){

        QPixmap bone = player->getCurrentBone();

        QPixmap img(bone.size());
        img.fill(Qt::transparent);
        //  https://www.qtcentre.org/threads/51158-setting-QPixmap-s-alpha-channel
        QPainter p(&img);

        p.setOpacity((double)(animationFrameCount - 185) / 200);
        p.drawPixmap(0, 0, bone);

        painter.drawPixmap(dinosaurBaseCoordinates[currentDinosaur].x(), dinosaurBaseCoordinates[currentDinosaur].y(), img);
    }
}

void MuseumScene::drawWorld(){
    drawBackgroundAndFoundDinos();
    drawBoneAnimation();
    drawQuiz();
    drawFinalDinoFact();
    drawCredits();
}

void MuseumScene::drawBackgroundAndFoundDinos(){
    // Build scene
    painter.drawPixmap(0, 0, background);

    painter.drawPixmap(dinosaurBaseCoordinates[DinosaurName::tRex].x(), dinosaurBaseCoordinates[DinosaurName::tRex].y(), tRexSilhouette.scaled(300, 300));
    painter.drawPixmap(dinosaurBaseCoordinates[DinosaurName::brontosaurus].x(), dinosaurBaseCoordinates[DinosaurName::brontosaurus].y(), brontosaurusSilhouette.scaled(400, 400));
    painter.drawPixmap(dinosaurBaseCoordinates[DinosaurName::triceratops].x(), dinosaurBaseCoordinates[DinosaurName::triceratops].y(), triceratopsSilhouette.scaled(300, 300));

    if(player->gameOver && !showDinoFact){
        painter.drawPixmap(240, 75, youWin);
        if (!winSoundPlayed){
            player->soundEffects.enqueue(SoundEffect::victory);
            winSoundPlayed = true;
        }
    }


    QMap<DinosaurBone, QPixmap> currentBones = player->getAllFoundBoneImages(currentDinosaur);

    // Draws the found bones of the current dinosaur
    for(auto bone = currentBones.begin(); bone != currentBones.end(); bone++){
        painter.drawPixmap(dinosaurBaseCoordinates[currentDinosaur].x(), dinosaurBaseCoordinates[currentDinosaur].y(), bone.value());
    }
    // Draws the completed dinosaurs
    for (auto dino = player->completeDinosaurs.begin(); dino != player->completeDinosaurs.end(); dino++){
        QMap<DinosaurBone, QPixmap> completeBones = player->getAllFoundBoneImages(*dino);
        for(auto bone = completeBones.begin(); bone != completeBones.end(); bone++){
            painter.drawPixmap(dinosaurBaseCoordinates[*dino].x(), dinosaurBaseCoordinates[*dino].y(), bone.value());
        }
    }

    if (!player->boneFound && !showDinoFact && !animationActive && !startCredits){
        painter.drawPixmap(340, 657, pressAnyKey);
    }

}

void MuseumScene::drawQuiz(){
    if (player->boneFound && !animationActive){
        painter.drawPixmap(150, 100, quizBackground.scaled(810,540));

        QRect incorrect(160, 230, 790, 100);
        // painter.fillRect(incorrect, Qt::red);
        // painter.drawRect(incorrect);

        QRect correct(160,230, 790, 100);
        // painter.fillRect(correct,Qt::green);
        // painter.drawRect(correct);

        // SOME LOGIC TO RESPOND TO WHICH INPUT THE USER ENTERED
        switch(playerAnswered){
        case 0:
            incorrect.moveTo(160,230);
            painter.fillRect(incorrect,Qt::red);
            break;
        case 1:
            incorrect.moveTo(160,330);
            correct.moveTo(160,330);
            painter.fillRect(incorrect,Qt::red);
            break;
        case 2:
            incorrect.moveTo(160,430);
            correct.moveTo(160,430);
            painter.fillRect(incorrect,Qt::red);
            break;
        case 3:
            incorrect.moveTo(160,530);
            correct.moveTo(160,530);
            painter.fillRect(incorrect,Qt::red);
            break;
        default:
            break;
        }

        if(playerAnswered != -1){
            switch(currentQuestion.correctIndex){
            case 0:
                correct.moveTo(160,230);
                painter.fillRect(correct,Qt::green);
                break;
            case 1:
                correct.moveTo(160,330);
                painter.fillRect(correct,Qt::green);
                break;
            case 2:
                correct.moveTo(160,430);
                painter.fillRect(correct,Qt::green);
                break;
            case 3:
                correct.moveTo(160,530);
                painter.fillRect(correct,Qt::green);
                break;
            default:
                break;
            }
        }

        //painter.drawPixmap(150, 100, quizBackground.scaled(810,540));


        //Quiz questions
        painter.setFont(title);

        QRect titleBox(160, 110, 790, 100);
        painter.fillRect(titleBox,Qt::transparent);
        painter.drawText(titleBox, Qt::TextWordWrap, currentQuestion.question);

        QRect q1Box(160, 230, 790, 100);
        painter.fillRect(q1Box,Qt::transparent);

        QRect q2Box(160, 330, 790, 100);
        painter.fillRect(q2Box,Qt::transparent);

        QRect q3Box(160, 430, 790, 100);
        painter.fillRect(q3Box,Qt::transparent);

        QRect q4Box(160, 530, 790, 100);
        painter.fillRect(q4Box,Qt::transparent);



        painter.setFont(body);
        painter.drawText(q1Box, Qt::TextWordWrap, currentQuestion.options[0]);
        painter.drawText(q2Box, Qt::TextWordWrap,currentQuestion.options[1]);
        painter.drawText(q3Box, Qt::TextWordWrap,currentQuestion.options[2]);
        painter.drawText(q4Box, Qt::TextWordWrap,currentQuestion.options[3]);

        if (playerAnswered != -1){
            painter.drawPixmap(340, 657, pressF);
        } else {
            painter.drawPixmap(340, 657, pressNumbers);
        }
    }
}

void MuseumScene::drawFinalDinoFact(){
    if (showDinoFact){

        // show the final dino fact

        painter.drawPixmap(150, 100, quizBackground.scaled(810,540));
        QRect titleBox(160, 110, 790, 50);
        painter.fillRect(titleBox,Qt::transparent);
        painter.setFont(credits);
        painter.drawText(titleBox, Qt::AlignCenter, factsHeader[currentDinosaur]);
        painter.setFont(factFont);
        QRect box(160, 170, 790, 460);
        //painter.drawText(300, 300, "Final dino fact!");
        painter.fillRect(box,Qt::transparent);
        painter.drawText(box, Qt::TextWordWrap, facts[currentDinosaur]);
        painter.drawPixmap(350, 657, pressF);
        return;
    }
}

void MuseumScene::drawCredits(){
    if(player->gameOver && startCredits){
        if (gameOverFrameCount < 120){
            gameOverFrameCount++;
            return;
        }

        QPixmap img(1080, 720);
        img.fill(Qt::transparent);

        QPixmap black(1080, 720);
        black.fill(Qt::black);

        QPainter p(&img);

        double val = (double)(gameOverFrameCount - 120) / 240;
        p.setOpacity(val);
        p.drawPixmap(0, 0, black);

        painter.drawPixmap(0, 0, img);

        QPixmap end(1080, 720);
        QPainter p1(&end);

        if (gameOverFrameCount > 360){

            p1.setOpacity((double)(gameOverFrameCount - 360) / 120);

            p1.setFont(credits);

            p1.setPen(QColor(255, 215, 0));

            p1.drawText(220, creditY, "Thank you for learning");
            p1.drawText(210, creditY + 50, "about dinosaurs with us!");

            p1.drawText(350, creditY + 450, "Team Members");
            p1.drawText(355, creditY + 900, "Arjun Sarkar");
            p1.drawText(325, creditY + 1350, "Dawson Jenkins");
            p1.drawText(305, creditY + 1800, "Ethan Heathcote");
            p1.drawText(325, creditY + 2250, "Hudson Bowman");
            p1.drawText(355, creditY + 2700, "Peyton Jensen");
            p1.drawText(395, creditY + 3150, "Samir Sarin");


            painter.drawPixmap(0, 0, end);
        }

        if (gameOverFrameCount >550) {
            creditY -= 3;
        }

        if (gameOverFrameCount == 1775){
            *currentScene = searchPtr;
            deactivate();
        }

        gameOverFrameCount++;
    }
}

void MuseumScene::activate(){
    if (activated){
        return;
    }

    //TESTING FOR END SCREEN
    //player->gameOver = true;
    painter.setPen(QColor(100, 100, 0));
    gameOverFrameCount = 0;
    currentDinosaur = player->currentDinosaur;
    animationFrameCount = 0;
    creditY = 320;
    animationX = 388;
    animationY = 12;
    animationDimension = 275;
    activated = true;
    animationActive = false;
    showDinoFact = false;
    //showDinoFact = true;
    winSoundPlayed = false;
    startCredits = false;
    closeQuiz = false;
    playerAnswered = -1;

    if (player->boneFound){
        // QUEUE UP THE QUIZ LOGIC THAT YOU WILL NEED
        quizNumber++;
        quizNumber = quizNumber == 3 ? 0 : quizNumber;
        currentQuestion = questionsMap[player->currentDinosaur][quizNumber];
    }

}

void MuseumScene::deactivate(){
    activated = false;
}

void MuseumScene::switchToSearchScene(){

    player->soundEffects.enqueue(SoundEffect::door);
    *currentScene = searchPtr;
    deactivate();

}

QVector<MuseumScene::Question> MuseumScene::loadQuestions(QString resourcePath, DinosaurName dinosaur) {
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

            facts[dinosaur] = question.toObject()["fact"].toString();
        }

        return result;
    } catch (...) {
        QVector<MuseumScene::Question> empty;
        return empty;
    }
}

