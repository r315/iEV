#ifndef MAIN_PRESENTER_HPP
#define MAIN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainView;

class MainPresenter : public Presenter, public ModelListener
{
private:
    MainPresenter();
    MainView& view;

public:
    MainPresenter(MainView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MainPresenter() {};

    void notifyRpmChange(int value);
    void notifySpeedChange(int value);
    void notifyDistanceChange(double value);
    void notifyBatteryChange(int percentage);
    void notifyMotorTempChange(int temp);
    void notifyControllerTempChange(int temp);
    void notifyMotorCurrentChange(int temp);
};


#endif // MAIN_PRESENTER_HPP
