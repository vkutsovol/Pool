#include "BallModel.h"
#include "Controller.h"
#include <thread>
#include <math.h>
# define M_PI         3.141592653589793238462643383279502884 /* pi */

BallModel::BallModel() {
	type = "Ball";
	frames = 250;
	m = 100000;
	startSpeed = 0.12f; //number of points to pass for model in frames/second
	//	stepLeft = 0.0; //numer of point for model to pass to left direction
	speedY = 0.0; // -//- up
	speedX = 0.0; // -//- right
	//	stepDown = 0.0; // -//- down
	//position
	x = 0.0; 
	y = 0.0;
	friction = 0;//0.002f; //number of points to decrease in frames/second for model
	radius = 20; //radious
	maxSpeed = 4.0; //max number of points to pass for model in frames/second
	//	listen();
}
BallModel::~BallModel() {
}



int BallModel::getRadius(){
	return radius;
}
void BallModel::setRadius(int radius) {
	this->radius = radius;
}

void BallModel::move(MoveType move) {
	switch (move) {
	case LEFT:
		speedX -= startSpeed;
		if (abs(speedX)> maxSpeed) speedX = -maxSpeed;
		break;
	case UP:
		speedY += startSpeed;
		if (abs(speedY)> maxSpeed) speedY = maxSpeed;
		break;
	case RIGHT:
		speedX += startSpeed;
		if (abs(speedX)> maxSpeed) speedX = maxSpeed;
		break;
	case DOWN:
		speedY -= startSpeed;
		if (abs(speedY)> maxSpeed) speedY = -maxSpeed;
		break;
	}
}

void BallModel::correctSpeed() {
	// speed c = sqrt ( speedX^2 + speedY^2)
	float c = sqrt(pow(speedX,2) + pow(speedY,2));
	if (c < friction) {
		c = speedX = speedY = 0;
	} 
	float localFriction = friction;
	//controll speed
	if (c > maxSpeed) {
		localFriction = c - maxSpeed; 
	}

	float x  = abs((c - localFriction)/c);
	if (x < 1) {
		speedX*=x;
		speedY*=x;
	}
}

void BallModel::listen() {
	std::thread t1(&BallModel::doListening, this);
	t1.detach();
}

void BallModel::doListening() {
	areaWidth = view->areaWidth;
	areaHeight = view->areaHeight;
	while (true) {

		y+=speedY;
		x+=speedX;
		//	correctSpeed();

		// mirror model way from vertical borders
		if (x+radius+1>=areaWidth/2 || x-radius-1<=-areaWidth/2) {
			speedX = -speedX;
		}
		// mirror model from horizontal borders
		if (y+radius+1>=areaHeight/2 || y-radius-1<=-areaHeight/2) {
			speedY = -speedY;
		}

		/*
		if (controller != NULL) {
		vector<Model*> models = controller->getView()->getModels();
		for (auto model: models) {
		if (this == model) continue;
		BallModel* localModel = (BallModel*) model;
		float dis = sqrt(pow(x - localModel->getX(),2) + pow(y - localModel->getY(), 2));
		if (dis  <= radius + localModel->getRadius()) {

		}
		}
		}*/

		if (view != NULL) {
			vector<Model*> models = view->getModels();
			for (auto model: models) {
				if (this == model) continue;
				BallModel* localModel = (BallModel*) model;
				float dis = sqrt(pow(x - localModel->getX(),2) + pow(y - localModel->getY(), 2));
				if (dis  <= radius + localModel->getRadius()) {
					cout << "lower" << endl;
					float dx = x-localModel->getX();
					float dy = y-localModel->getY();
					float dif = dx!=0?dy/dx:0;
					float collisionision_angle = atan(dif);
					float magnitude_1 = sqrt(pow(speedX,2) + pow(speedY,2));
					float magnitude_2 = sqrt(pow(localModel->speedX,2) + pow(localModel->speedY,2));
					dif = speedX!=0?speedY/speedX:0;
					float direction_1 = atan(dif);
					dif = localModel->speedX!=0?localModel->speedY/localModel->speedX:0;
					float direction_2 = atan(dif);
					cout << collisionision_angle << " " << magnitude_1 << " " << magnitude_2 << " " << direction_1 << " " << direction_2 << endl;
					float new_xspeed_1 = magnitude_1*cos(direction_1-collisionision_angle);
					float new_yspeed_1 = magnitude_1*sin(direction_1-collisionision_angle);
					float new_xspeed_2 = magnitude_2*cos(direction_2-collisionision_angle);
					float new_yspeed_2 = magnitude_2*sin(direction_2-collisionision_angle);
					float final_xspeed_1 = ((m-localModel->m)*new_xspeed_1+(localModel->m+localModel->m)*new_xspeed_2)/(m+localModel->m);
					float final_xspeed_2 = ((m+m)*new_xspeed_1+(localModel->m-m)*new_xspeed_2)/(m+localModel->m);
					float final_yspeed_1 = new_yspeed_1;
					float final_yspeed_2 = new_yspeed_2;
					speedX = cos(collisionision_angle)*final_xspeed_1+cos(collisionision_angle+M_PI/2)*final_yspeed_1;
					speedY = sin(collisionision_angle)*final_xspeed_1+sin(collisionision_angle+M_PI/2)*final_yspeed_1;
					localModel->speedX = cos(collisionision_angle)*final_xspeed_2+cos(collisionision_angle+M_PI/2)*final_yspeed_2;
					localModel->speedY = sin(collisionision_angle)*final_xspeed_2+sin(collisionision_angle+M_PI/2)*final_yspeed_2;
					
//					float dis = sqrt(pow(x - localModel->getX(),2) + pow(y - localModel->getY(), 2));
//					while (dis  <= radius + localModel->getRadius()) {
						x += speedX;
						y += speedY;
						localModel->x += localModel->speedX;
						localModel->y += localModel->speedY;
//					}


				}
				cout << type << " " << speedX << " " << speedY << " " << localModel->speedX << " " << localModel->speedY << endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds((long)(1000/frames)));
	}
}


