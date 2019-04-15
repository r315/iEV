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
    void setDistance(int value);
    void setRpm(int value);
    void setSpeed(int value);
protected:

    Gauge gauge;
};

#endif // MAIN_VIEW_HPP
