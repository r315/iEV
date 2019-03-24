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

void MainView::testValues(int value){
    Unicode::snprintf(TextAreaSpeedBuffer, 4, "%d", value);
    // Invalidate text area, which will result in it being redrawn in next tick.
    //TextAreaSpeedBuffer[0] = 31;
    //TextAreaSpeedBuffer[1] = 31;
    //TextAreaSpeedBuffer[2] = 31;
    //TextAreaSpeed.resizeToCurrentText(); 
    TextAreaSpeed.invalidate();
     Unicode::snprintf(TextAreaDistanceBuffer, 7, "%06d", value *999);
    TextAreaDistance.invalidate();
}

void MainView::setRpm(int rpm){
    Unicode::snprintf(TextAreaRpmBuffer, 6, "%d", rpm);
    TextAreaRpm.invalidate();
}