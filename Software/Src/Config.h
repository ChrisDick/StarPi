
/*
   The type of accelerometer is: 
*/
//#define ADXL345_ACCEL
//#define BMA150_ACCEL
//#define L3G4200D_ACCEL
//#define MPU9150_ACCEL
#define MPU6050_ACCEL

/*
    Orientation of the accelerometer with respect to the telescope
    When the telescope is level (0 degrees elevation):
    The axis that points to the objective end of the telescope is
*/
//#define OBJECTIVE_END_ACCEL_X_PLUS
//#define OBJECTIVE_END_ACCEL_X_MINUS
//#define OBJECTIVE_END_ACCEL_Y_PLUS
#define OBJECTIVE_END_ACCEL_Y_MINUS
//#define OBJECTIVE_END_ACCEL_Z_PLUS
//#define OBJECTIVE_END_ACCEL_Z_MINUS
/*
    The axis which points to the right hand side when looking into the eyepiece is
*/
#define TELESCOPE_RIGHT_ACCEL_X_PLUS
//#define TELESCOPE_RIGHT_ACCEL_X_MINUS
//#define TELESCOPE_RIGHT_ACCEL_Y_PLUS
//#define TELESCOPE_RIGHT_ACCEL_Y_MINUS
//#define TELESCOPE_RIGHT_ACCEL_Z_PLUS
//#define TELESCOPE_RIGHT_ACCEL_Z_MINUS

/*
    The axis which points up is
*/
//#define UP_ACCEL_X_PLUS
//#define UP_ACCEL_X_MINUS
//#define UP_ACCEL_Y_PLUS
//#define UP_ACCEL_Y_MINUS
#define UP_ACCEL_Z_PLUS
//#define UP_ACCEL_Z_MINUS


/*
   The type of magnetometer is: 
*/
//#define AK8975_MAGNETOMETER
//#define HMC5843_MAGNETOMETER
#define HMC5883L_MAGNETOMETER
//#define MPU9150_MAGNETOMETER

/*
    Orientation of the magnetometer with respect to the telescope
    When the telescope is level (0 degrees elevation):
    The axis that points to the objective end of the telescope is
*/
#define OBJECTIVE_END_MAGNETOMETER_X_PLUS
//#define OBJECTIVE_END_MAGNETOMETER_X_MINUS
//#define OBJECTIVE_END_MAGNETOMETER_Y_PLUS
//#define OBJECTIVE_END_MAGNETOMETER_Y_MINUS
//#define OBJECTIVE_END_MAGNETOMETER_Z_PLUS
//#define OBJECTIVE_END_MAGNETOMETER_Z_MINUS
/*
    The axis which points to the right hand side when looking into the eyepiece is
*/
//#define TELESCOPE_RIGHT_MAGNETOMETER_X_PLUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_X_MINUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
#define TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS

/*
    The axis which points up is
*/
//#define UP_MAGNETOMETER_X_PLUS
//#define UP_MAGNETOMETER_X_MINUS
//#define UP_MAGNETOMETER_Y_PLUS
//#define UP_MAGNETOMETER_Y_MINUS
#define UP_MAGNETOMETER_Z_PLUS
//#define UP_MAGNETOMETER_Z_MINUS














/*
    define checking macros
*/

#if ((defined ADXL345) && (defined BMA150))
    #error two accelerometers defined
#endif
// etc.
