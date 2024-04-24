#include "view.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include "ui_view.h"
#include <QMainWindow>
#include "keystroke.h"
#include "model.h"
#include "soundeffect.h"
#include <QSoundEffect>
#include <QtMultimedia>

///Reviewed by Ethan Heathcote

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    connect(this, &View::keyPressed, &model, &Model::handleKeyPress);
    connect(&model, &Model::sendFrameToView, this, &View::updateFrame);

    //Creates the music player and the defines the audio output
    music = new QMediaPlayer();
    output = new QAudioOutput();
    music->setAudioOutput(output);

    output->setVolume(.15);

    music->setSource(QUrl("qrc:/audio/backgroundMusic.mp3"));

    dig = new QSoundEffect();
    dig->setSource(QUrl("qrc:/audio/digSpotSound.wav"));
    dig->setVolume(.35);

    soundEffects[SoundEffect::digSpot] = dig;

    walk = new QSoundEffect();
    walk->setSource(QUrl("qrc:/audio/walkSound.wav"));
    walk->setVolume(.5);

    soundEffects[SoundEffect::walk] = walk;

    findBone = new QSoundEffect();
    findBone->setSource(QUrl("qrc:/audio/findBone.wav"));
    findBone->setVolume(.4);

    soundEffects[SoundEffect::findBone] = findBone;

    door = new QSoundEffect();
    door->setSource(QUrl("qrc:/audio/door.wav"));
    door->setVolume(.4);

    soundEffects[SoundEffect::door] = door;

    shovel = new QSoundEffect();
    shovel->setSource(QUrl("qrc:/audio/dig.wav"));
    shovel->setVolume(.55);

    soundEffects[SoundEffect::shovel] = shovel;

    brush = new QSoundEffect();
    brush->setSource(QUrl("qrc:/audio/brush.wav"));
    brush->setVolume(.85);

    soundEffects[SoundEffect::brush] = brush;

    pageTurn = new QSoundEffect();
    pageTurn->setSource(QUrl("qrc:/audio/pageTurn.wav"));
    pageTurn->setVolume(.4);

    soundEffects[SoundEffect::pageTurn] = pageTurn;

    correct = new QSoundEffect();
    correct->setSource(QUrl("qrc:/audio/correct.wav"));
    correct->setVolume(.4);

    soundEffects[SoundEffect::correct] = correct;

    wrong = new QSoundEffect();
    wrong->setSource(QUrl("qrc:/audio/wrong.wav"));
    wrong->setVolume(.85);

    soundEffects[SoundEffect::wrong] = wrong;

    victory = new QSoundEffect();
    victory->setSource(QUrl("qrc:/audio/victory.wav"));
    victory->setVolume(.25);

    soundEffects[SoundEffect::victory] = victory;


    music->play();

    connect(music, &QMediaPlayer::mediaStatusChanged, this, &View::loopAudio);
    connect(&model, &Model::sendSoundEffect, this, &View::playSoundEffect);

    // Captures the initial size of the game for resizing
    gameSize = QSize(1080, 720);
}

void View::resizeEvent(QResizeEvent *event){
    //Resizes the game label to fit the window
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
    switch(event->key()){
    case Qt::Key_A:
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
    case Qt::Key_1:
        emit keyPressed(KeyStroke::oneKey);
        break;
    case Qt::Key_2:
        emit keyPressed(KeyStroke::twoKey);
        break;
    case Qt::Key_3:
        emit keyPressed(KeyStroke::threeKey);
        break;
    case Qt::Key_4:
        emit keyPressed(KeyStroke::fourKey);
        break;
    default:
        emit keyPressed(KeyStroke::any);
        break;
    }
}

void View::updateFrame(QPixmap frame)
{
    ui->gameLabel->setPixmap(frame.scaled(gameSize.width(), gameSize.height(), Qt::KeepAspectRatio));
}

View::~View()
{
    delete walk;
    delete findBone;
    delete door;
    delete shovel;
    delete brush;
    delete pageTurn;
    delete correct;
    delete wrong;
    delete victory;
    delete music;
    delete output;
    delete ui;
}
