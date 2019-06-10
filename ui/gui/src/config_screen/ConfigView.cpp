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
    presenter->setSerialMode(ModeSelector.getState() == false ? 0 : 1 );
}