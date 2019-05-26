#ifndef CONFIG_PRESENTER_HPP
#define CONFIG_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ConfigView;

class ConfigPresenter : public Presenter, public ModelListener
{
public:
    ConfigPresenter(ConfigView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ConfigPresenter() {};
    void setSerialMode(uint8_t mode);
private:
    ConfigPresenter();

    ConfigView& view;
};


#endif // CONFIG_PRESENTER_HPP
