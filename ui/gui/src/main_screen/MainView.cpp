#include <gui/main_screen/MainView.hpp>


#define BITMAP_DIAL_KPH_LOC 440, 160
#define BITMAP_DIAL_RPM_LOC 0, 160

MainView::MainView()
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
    
    //gauge.setBitmaps(Bitmap(BITMAP_DIAL_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialKph.setBitmaps(Bitmap(BITMAP_DIAL_KPH_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialKph.setXY(BITMAP_DIAL_KPH_LOC);
    dialKph.setLimits(0, 180, 247, 247 + 205);
    dialKph.setValue(180);
    dialKph.setAnimationDuration(20);
    dialKph.setVisible(true);   

     //gauge.setBitmaps(Bitmap(BITMAP_DIAL_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialRpm.setBitmaps(Bitmap(BITMAP_DIAL_RPM_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialRpm.setXY(BITMAP_DIAL_RPM_LOC);
    dialRpm.setLimits(0, 10000, 247, 247 + 225);
    dialRpm.setValue(0);
    dialRpm.setAnimationDuration(20);
    dialRpm.setVisible(true);

    remove(TextAreaDistance);
    add(dialKph);
    add(dialRpm);

    TextAreaDistance.setXY(dialKph.getX() + 100, dialKph.getY() + 240);
    add(TextAreaDistance);

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
    //Unicode::snprintf(TextAreaRpmBuffer, 6, "%d", value);
    //TextAreaRpm.invalidate();
    dialRpm.setValue(value);
}


void MainView::setSpeed(int value){
    dialKph.setValue(value);
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