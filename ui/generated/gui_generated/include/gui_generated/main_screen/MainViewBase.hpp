/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAIN_VIEW_BASE_HPP
#define MAIN_VIEW_BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

class MainViewBase : public touchgfx::View<MainPresenter>
{
public:
    MainViewBase();
    virtual ~MainViewBase() {}

    virtual void setupScreen();

    /*
     * Custom Action Handlers
     */
    virtual void testValues(int value)
    {
        // Override and implement this function in MainView
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box Background;
    touchgfx::BoxWithBorder boxWithBorder_rpm;
    touchgfx::TextArea textArea_rpm_unit;
    touchgfx::BoxProgress boxRpm;
    touchgfx::TextArea textArea1_rpm_unit;
    touchgfx::BoxWithBorder boxWithBorder_km;
    touchgfx::TextArea TextArea_km_unit;
    touchgfx::Slider slider1;
    touchgfx::TextAreaWithOneWildcard TextAreaSpeed;
    touchgfx::TextAreaWithOneWildcard TextAreaRpm;
    touchgfx::TextAreaWithOneWildcard TextAreaDistance;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREASPEED_SIZE = 4;
    touchgfx::Unicode::UnicodeChar TextAreaSpeedBuffer[TEXTAREASPEED_SIZE];
    static const uint16_t TEXTAREARPM_SIZE = 7;
    touchgfx::Unicode::UnicodeChar TextAreaRpmBuffer[TEXTAREARPM_SIZE];
    static const uint16_t TEXTAREADISTANCE_SIZE = 7;
    touchgfx::Unicode::UnicodeChar TextAreaDistanceBuffer[TEXTAREADISTANCE_SIZE];

private:

    /*
     * Callback Handler Declarations
     */
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);

    /*
     * Callback Declarations
     */
    touchgfx::Callback<MainViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

};

#endif // MAIN_VIEW_BASE_HPP
