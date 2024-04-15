#include "scene.h"
#include <Qimage>

Scene::Scene(PlayerState& player, Scene** currentScene, QObject *parent)
    : QObject{parent}
    , player(&player)
    , world(b2Vec2(0, 0))
    , currentScene(currentScene)
    , frame(1080, 720)
    , painter(&frame)
    , timeStep(1.0f / 60.0f)
    , deactivated(true)
{}


// THIS IS BASICALLY JUST COPY PASTED FROM LAB
// THIS IS ALMOST CERTAIN TO BE CHANGED FOR OUR PURPOSES
// THIS IS IMPLEMENTED ON A PROOF OF CONCEPT LEVEL AT THE MOMENT
    void Scene::setupBox2D(){};
//     // Define the gravity vector.
//     b2Vec2 gravity(0.0f, 0.0f);

//     // Construct a world object, which will hold and simulate the rigid bodies.
//     b2World world(gravity);


    // // Define the ground body.
    // b2BodyDef groundBodyDef;
    // groundBodyDef.position.Set(0.0f, -10.0f);

    // // Call the body factory which allocates memory for the ground body
    // // from a pool and creates the ground box shape (also from a pool).
    // // The body is also added to the world.
    // b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // // Define the ground box shape.
    // b2PolygonShape groundBox;

    // // The extents are the half-widths of the box.
    // groundBox.SetAsBox(50.0f, 10.0f);

    // // Add the ground fixture to the ground body.
    // groundBody->CreateFixture(&groundBox, 0.0f);

    // // Define the dynamic body. We set its position and call the body factory.
    // b2BodyDef bodyDef;
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position.Set(0.0f, 4.0f);
    // body = world.CreateBody(&bodyDef);

    // // Define another box shape for our dynamic body.
    // b2PolygonShape dynamicBox;
    // dynamicBox.SetAsBox(1.0f, 1.0f);

    // // Define the dynamic body fixture.
    // b2FixtureDef fixtureDef;
    // fixtureDef.shape = &dynamicBox;

    // // Set the box density to be non-zero, so it will be dynamic.
    // fixtureDef.density = 1.0f;

    // // Override the default friction.
    // fixtureDef.friction = 0.3f;

    // fixtureDef.restitution = 0.9f;

    // // Add the shape to the body.
    // body->CreateFixture(&fixtureDef);
// }

QPixmap Scene::buildScene(){
    return QPixmap();
}

void Scene::activate(){

}

void Scene::deactivate(){

}

void Scene::processPlayerInput(){

}
