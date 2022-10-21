#pragma once

#include "main.h"
#include "../Constants.h"
#include "../Display.h"

using namespace Constants;
using namespace Display;
using namespace pros;

#define ODOM_DEBUG

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
	private:
		Motor* fl_mtr;
		Motor* bl_mtr;
		Motor* fr_mtr;
		Motor* br_mtr;

		double newLeft, newRight;
		double deltaLeft, deltaRight, deltaCenter;
		double phi;
		double deltaPerp;

		lv_obj_t** odometryInfo;

	public:
		Vector2 pos = {0,0};
		double heading = 0.0;
		double leftEncoder = 0.0;
		double rightEncoder = 0.0;

		std::function<void(void)> odomTick = [=](){
			newLeft = (fl_mtr->get_position() + bl_mtr->get_position()) / 2;
			newRight = (fr_mtr->get_position() + br_mtr->get_position()) / 2;

			deltaLeft = newLeft - leftEncoder;
			deltaRight = newRight - rightEncoder;
			deltaCenter = (deltaLeft + deltaRight) / 2;

			phi = (deltaLeft - deltaRight) / (trackwidth * inchesToUnits);
			deltaPerp = deltaCenter * phi;
			
			pos.x += (deltaCenter * cos(heading) - deltaPerp * sin(heading))/inchesToUnits;
			pos.y += (deltaCenter * sin(heading) + deltaPerp * cos(heading))/inchesToUnits;
			heading += phi;

			leftEncoder = newLeft;
			rightEncoder = newRight;

			lv_label_set_text(*odometryInfo, ("Position: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ") Theta: " + std::to_string(heading)).c_str());
			delay(20);
		};
        Odometry(Motor* fl_mtr, Motor* bl_mtr, Motor* fr_mtr, Motor* br_mtr, lv_obj_t** odometryInfo):fl_mtr(fl_mtr), bl_mtr(bl_mtr), fr_mtr(fr_mtr), br_mtr(br_mtr), odometryInfo(odometryInfo){}
};