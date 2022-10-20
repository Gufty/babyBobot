#pragma once

#include "main.h"
#include "../Constants.h"
#include <string>

using namespace Constants;
using namespace pros;

struct Vector2{
    double x;
    double y;

	Vector2(double x = 0.0, double y = 0.0):x(x),y(y){}
	
	Vector2 operator+(Vector2 otherVector){
		return {x + otherVector.x, y + otherVector.y};
	}

	Vector2 operator-(Vector2 otherVector){
		return {x - otherVector.x, y - otherVector.y};
	}

	Vector2 operator*(Vector2 otherVector){
		return {x * otherVector.x, y * otherVector.y};
	}

	Vector2 operator/(Vector2 otherVector){
		return {x / otherVector.x, y / otherVector.y};
	}
};

class Odometry {
	public:
		Vector2 pos = {0,0};
		double theta = 0.0;
		double leftEncoder = 0.0;
		double rightEncoder = 0.0;
	private:
		Motor fl_mtr;
		Motor bl_mtr;
		Motor fr_mtr;
		Motor br_mtr;

		double newLeft; double newRight;
		double deltaLeft; double deltaRight; double deltaCenter; double deltaPerp;
		double phi;

		lv_obj_t* odometryInfo;

		std::function<void(void)> odomTick = [=](){
			newLeft = (fl_mtr.get_position() + bl_mtr.get_position()) / 2;
			newRight = (fr_mtr.get_position() + br_mtr.get_position()) / 2;
			deltaLeft = newLeft - leftEncoder;
			deltaRight = newRight - rightEncoder;
			deltaCenter = (deltaLeft + deltaRight) / 2;
			phi = (deltaLeft - deltaRight) / trackwidth;
			
			pos.x += deltaCenter * cos(theta) - deltaPerp * sin(theta);
			pos.y += deltaCenter * sin(theta) + deltaPerp * cos(theta);
			theta += phi;

			leftEncoder = newLeft;
			rightEncoder = newRight;
			
			delay(200);
			lv_label_set_text(odometryInfo, ("(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")").c_str());
		};
    public:
        Odometry(Motor fl_mtr, Motor bl_mtr, Motor fr_mtr, Motor br_mtr, lv_obj_t* &odometryInfo):fl_mtr(fl_mtr),bl_mtr(bl_mtr),fr_mtr(fr_mtr),br_mtr(br_mtr),odometryInfo(odometryInfo){}
        Task odomTask{odomTick};
};