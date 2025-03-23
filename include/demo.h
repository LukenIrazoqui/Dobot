#ifndef DEMO_H
#define DEMO_H


#include <qcontainerfwd.h>
#include <QSerialPortInfo>
#include "dobot.h"
#include <memory>
#include "DobotDll.h"
#include "DobotType.h"

class Demo
{
public:
    Demo();
    ~Demo();

    void exec();
private:
    void findDobotBySerialNumbers();
    void startMotor();
    void stopMotor();
    void moveBox(Dobot::s_POSITION startingPos, Dobot::s_POSITION endingPos);
    void moveBoxToSensor();
    void moveBoxToMat();
    void moveBoxToCorrectPosition();

    std::unique_ptr<Dobot> armOnMat;
    std::unique_ptr<Dobot> armTakingFromMat;

    const std::string serialNumberArmOnMat = "SerialNumber1";
    const std::string serialNumberArmTakingFromMat = "SerialNumber2";

    static constexpr Dobot::s_POSITION BOX_TAKE_POSITION{1, 1, 1, 1};
    static constexpr Dobot::s_POSITION BOX_DROP_MAT_POSITION{1, 1, 1, 1};
    static constexpr Dobot::s_POSITION SENSOR_POSITION{1, 1, 1, 1};
    static constexpr Dobot::s_POSITION BOX_TAKE_MAT_POSITION{1, 1, 1, 1};
    static constexpr Dobot::s_POSITION BOX_DROP_POSITION{1, 1, 1, 1};
    static constexpr int BOX_SIDE_LEN = 30;
    int boxCount;
};

#endif // DEMO_H
