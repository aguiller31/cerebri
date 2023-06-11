#ifndef CEREBRI_CONTROL_ACKERMANN_PARAMETERS_H__
#define CEREBRI_CONTROL_ACKERMANN_PARAMETERS_H__

// gains
const double gain_along_track = CONFIG_CONTROL_ACKERMANN_GAIN_ALONG_TRACK/1000.0;
const double gain_cross_track = CONFIG_CONTROL_ACKERMANN_GAIN_CROSS_TRACK/1000.0;
const double gain_heading = CONFIG_CONTROL_ACKERMANN_GAIN_HEADING/1000.0;

// parameters
const double wheel_radius = CONFIG_CONTROL_ACKERMANN_WHEEL_RADIUS_MM/1000.0;
const double wheel_base = CONFIG_CONTROL_ACKERMANN_WHEEL_BASE_MM/1000.0;
const double max_turn_angle = CONFIG_CONTROL_ACKERMANN_MAX_TURN_ANGLE_MRAD/1000.0;
const double max_velocity = CONFIG_CONTROL_ACKERMANN_MAX_VELOCITY_MM_S/1000.0;


#endif /* CEREBRI_CONTROL_ACKERMANN_PARAMETERS_H__ */
