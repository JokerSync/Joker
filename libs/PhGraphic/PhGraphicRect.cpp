/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDebug>
#include "PhGraphicRect.h"

PhGraphicRect::PhGraphicRect(int x, int y, int z, int w, int h, QString color) :
    PhGraphicObject(color, x, y, z)
{
    this->setSize(w, h);
}

void PhGraphicRect::draw(int px)
{
    qDebug() << "I'm drawing a rect";
}

void PhGraphicRect::setSize(int w, int h){
    this->setHeight(h);
    this->setWidht(w);
}
void PhGraphicRect::setRect(int x, int y, int w, int h){
    this->setSize(w,h);
    this->setPosition(x,y,this->getZ());
}

void PhGraphicRect::setHeight(int h){
    _h = h;
}
void PhGraphicRect::setWidht(int w){
    _w = w;
}

int PhGraphicRect::getHeight(){
    return _h;
}
int PhGraphicRect::getWidth(){
    return _w;
}
