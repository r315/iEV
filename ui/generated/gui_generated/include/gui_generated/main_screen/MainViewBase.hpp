/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAIN_VIEW_BASE_HPP
#define MAIN_VIEW_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/Container.hpp>

#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
class MainViewBase : public touchgfx::View<MainPresenter>
{
public:
    MainViewBase();
    virtual ~MainViewBase() {}

    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Image fon_411111;
    touchgfx::BoxWithBorder boxWithBorder_rpm;
    touchgfx::TextArea textArea_rpm_unit;
    touchgfx::TextArea TextArea_km_unit;
    touchgfx::TextAreaWithOneWildcard TextAreaRpm;
    touchgfx::TextAreaWithOneWildcard TextAreaDistance;
    touchgfx::Container batteryGaugeContainer;
    touchgfx::Image batteryGauge;


    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREARPM_SIZE = 7;
    touchgfx::Unicode::UnicodeChar TextAreaRpmBuffer[TEXTAREARPM_SIZE];
    static const uint16_t TEXTAREADISTANCE_SIZE = 7;
    touchgfx::Unicode::UnicodeChar TextAreaDistanceBuffer[TEXTAREADISTANCE_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

};

#endif // MAIN_VIEW_BASE_HPP
