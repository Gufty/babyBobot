
#pragma once

#include "Vector2.hpp"

class Line{
    public:
    Vector2 startPos, endPos, disToCord;
    double disBtwnCords, ratio;
    Line(Vector2 startPos, Vector2 endPos):startPos(startPos),endPos(endPos){}
    Vector2 distanceToCoordinate(double dis){
      disToCord = endPos.operator-(startPos);
      disBtwnCords = startPos.distanceTo(endPos);
      ratio = dis/disBtwnCords;
      disToCord.x = disToCord.x * ratio + startPos.x;
      disToCord.y = disToCord.y * ratio + startPos.y;
      return disToCord;
    }
};