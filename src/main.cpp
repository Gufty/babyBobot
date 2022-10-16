#include "main.h"
#include "systems/DriveTrain.h"
#include "systems/Roller.h"
#include "systems/Extender.h"

#include "../include/display/lvgl.h"
using namespace pros;

Controller master(E_CONTROLLER_MASTER);

DriveTrain dt = DriveTrain();
Roller roll = Roller();
Extender xtend = Extender();

void lv_disp(lv_img_dsc_t cArr)
{
    lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &cArr);
    lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void initialize() { 
	LV_IMG_DECLARE(alpha);
	lv_disp(alpha);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	roll.rollerHalfStep();
	delay(300);
	dt.tankDrive(30, 30);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		dt.arcadeDrive(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

		if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) {roll.rollerHalfStep();}
		if (master.get_digital(E_CONTROLLER_DIGITAL_L2)) {roll.rollerHalfStep(-1);}

		if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {xtend.spoolXtend(1);}
		if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {xtend.spoolXtend(-1);}

        delay(20);
    }
}