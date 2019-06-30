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
    dialKph.setValue(0);
    dialKph.setAnimationDuration(20);
    dialKph.setVisible(true);

     //gauge.setBitmaps(Bitmap(BITMAP_DIAL_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialRpm.setBitmaps(Bitmap(BITMAP_DIAL_RPM_ID), Bitmap(BITMAP_DIAL_CENTER_ID));
    dialRpm.setXY(BITMAP_DIAL_RPM_LOC);
    dialRpm.setLimits(0, 10000, 247, 247 + 225);
    dialRpm.setValue(0);
    dialRpm.setAnimationDuration(20);
    dialRpm.setVisible(true);

    remove(Hodometer);
    add(dialKph);
    add(dialRpm);
    // Add hodometer on top of the dial
    add(Hodometer);

    batteryGaugeMaxHeight = batteryGaugeContainer.getHeight();    
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::setDistance(double value){
    Unicode::snprintf(KilometersBuffer, KILOMETERS_SIZE, "%06d", (int)value);
    Unicode::snprintf(TenthsKilometerBuffer, TENTHSKILOMETER_SIZE, "%d", (int)((value*10))%10);
    Kilometers.invalidate();
    TenthsKilometer.invalidate();
}

void MainView::setRpm(int value){
    //Unicode::snprintf(TextAreaRpmBuffer, 6, "%d", value);
    //TextAreaRpm.invalidate();
    dialRpm.setValue(value);
}


void MainView::setSpeed(int value){
    dialKph.setValue(value);
}

void MainView::setBatteryVoltage(double voltage){
    Unicode::snprintf(BatteryVoltageBuffer, BATTERYVOLTAGE_SIZE, "%d.%d", (int)voltage, (int)((voltage*10))%10);    
    BatteryVoltage.invalidate();    
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

void MainView::setMotorTemp(int temp){
    Unicode::snprintf(MotorTempBuffer, MOTORTEMP_SIZE, "%d", temp);
    MotorTemp.invalidate();
}

void MainView::setControllerTemp(int temp){
   Unicode::snprintf(ControllerTempBuffer, CONTROLLERTEMP_SIZE, "%d", temp);
    ControllerTemp.invalidate(); 
}

void MainView::setMotorCurrent(int current){
    Unicode::snprintf(MotorCurrentBuffer, MOTORCURRENT_SIZE, "%d", current);
    MotorCurrent.invalidate();
}