#include <gui/main_screen/MainView.hpp>

MainView::MainView()
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
    
    gauge.setBitmaps(Bitmap(BITMAP_DIAL_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    gauge.setXY(250,187);
    gauge.setLimits(0, 160, 270, 270 + 180);
    gauge.setValue(0);
    gauge.setAnimationDuration(20);
    gauge.setVisible(true);
    add(gauge);
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
    gauge.setValue(value);
}