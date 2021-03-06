#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <gui/main_screen/Gauge.hpp>

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void setDistance(double value);
    void setRpm(int value);
    void setSpeed(int value);
    void setBatteryLevel(int value);
    void setMotorTemp(int temp);
    void setControllerTemp(int temp);
    void setMotorCurrent(int temp);
    void setBatteryVoltage(double voltage);
protected:
    uint16_t batteryGaugeMaxHeight;
    Gauge dialKph, dialRpm;
};

#endif // MAIN_VIEW_HPP
