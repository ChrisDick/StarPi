
/*
   The type of accelerometer is: 
*/
#define LSM303DLHC_ACCEL

/*
    Orientation of the accelerometer with respect to the telescope
    When the telescope is level (0 degrees elevation):
    The axis that points to the objective end of the telescope is
*/
#define OBJECTIVE_END_ACCEL_X_PLUS
//#define OBJECTIVE_END_ACCEL_X_MINUS
//#define OBJECTIVE_END_ACCEL_Y_PLUS
//#define OBJECTIVE_END_ACCEL_Y_MINUS
//#define OBJECTIVE_END_ACCEL_Z_PLUS
//#define OBJECTIVE_END_ACCEL_Z_MINUS
/*
    The axis which points to the right hand side when looking into the eyepiece is
*/
//#define TELESCOPE_RIGHT_ACCEL_X_PLUS
//#define TELESCOPE_RIGHT_ACCEL_X_MINUS
#define TELESCOPE_RIGHT_ACCEL_Y_PLUS
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

#define CONFIG_AXMAX ((float)16192.000000  )
#define CONFIG_AXMIN ((float)-16704.000000 )
#define CONFIG_AYMAX ((float)16640.000000  )
#define CONFIG_AYMIN ((float)-16384.000000 )
#define CONFIG_AZMAX ((float)17984.000000  )
#define CONFIG_AZMIN ((float)-17984.000000 )

#define CONFIG_AX_OFFSET ((float) ((CONFIG_AXMIN + CONFIG_AXMAX)/2.0))
#define CONFIG_AY_OFFSET ((float) ((CONFIG_AYMIN + CONFIG_AYMAX)/2.0))
#define CONFIG_AZ_OFFSET ((float) ((CONFIG_AZMIN + CONFIG_AZMAX)/2.0))


/*
   The type of magnetometer is: 
*/
//#define AK8975_MAGNETOMETER
//#define HMC5843_MAGNETOMETER
//#define HMC5883L_MAGNETOMETER
//#define MPU9150_MAGNETOMETER
#define LSM303DLHC_MAGNETOMETER

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
//#define UP_MAGNETOMETER_Z_PLUS
#define UP_MAGNETOMETER_Z_MINUS

/*
    Calibration Values:        
*/
/* on jig */
#define CONFIG_MXMAX ((float) 567.000000   )
#define CONFIG_MXMIN ((float) -472.000000  )
#define CONFIG_MYMAX ((float) 686.000000   )
#define CONFIG_MYMIN ((float) -377.000000  )
#define CONFIG_MZMAX ((float) 383.000000   )
#define CONFIG_MZMIN ((float) -532.000000  )

#define CONFIG_MX_OFFSET ((float) ((CONFIG_MXMIN + CONFIG_MXMAX)/2.0))
#define CONFIG_MY_OFFSET ((float) ((CONFIG_MYMIN + CONFIG_MYMAX)/2.0))
#define CONFIG_MZ_OFFSET ((float) ((CONFIG_MZMIN + CONFIG_MZMAX)/2.0))

/*
    Timing defines
*/
#if 0
#include "GPIO.h"
#define TIMING
#define TELESCOPE_MANAGER_PIN      BCM_GPIO16
#define TELESCOPE_ORIENTATION_PIN  BCM_GPIO19
#define HAL_WEBSOCKETD_PIN         BCM_GPIO20
#define HAL_GPS_PIN                BCM_GPIO13
#define SERVER_PI_PIN              BCM_GPIO26
#endif /* end of timing defines #if 0 */

