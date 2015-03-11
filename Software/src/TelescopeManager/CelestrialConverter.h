
#include <stdint.h>

typedef struct{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} CC_TIME_T;

/*
    All angles are in radians
*/
typedef struct {
    float longitude_west;
    float latitude;
    float azimuth;
    float altitude;
    float hour_angle;
    float right_ascension;
    float declination;
} CC_ANGLES_T;

class CelestrialConverter {

    public:
        CelestrialConverter();
        float calculate_local_sidereal_time( CC_TIME_T gst, float longitude_west);
        CC_TIME_T add_time( CC_TIME_T time_a, CC_TIME_T time_b );
        CC_TIME_T subtract_time( CC_TIME_T time_a, CC_TIME_T time_b );
        float decimalise_time( CC_TIME_T time );
        CC_TIME_T un_decimalise_time( float time_dec );
        float convert_time_to_angle( CC_TIME_T time );
        CC_TIME_T convert_angle_to_time( float angle );
        void equitorial_to_celestrial( CC_ANGLES_T* angles, CC_TIME_T grenwich_standard_time );
        void celestrial_to_equitorial( CC_ANGLES_T* angles, CC_TIME_T grenwich_standard_time );

};