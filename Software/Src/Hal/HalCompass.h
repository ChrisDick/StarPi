#ifndef HALCOMPASS_H
#define HALCOMPASS_H

#include <stdint.h>

class HalCompass {
    public:
        HalCompass( void );
    
        bool HalCompassInit( void );
        void HalCompassSetPitch( float NewPitch );
        void HalCompassSetRoll( float NewRoll );
        void HalCompassRun( void );
        float HalCompassGetHeading( void );
        float HalCompassGetTiltCompensatedHeading( void );
        
        private:
        int16_t GetXRawHeading( void );
        int16_t GetYRawHeading( void );
        int16_t GetZRawHeading( void );
        float FilterX[5];
        float FilterY[5];
        float FilterZ[5];
        uint8_t FilterCount;
        float Scaling;
        float Pitch;
        float Roll;
};

#endif /* HALCOMPASS_H */
