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
{
    background = background.scaled(1080, 720);
    questionsMap[DinosaurName::tRex] = loadQuestions(":/tRexQuestions.json");

    // --------------------- QUESTIONS TEST CASE --------------------- //
    // PRINTS ON START UP
    Question q = questionsMap[DinosaurName::tRex][0];
    qDebug() << "Question: " << q.question;
    qDebug() << "Option 0: " << q.options.at(0);
    qDebug() << "Option 1: " << q.options.at(1);
    qDebug() << "Option 2: " << q.options.at(2);
    qDebug() << "Option 3: " << q.options.at(3);
    qDebug() << "Answer is Option: " << q.correctIndex;
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



