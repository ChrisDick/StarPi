
/*
   The type of accelerometer is: 
*/
//#define ADXL345_ACCEL
//#define BMA150_ACCEL
//#define L3G4200D_ACCEL
//#define MPU9150_ACCEL
#define MPU6050_ACCEL
//#define LSM303DLHC_ACCEL

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
//#define LSM303DLHC_MAGNETOMETER

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
#define TELESCOPE_RIGHT_MAGNETOMETER_Y_PLUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Y_MINUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Z_PLUS
//#define TELESCOPE_RIGHT_MAGNETOMETER_Z_MINUS

/*
    The axis which points up is
*/
//#define UP_MAGNETOMETER_X_PLUS
//#define UP_MAGNETOMETER_X_MINUS
//#define UP_MAGNETOMETER_Y_PLUS
//#define UP_MAGNETOMETER_Y_MINUS
//#define UP_MAGNETOMETER_Z_PLUS
#define UP_MAGNETOMETER_Z_MINUS

/*
    Calibration Values:
*/
#define CONFIG_MxMax ((float) 612.000000   )
#define CONFIG_MxMin ((float) -518.000000  )
#define CONFIG_MyMax ((float) 548.000000   )
#define CONFIG_MyMin ((float) -582.000000  )
#define CONFIG_MzMax ((float) 318.000000   )
#define CONFIG_MzMin ((float) -666.000000  )


#define CONFIG_MX_OFFSET ((float) ((CONFIG_MxMin + CONFIG_MxMax)/2.0))
#define CONFIG_MY_OFFSET ((float) ((CONFIG_MyMin + CONFIG_MyMax)/2.0))
#define CONFIG_MZ_OFFSET ((float) ((CONFIG_MzMin + CONFIG_MzMax)/2.0))

/*
   The type of gyro is: 
*/
//#define ITG3200_GYRO
//#define MPU9150_GYRO
#define MPU6050_GYRO

/*
    Orientation of the accelerometer with respect to the telescope
    When the telescope is level (0 degrees elevation):
    The axis that points to the objective end of the telescope is
*/
//#define OBJECTIVE_END_GYRO_X_PLUS
//#define OBJECTIVE_END_GYRO_X_MINUS
//#define OBJECTIVE_END_GYRO_Y_PLUS
#define OBJECTIVE_END_GYRO_Y_MINUS
//#define OBJECTIVE_END_GYRO_Z_PLUS
//#define OBJECTIVE_END_GYRO_Z_MINUS
/*
    The axis which points to the right hand side when looking into the eyepiece is
*/
#define TELESCOPE_RIGHT_GYRO_X_PLUS
//#define TELESCOPE_RIGHT_GYRO_X_MINUS
//#define TELESCOPE_RIGHT_GYRO_Y_PLUS
//#define TELESCOPE_RIGHT_GYRO_Y_MINUS
//#define TELESCOPE_RIGHT_GYRO_Z_PLUS
//#define TELESCOPE_RIGHT_GYRO_Z_MINUS

/*
    The axis which points up is
*/
//#define UP_GYRO_X_PLUS
//#define UP_GYRO_X_MINUS
//#define UP_GYRO_Y_PLUS
//#define UP_GYRO_Y_MINUS
//#define UP_GYRO_Z_PLUS
#define UP_GYRO_Z_MINUS

/*
    Motor stuff
*/
#define LM29XIN1 WIRINGPIN26
#define LM29XIN2 WIRINGPIN27
#define LM29XIN3 WIRINGPIN28
#define LM29XIN4 WIRINGPIN29
#define PWM1
#define PWM2
#define LM29X
#define LM29X1 1
#define LM29X2 2
#define CONFIG_NUMBER_OF_MOTORS 2
#define MOTOR_ONE LM29X1
#define MOTOR_TWO LM29X2






/*
    define checking macros
*/

#if ((defined ADXL345) && (defined BMA150))
    #error two accelerometers defined
#endif
// etc.
