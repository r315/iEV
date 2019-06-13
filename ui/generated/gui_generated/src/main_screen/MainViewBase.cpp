/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/main_screen/MainViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MainViewBase::MainViewBase() :
    buttonCallback(this, &MainViewBase::buttonCallbackHandler)
{
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    Main_BG.setXY(0, 0);
    Main_BG.setBitmap(Bitmap(BITMAP_FON_41111_ID));

    GoToConfig.setXY(758, 31);
    GoToConfig.setBitmaps(Bitmap(BITMAP_DARK_ICONS_NEXT_ARROW_48_ID), Bitmap(BITMAP_DARK_ICONS_NEXT_ARROW_48_ID));
    GoToConfig.setAction(buttonCallback);

    batteryGaugeContainer.setPosition(360, 150, 80, 300);

    batteryGauge.setXY(-20, -150);
    batteryGauge.setBitmap(Bitmap(BITMAP_BATTERY_GAUGE_ID));
    batteryGaugeContainer.add(batteryGauge);

    TempContainer.setPosition(40, 50, 300, 80);

    EngineTemp_BG.setPosition(0, 0, 300, 80);
    EngineTemp_BG.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    EngineTemp_BG.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(77, 75, 75));
    EngineTemp_BG.setBorderSize(5);
    TempContainer.add(EngineTemp_BG);

    MotorTempIcon.setXY(100, 6);
    MotorTempIcon.setBitmap(Bitmap(BITMAP_TEMP_ID));
    TempContainer.add(MotorTempIcon);

    MotorTemp.setXY(6, 16);
    MotorTemp.setColor(touchgfx::Color::getColorFrom24BitRGB(148, 40, 40));
    MotorTemp.setLinespacing(0);
    Unicode::snprintf(MotorTempBuffer, MOTORTEMP_SIZE, "%s", TypedText(T_SINGLEUSEID10).getText());
    MotorTemp.setWildcard(MotorTempBuffer);
    MotorTemp.resizeToCurrentText();
    MotorTemp.setTypedText(TypedText(T_ENGINETEMP));
    TempContainer.add(MotorTemp);

    ControllerTemp.setXY(150, 16);
    ControllerTemp.setColor(touchgfx::Color::getColorFrom24BitRGB(148, 40, 40));
    ControllerTemp.setLinespacing(0);
    Unicode::snprintf(ControllerTempBuffer, CONTROLLERTEMP_SIZE, "%s", TypedText(T_SINGLEUSEID11).getText());
    ControllerTemp.setWildcard(ControllerTempBuffer);
    ControllerTemp.resizeToCurrentText();
    ControllerTemp.setTypedText(TypedText(T_CONTROLLERTEMP));
    TempContainer.add(ControllerTemp);

    ControllerTempIcon.setXY(248, 6);
    ControllerTempIcon.setBitmap(Bitmap(BITMAP_TEMP_C_ID));
    TempContainer.add(ControllerTempIcon);

    CurrentContainer.setPosition(480, 50, 260, 80);

    CurrentConsume_BG.setPosition(0, 0, 260, 80);
    CurrentConsume_BG.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    CurrentConsume_BG.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(77, 75, 75));
    CurrentConsume_BG.setBorderSize(5);
    CurrentContainer.add(CurrentConsume_BG);

    MotorCurrent.setPosition(0, 15, 260, 50);
    MotorCurrent.setColor(touchgfx::Color::getColorFrom24BitRGB(148, 40, 40));
    MotorCurrent.setLinespacing(0);
    Unicode::snprintf(MotorCurrentBuffer, MOTORCURRENT_SIZE, "%s", TypedText(T_SINGLEUSEID12).getText());
    MotorCurrent.setWildcard(MotorCurrentBuffer);
    MotorCurrent.setTypedText(TypedText(T_MOTORCURRENT));
    CurrentContainer.add(MotorCurrent);

    BatIconContainer.setPosition(360, 150, 80, 32);

    BatIcon_BG.setPosition(0, 0, 80, 32);
    BatIcon_BG.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    BatIcon_BG.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(25, 69, 67));
    BatIcon_BG.setBorderSize(1);
    BatIconContainer.add(BatIcon_BG);

    BatIcon.setXY(13, 2);
    BatIcon.setBitmap(Bitmap(BITMAP_BATTERY_ICON_ID));
    BatIconContainer.add(BatIcon);

    Hodometer.setPosition(534, 400, 185, 50);

    Kilometers.setPosition(0, 0, 153, 50);
    Kilometers.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 173, 217));
    Kilometers.setLinespacing(0);
    Unicode::snprintf(KilometersBuffer, KILOMETERS_SIZE, "%s", TypedText(T_SINGLEUSEID7).getText());
    Kilometers.setWildcard(KilometersBuffer);
    Kilometers.setTypedText(TypedText(T_TOTALDISTANCE));
    Hodometer.add(Kilometers);

    TenthsKilometer.setXY(152, 0);
    TenthsKilometer.setColor(touchgfx::Color::getColorFrom24BitRGB(212, 121, 6));
    TenthsKilometer.setLinespacing(0);
    Unicode::snprintf(TenthsKilometerBuffer, TENTHSKILOMETER_SIZE, "%s", TypedText(T_SINGLEUSEID13).getText());
    TenthsKilometer.setWildcard(TenthsKilometerBuffer);
    TenthsKilometer.resizeToCurrentText();
    TenthsKilometer.setTypedText(TypedText(T_PARTIALDISTANCE));
    Hodometer.add(TenthsKilometer);

    add(Main_BG);
    add(GoToConfig);
    add(batteryGaugeContainer);
    add(TempContainer);
    add(CurrentContainer);
    add(BatIconContainer);
    add(Hodometer);
}

void MainViewBase::setupScreen()
{

}

void MainViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &GoToConfig)
    {
        //Interaction1
        //When GoToConfig clicked change screen to Config
        //Go to Config with screen transition towards East
        application().gotoConfigScreenSlideTransitionEast();
    }
}
