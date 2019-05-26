#include <gui/config_screen/ConfigView.hpp>

ConfigView::ConfigView()
{

}

void ConfigView::setupScreen()
{
    ConfigViewBase::setupScreen();
}

void ConfigView::tearDownScreen()
{
    ConfigViewBase::tearDownScreen();
}

void ConfigView::updateSerialMode(){
    //setMode(3);
    presenter->setSerialMode(toggleSerialMode.getState() == false ? 1 : 0 );
}