#include "view.h"
#include <QKeyEvent>
#include "ui_view.h"
#include "QPainter"
#include <QResizeEvent>

#include "searchscene.h"

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    connect(this, &View::keyPressed, &model, &Model::handleKeyPress);
    connect(&model, &Model::sendFrameToView, this, &View::updateFrame);

    music = new QMediaPlayer();
    output = new QAudioOutput();
    music->setAudioOutput(output);

    output->setVolume(.25);

    music->setSource(QUrl("qrc:/backgroundMusic.mp3"));

    QSoundEffect* dig = new QSoundEffect();
    dig->setSource(QUrl("qrc:/digSpotSound.wav"));
    dig->setVolume(.35);

    soundEffects[SoundEffect::digSpot] = dig;

    QSoundEffect* walk = new QSoundEffect();
    walk->setSource(QUrl("qrc:/walkSound.wav"));
    walk->setVolume(.5);

    soundEffects[SoundEffect::walk] = walk;

    QSoundEffect* findBone = new QSoundEffect();
    findBone->setSource(QUrl("qrc:/findBone.wav"));
    findBone->setVolume(.25);

    soundEffects[SoundEffect::findBone] = findBone;

    QSoundEffect* door = new QSoundEffect();
    door->setSource(QUrl("qrc:/door.wav"));
    door->setVolume(.4);

    soundEffects[SoundEffect::door] = door;

    //music->play();

    connect(music, &QMediaPlayer::mediaStatusChanged, this, &View::loopAudio);
    connect(&model, &Model::sendSoundEffect, this, &View::playSoundEffect);


    gameSize = QSize(1080, 720);
    // if (music->error() != QMediaPlayer::NoError){
    //     qDebug() << "busted";
    // }

    // PROOF OF CONCEPT
    // Big artboard containing multiple pixmaps and lines them up seamlessly side by side
    // Needs to be adapted to use foreground instead of background, add move functionality

    // QPixmap pixmap1(":/background.png");
    // QPixmap pixmap2(":/background.png");

    // pixmap1 = pixmap1.scaled(1080, 720);
    // pixmap2 = pixmap2.scaled(1080, 720);

    //QPixmap largePixmap(1080, 720);
    //largePixmap.fill(Qt::transparent);


    //QPainter painter(&largePixmap);

    //painter.drawPixmap(0, 0, pixmap1.scaled(1080, 720));
    // painter.drawPixmap(pixmap1.rect().right(), 0, pixmap2.scaled(1080, 720));

    //painter.end();
    //ui->backgroundLabel->setPixmap(largePixmap.scaled(1080, 720, Qt::KeepAspectRatio));


    // INITIALIZATION OF BACKGROUND
    // Needs to be moved to initialize within search scene
    // Data sent to view for drawing

    // QPixmap background(":/background.png");
    // background = background.scaled(1080, 720);
    // ui->gameLabel->setPixmap(background);
}

void View::resizeEvent(QResizeEvent *event){
    qDebug() << "resize event";
    gameSize = event->size();
    ui->gameLabel->resize(event->size());
    QWidget::resizeEvent(event);
}

void View::loopAudio(){
    if (music->mediaStatus() == QMediaPlayer::EndOfMedia){
        music->play();
    }
}

void View::playSoundEffect(SoundEffect sound){
    if (!soundEffects[sound]->isPlaying()){
        soundEffects[sound]->play();
    }
}


void View::keyPressEvent(QKeyEvent *event)
{
    // if (typing) to disable when typing for guesses
    switch(event->key()){
    case Qt::Key_A:
        //soundEffects[SoundEffect::digSpot]->play();
        emit keyPressed(KeyStroke::moveLeftKey);
        break;
    case Qt::Key_D:
        emit keyPressed(KeyStroke::moveRightKey);
        break;
    case Qt::Key_M:
        emit keyPressed(KeyStroke::museumKey);
        break;
    case Qt::Key_F:
        emit keyPressed(KeyStroke::interactKey);
        break;
    case Qt::Key_T:     //TODO remove for final submission
        emit keyPressed(KeyStroke::test);
        break;
    case Qt::Key_1:
        music->play();
        emit keyPressed(KeyStroke::oneKey);
        break;
    case Qt::Key_2:
        music->stop();
        emit keyPressed(KeyStroke::twoKey);
        break;
    case Qt::Key_3:
        music->stop();
        emit keyPressed(KeyStroke::threeKey);
        break;
    case Qt::Key_4:
        music->stop();
        emit keyPressed(KeyStroke::fourKey);
        break;
    default: break;
    }
}

void View::updateFrame(QPixmap frame)
{
    ui->gameLabel->setPixmap(frame.scaled(gameSize.width(), gameSize.height(), Qt::KeepAspectRatio));
}

View::~View()
{
    delete music;
    delete output;
    delete ui;
}
