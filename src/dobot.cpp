#include "dobot.h"

#include "DobotDll.h"
#include "DobotType.h"

Dobot::Dobot(const char *portName)
{
    is_connected = false;
    portName = portName;

    eMotor.index = 0;
    eMotor.isEnabled = false;
    eMotor.speed = 0;
}

Dobot::~Dobot()
{
    disconnect();
}

void Dobot::connect()
{
    char fwType[20];
    char version[20];
    //connect dobot
    if (!is_connected) {
        if (ConnectDobot(portName, 115200, fwType, version, &dobotId) != DobotConnect_NoError) {
            std::cout << "Connect dobot error!!!";
            return;
        }
        std::cout << "dobotId" << dobotId;
        is_connected = true;
        init();

        std::cout << "connect success!!!";
    } else {
        is_connected = false;
        DisconnectDobot(dobotId);
    }
}


void Dobot::disconnect()
{
    if (!is_connected) {
        if (DisconnectDobot(dobotId) != DobotConnect_NoError) {
            std::cout << "Disconnect dobot error!!!";
            return;
        }

        is_connected = false;
        std::cout << "disconnect success!!!";
    }
}



Pose Dobot::getPose()
{
    Pose pose;

    while (GetPose(dobotId, &pose) != DobotCommunicate_NoError) {
    }

    return pose;
}

void Dobot::init()
{
    //Command timeout
    SetCmdTimeout(dobotId, 3000);

    //clear old commands and set the queued command running
    SetQueuedCmdClear(dobotId);
    SetQueuedCmdStartExec(dobotId);

    GetDeviceSN(dobotId, deviceSN, sizeof(deviceSN));

    GetDeviceName(dobotId, deviceName, sizeof(deviceName));

    GetDeviceVersion(dobotId, &majorVersion, &minorVersion, &revision);

    //set the end effector parameters
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 71.6f;
    SetEndEffectorParams(dobotId, &endEffectorParams, false, NULL);

    JOGJointParams jogJointParams;
    for (int i = 0; i < 4; i++) {
        jogJointParams.velocity[i] = 100;
        jogJointParams.acceleration[i] = 100;
    }
    SetJOGJointParams(dobotId, &jogJointParams, false, NULL);

    JOGCoordinateParams jogCoordinateParams;
    for (int i = 0; i < 4; i++) {
        jogCoordinateParams.velocity[i] = 100;
        jogCoordinateParams.acceleration[i] = 100;
    }
    SetJOGCoordinateParams(dobotId, &jogCoordinateParams, false, NULL);

    JOGCommonParams jogCommonParams;
    jogCommonParams.velocityRatio = 50;
    jogCommonParams.accelerationRatio = 50;
    SetJOGCommonParams(dobotId, &jogCommonParams, false, NULL);

    PTPJointParams ptpJointParams;
    for (int i = 0; i < 4; i++) {
        ptpJointParams.velocity[i] = 100;
        ptpJointParams.acceleration[i] = 100;
    }
    SetPTPJointParams(dobotId, &ptpJointParams, false, NULL);

    PTPCoordinateParams ptpCoordinateParams;
    ptpCoordinateParams.xyzVelocity = 100;
    ptpCoordinateParams.xyzAcceleration = 100;
    ptpCoordinateParams.rVelocity = 100;
    ptpCoordinateParams.rAcceleration = 100;
    SetPTPCoordinateParams(dobotId, &ptpCoordinateParams, false, NULL);

    PTPJumpParams ptpJumpParams;
    ptpJumpParams.jumpHeight = 20;
    ptpJumpParams.zLimit = 150;
    SetPTPJumpParams(dobotId, &ptpJumpParams, false, NULL);
}


void Dobot::ptpMovj(s_POSITION position)
{
    sendPTP(position, PTPMOVJXYZMode);
}


void Dobot::ptpMovl(s_POSITION position)
{
    sendPTP(position, PTPMOVLXYZMode);
}


void Dobot::ptpJump(s_POSITION position)
{
    sendPTP(position, PTPJUMPXYZMode);
}

void Dobot::sendPTP(s_POSITION position, PTPMode mode)
{
    PTPCmd ptpCmd;
    ptpCmd.ptpMode = mode;
    ptpCmd.x = position.X;
    ptpCmd.y = position.Y;
    ptpCmd.z = position.Z;
    ptpCmd.r = position.R;

    while (SetPTPCmd(dobotId, &ptpCmd, true, NULL) != DobotCommunicate_NoError) {
    }
}


void Dobot::initColorSensor(ColorPort colorPort)
{
    while (SetColorSensor(dobotId, true, colorPort) != DobotCommunicate_NoError) {
    }
    this->colorPort = colorPort;
}

void Dobot::initInfraredSensor(InfraredPort infraredPort)
{
    while (SetInfraredSensor(dobotId,true, infraredPort) != DobotCommunicate_NoError) {
    }
    this->infraredPort = infraredPort;
}


Dobot::e_COLOR Dobot::getColorSensor()
{
    uint8_t r, g, b;

    while (GetColorSensor(dobotId, &r, &g, &b) != DobotCommunicate_NoError) {
    }

    // Check the color detected
    if (r == 1) {
        return e_COLOR::R;
    }
    else if (g == 1) {
        return e_COLOR::G;
    }
    else if (b == 1) {
        return e_COLOR::B;
    }
    else {
        return e_COLOR::DEFAULT;
    }
}

bool Dobot::getInfraredSensor()
{
    uint8_t value;

    while (GetInfraredSensor(dobotId, infraredPort, &value) != DobotCommunicate_NoError) {
    }

    return value == 1;
}

void Dobot::enableEMotor()
{
    eMotor.isEnabled = true;

    setEMotor();
}

void Dobot::disableEMotor()
{
    eMotor.isEnabled = false;

    setEMotor();
}

void Dobot::setEMotorIndex(uint8_t index)
{
    eMotor.index = index;
}

void Dobot::setEMotorSpeed(int32_t speed)
{
    eMotor.speed = speed;

    setEMotor();
}

void Dobot::setEMotor()
{
    while (SetEMotor(dobotId, &eMotor, true, NULL) != DobotCommunicate_NoError) {
    }
}

const char *Dobot::getPortName() const
{
    return portName;
}

void Dobot::setPortName(const char *newPortName)
{
    portName = newPortName;
}

std::string Dobot::getSerialNumber() const
{
    return std::string(deviceSN);
}

std::string Dobot::getName() const
{
    return std::string(deviceSN);
}

void Dobot::goHome()
{
    HOMECmd homeCmd;
    homeCmd.reserved = true;
    while (SetHOMECmd(dobotId, &homeCmd, true, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::setHome(s_POSITION position)
{
    HOMEParams homeParams;
    homeParams.x = position.X;
    homeParams.y = position.Y;
    homeParams.z = position.Z;
    homeParams.r = position.R;

    while (SetHOMEParams(dobotId, &homeParams, true, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::enableSuctionCup()
{
    while (SetEndEffectorSuctionCup(dobotId, true, true, true, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::disableSuctionCup()
{
    while (SetEndEffectorSuctionCup(dobotId, true, false, true, NULL) != DobotCommunicate_NoError) {
    }
}


