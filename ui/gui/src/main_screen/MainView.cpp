#include <gui/main_screen/MainView.hpp>

MainView::MainView()
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::setDistance(int value){
     Unicode::snprintf(TextAreaDistanceBuffer, 7, "%06d", value);
    TextAreaDistance.invalidate();
}

void MainView::setRpm(int value){
    Unicode::snprintf(TextAreaRpmBuffer, 6, "%d", value);
    TextAreaRpm.invalidate();
}


void MainView::setSpeed(int value){
    Unicode::snprintf(TextAreaSpeedBuffer, 4, "%d", value);
    TextAreaSpeed.invalidate();
}