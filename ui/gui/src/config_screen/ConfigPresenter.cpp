#include <gui/config_screen/ConfigView.hpp>
#include <gui/config_screen/ConfigPresenter.hpp>

ConfigPresenter::ConfigPresenter(ConfigView& v)
    : view(v)
{
}

void ConfigPresenter::activate()
{

}

void ConfigPresenter::deactivate()
{

}

void ConfigPresenter::setSerialMode(uint8_t mode){
    model->userSetSerialMode(mode);
}
