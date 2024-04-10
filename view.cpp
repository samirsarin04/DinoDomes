#include "view.h"
#include <QKeyEvent>
#include "ui_view.h"

#include "searchscene.h"

View::View(Model &model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    connect(this, &View::keyPressed, &model, &Model::handleKeyPress);
    connect(&model, &Model::sendFrameToView, this, &View::updateFrame);

    QPixmap background(":/background.png");
    QPixmap resizedBackground = background.scaled(1080, 720, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->backgroundLabel->setPixmap(resizedBackground);

}

void View::keyPressEvent(QKeyEvent *event)
{
    // if (typing) to disable when typing for guesses
    if (event->key() == Qt::Key_A) {
        emit keyPressed(KeyStroke::moveLeftKey);
    } else if (event->key() == Qt::Key_D) {
        emit keyPressed(KeyStroke::moveRightKey);
    } else if (event->key() == Qt::Key_M) {
        emit keyPressed(KeyStroke::museumKey);
    } else if (event->key() == Qt::Key_F) {
        emit keyPressed(KeyStroke::interactKey);
    }
}

void View::updateFrame(QImage frame)
{
    //ui->gameWindow->setPixmap(QPixmap::fromImage(frame).scaled(600, 400, Qt::KeepAspectRatio));
}

View::~View()
{
    delete ui;
}
