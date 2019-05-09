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

    batteryGaugeMaxHeight = batteryGaugeContainer.getHeight();
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

void MainView::setBatteryLevel(int value){
    if(value > 100 || value < 0) 
        return;
        
    // Divide gauge in 10 sections
    uint16_t section = batteryGaugeMaxHeight / 10;
    
    value = value / 10;
    
    value = (value == 0) ? section : section * value;   // always has at least one bar

    batteryGauge.setY(-value);

    batteryGauge.invalidate();    
}