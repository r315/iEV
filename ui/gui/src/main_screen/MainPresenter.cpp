#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{
}

void MainPresenter::activate()
{

}

void MainPresenter::deactivate()
{

}

void MainPresenter::notifyRpmChange(int value){ view.setRpm(value); }
void MainPresenter::notifySpeedChange(int value){ view.setSpeed(value); }
void MainPresenter::notifyDistanceChange(int value){ view.setDistance(value); }    
void MainPresenter::notifyBatteryChange(int percentage){ view.setBatteryLevel(percentage); }
void MainPresenter::notifyMotorTempChange(int temp){ view.setMotorTemp(temp); }
void MainPresenter::notifyControllerTempChange(int temp){ view.setControllerTemp(temp); }
void MainPresenter::notifyMotorCurrentChange(int temp){ view.setMotorCurrent(temp); }