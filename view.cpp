#include "view.h"
#include <QKeyEvent>
#include "ui_view.h"
#include "QPainter"

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

    music->play();

    connect(music, &QMediaPlayer::mediaStatusChanged, this, &View::loopAudio);

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

void View::loopAudio(){
    if (music->mediaStatus() == QMediaPlayer::EndOfMedia){
        music->play();
    }
}


void View::keyPressEvent(QKeyEvent *event)
{
    // if (typing) to disable when typing for guesses
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
    case Qt::Key_T:
        emit keyPressed(KeyStroke::test);
        break;
    case Qt::Key_1:
        music->play();
        break;
    case Qt::Key_2:
        music->stop();
        break;
    default: break;
    }
}

void View::updateFrame(QPixmap frame)
{
    ui->gameLabel->setPixmap(frame.scaled(1080, 720, Qt::KeepAspectRatio));
}

View::~View()
{
    delete ui;
}
