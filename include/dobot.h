#ifndef DOBOT_H
#define DOBOT_H

#include "DobotDll.h"
#include "DobotType.h"
#include <cstdint>
#include <cstring>
#include <iostream>

class Dobot
{
public:
    typedef struct s_POSITION {
        int X;
        int Y;
        int Z;
        int R;
    } s_POSITION;

    typedef enum e_COLOR {
        DEFAULT,
        R,
        G,
        B
    } e_COLOR;

    Dobot(const char *portName);
    ~Dobot();

    void connect();
    void disconnect();

    void ptpMovj(s_POSITION position);
    void ptpMovl(s_POSITION position);
    void ptpJump(s_POSITION position);
    Pose getPose();

    void initColorSensor(ColorPort colorPort);
    void initInfraredSensor(InfraredPort infraredPort);
    void setEMotor(int32_t speed);

    e_COLOR getColorSensor();
    bool getInfraredSensor();

    void enableEMotor();
    void disableEMotor();
    void setEMotorIndex(uint8_t index);
    void setEMotorSpeed(int32_t speed);

    const char *getPortName() const;
    void setPortName(const char *newPortName);

    std::string getSerialNumber() const;
    std::string getName() const;

    void goHome();
    void setHome(s_POSITION position);

    void disableSuctionCup();
    void enableSuctionCup();

private:
    void sendPTP(s_POSITION position, PTPMode mode);
    void init();
    void setEMotor();

    bool is_connected;
    const char *portName;
    uint8_t majorVersion, minorVersion, revision, hwVersion;
    char deviceName[64];
    char deviceSN[64];

    uint8_t COLOR_VERSION;
    ColorPort colorPort;
    uint8_t INFRARED_VERSION;
    InfraredPort infraredPort;
    EMotor eMotor;
};

#endif // DOBOT_H
