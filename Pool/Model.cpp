#include "Model.h"

int Model::getX() {
	return x;
}
void Model::setX(int x) {
	this->x=x;
}
int Model::getY() {
	return y;
}
void Model::setY() {
	this->y=y;
}
int Model::getRadius() {
	return radius;
}
void Model::setRadius() {
	this->radius=radius;
}
void Model::setType(string type) {
	this->type = type;
}
string Model::getType() {
	return type;
}