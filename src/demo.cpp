#include "demo.h"
#include <iostream>
#include <thread>
#include "DobotDll.h"
#include "DobotType.h"


Demo::Demo()
{
    findDobotBySerialNumbers();

    armOnMat->setEMotorIndex(1);
    armTakingFromMat->initColorSensor(ColorPort::CL_PORT_GP1);
    armTakingFromMat->initInfraredSensor(InfraredPort::IF_PORT_GP1);
    armOnMat->setEMotorSpeed(0);
    armOnMat->enableEMotor();
}

Demo::~Demo()
{

}

void Demo::exec()
{
    while(true)
    {
        Pose armOnMatPos = armOnMat->getPose();
        std::cout << "armOnMat position, x : " << armOnMatPos.x << ", y : " << armOnMatPos.y << ", z : " << armOnMatPos.z << ", r : " <<  armOnMatPos.r << std::endl;


        Pose armTakingFromMattPos = armTakingFromMat->getPose();
        std::cout << "armOnMat position, x : " << armTakingFromMattPos.x << ", y : " << armTakingFromMattPos.y << ", z : " << armTakingFromMattPos.z << ", r : " <<  armTakingFromMattPos.r << std::endl;

        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(5000));
    }


    startMotor();

    while(true)
    {
        if(armTakingFromMat->getInfraredSensor() == true)
        {
            stopMotor();
            std::cout << "box detected" << std::endl;
            moveBoxToMat();
            moveBoxToSensor();
            moveBoxToCorrectPosition();
        }
    }
}

void Demo::startMotor()
{
    armOnMat->setEMotorSpeed(100);
}

void Demo::stopMotor()
{
    armOnMat->setEMotorSpeed(0);
}

void Demo::moveBox(Dobot::s_POSITION startingPos, Dobot::s_POSITION endingPos)
{
    armTakingFromMat->ptpJump(startingPos);
    armTakingFromMat->enableSuctionCup();
    armTakingFromMat->ptpJump(endingPos);
    armTakingFromMat->disableSuctionCup();
}

void Demo::moveBoxToMat()
{
    Dobot::s_POSITION boxTakePosition = BOX_TAKE_POSITION;
    boxTakePosition.Y = BOX_TAKE_POSITION.Y + BOX_SIDE_LEN * (int)(boxCount / 3);
    boxTakePosition.X = BOX_TAKE_POSITION.X + BOX_SIDE_LEN * (boxCount % 3);

    moveBox(boxTakePosition, BOX_DROP_MAT_POSITION);
    boxCount++;
}

void Demo::moveBoxToSensor()
{
    moveBox(BOX_TAKE_MAT_POSITION, SENSOR_POSITION);
}

void Demo::moveBoxToCorrectPosition()
{
    Dobot::s_POSITION dropPosition;
    switch (armTakingFromMat->getColorSensor()) {
    case Dobot::e_COLOR::R:
        dropPosition = BOX_DROP_POSITION;
        break;
    case Dobot::e_COLOR::G:
        dropPosition = BOX_DROP_POSITION;
        dropPosition.Y = BOX_DROP_POSITION.Y + BOX_SIDE_LEN;
        break;
    case Dobot::e_COLOR::B:
        dropPosition = BOX_DROP_POSITION;
        dropPosition.Y = BOX_DROP_POSITION.Y + BOX_SIDE_LEN * 2;
        break;
    default:
        break;
    }

    moveBox(SENSOR_POSITION, dropPosition);
}


void Demo::findDobotBySerialNumbers()
{
    char portsBuffer[100] = {0};
    SearchDobot(portsBuffer, 100);
    QStringList comList = QString(portsBuffer).split(" ");
    comList.removeAll("");

    if (comList.isEmpty()) {
        std::cout << "No Dobot found." << std::endl;
        return;
    }

    for (const QString& comPort : comList) {
        Dobot currentDobot(comPort.toStdString().c_str());

        currentDobot.connect();

        std::string serialNumber = currentDobot.getSerialNumber();

        std::cout << serialNumber << std::endl;

        if (serialNumber == serialNumberArmOnMat && !armOnMat) {

            armOnMat = std::make_unique<Dobot>(std::move(currentDobot));
            std::cout << "Arm on mat connected with serial number " << serialNumber
                      << " on port " << comPort.toStdString() << std::endl;

        } else if (serialNumber == serialNumberArmTakingFromMat && !armTakingFromMat) {

            armTakingFromMat = std::make_unique<Dobot>(std::move(currentDobot));
            std::cout << "Arm taking from mat connected with serial number " << serialNumber
                      << " on port " << comPort.toStdString() << std::endl;

        } else {
            currentDobot.disconnect();
        }
    }
}
