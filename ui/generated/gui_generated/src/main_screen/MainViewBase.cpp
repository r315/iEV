/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/main_screen/MainViewBase.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MainViewBase::MainViewBase()
{
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    fon_411111.setXY(0, 0);
    fon_411111.setBitmap(Bitmap(BITMAP_FON_41111_ID));

    boxWithBorder_rpm.setPosition(316, 51, 358, 106);
    boxWithBorder_rpm.setColor(touchgfx::Color::getColorFrom24BitRGB(56, 56, 56));
    boxWithBorder_rpm.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    boxWithBorder_rpm.setBorderSize(5);

    textArea_rpm_unit.setXY(627, 51);
    textArea_rpm_unit.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 219, 209));
    textArea_rpm_unit.setLinespacing(0);
    textArea_rpm_unit.setTypedText(TypedText(T_SINGLEUSEID1));

    TextArea_km_unit.setXY(181, 436);
    TextArea_km_unit.setColor(touchgfx::Color::getColorFrom24BitRGB(207, 219, 209));
    TextArea_km_unit.setLinespacing(0);
    TextArea_km_unit.setTypedText(TypedText(T_SINGLEUSEID4));

    TextAreaRpm.setPosition(324, 40, 311, 117);
    TextAreaRpm.setColor(touchgfx::Color::getColorFrom24BitRGB(17, 194, 32));
    TextAreaRpm.setLinespacing(0);
    Unicode::snprintf(TextAreaRpmBuffer, TEXTAREARPM_SIZE, "%s", TypedText(T_SINGLEUSEID6).getText());
    TextAreaRpm.setWildcard(TextAreaRpmBuffer);
    TextAreaRpm.setTypedText(TypedText(T_CURRENTRPM));

    TextAreaDistance.setPosition(22, 417, 153, 49);
    TextAreaDistance.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 173, 217));
    TextAreaDistance.setLinespacing(0);
    Unicode::snprintf(TextAreaDistanceBuffer, TEXTAREADISTANCE_SIZE, "%s", TypedText(T_SINGLEUSEID7).getText());
    TextAreaDistance.setWildcard(TextAreaDistanceBuffer);
    TextAreaDistance.setTypedText(TypedText(T_TOTALDISTANCE));

    add(fon_411111);
    add(boxWithBorder_rpm);
    add(textArea_rpm_unit);
    add(TextArea_km_unit);
    add(TextAreaRpm);
    add(TextAreaDistance);
}

void MainViewBase::setupScreen()
{

}
