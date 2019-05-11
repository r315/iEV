#ifndef CONFIG_VIEW_HPP
#define CONFIG_VIEW_HPP

#include <gui_generated/config_screen/ConfigViewBase.hpp>
#include <gui/config_screen/ConfigPresenter.hpp>

class ConfigView : public ConfigViewBase
{
public:
    ConfigView();
    virtual ~ConfigView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CONFIG_VIEW_HPP
