
#include "commandConcrete.h"
#include "receiverConcrete.h"
#include "invoker.h"

void SimpleRemoteControlTest()
{
    // invoker 调用对象
    SimpleRemoteControl *remote = new SimpleRemoteControl();
    // recevier 执行对象
    Light *light = new Light("Hall");
    // command 命令对象(请求以命令的形式包裹在对象中)
    LightOnCommand *lightOn = new LightOnCommand(light);

    // 参数化
    remote->setCommand(lightOn);
    // 具体调用
    remote->buttonWasPressed();
}

void RemoteControlTest()
{
    // invoker 调用对象
    RemoteControl *remote = new RemoteControl(7);   

    // receiver 执行对象
    Light *livingRoomLight = new Light("livingRoom");   
    Light *kitchenLight = new Light("kitchen");
    Aircon *hallAircon = new Aircon("hall");
    Aircon *livingRoomAircon = new Aircon("livingRoom");
    Stereo *stereo = new Stereo();
    TV *tv = new TV();

    // command 命令对象
    ICommand *livingRoomLightOn = new LightOnCommand(livingRoomLight);
    ICommand *livingRoomLightOff = new LightOffCommand(livingRoomLight);
    ICommand *kitchenLightOn = new LightOnCommand(kitchenLight);
    ICommand *kitchendLightOff = new LightOffCommand(kitchenLight);

    ICommand *hallAirconOn = new AirconOnCommand(hallAircon);
    ICommand *hallAirconOff = new AirconOffCommand(hallAircon);
    ICommand *livingRoomAirconOn = new AirconOnCommand(livingRoomAircon);
    ICommand *livingRoomAirconOff = new AirconOffCommand(livingRoomAircon);

    ICommand *stereoOn = new StereoOnCommand(stereo);
    ICommand *stereoOff = new StereoOffCommand(stereo);

    ICommand *tvOn = new TVOnCommand(tv);
    ICommand *tvOff = new TVOffCommand(tv);

    // 调用关系设置 参数化
    remote->setCommand(0, livingRoomLightOn, livingRoomLightOff);
    remote->setCommand(1, kitchenLightOn, kitchendLightOff);
    remote->setCommand(2, hallAirconOn, hallAirconOff);
    remote->setCommand(3, livingRoomAirconOn, livingRoomAirconOff);
    remote->setCommand(4, stereoOn, stereoOff);
    remote->setCommand(5, tvOn, tvOff);

    // 具体的调用
    for (int i = 0; i <= 5; ++i)
    {
        remote->onButtonWasPushed(i);
        remote->offButtonWasPushed(i);
    }
    remote->onButtonWasPushed(2);
    remote->onButtonWasPushed(5);
    // 撤销上一步调用
    remote->undoButtonWasPushed();
}

int main(int argc, char *argv[])
{
    // SimpleRemoteControlTest();
    RemoteControlTest();
    return 0;
}
