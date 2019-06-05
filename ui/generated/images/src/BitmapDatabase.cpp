// -alpha_dither yes -dither 2 -non_opaque_image_format ARGB8888 -opaque_image_format RGB888 0x56e1b210
// Generated by imageconverter. Please, do not edit!

#include <touchgfx/Bitmap.hpp>
#include <BitmapDatabase.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

extern const unsigned char _battery_gauge[];
extern const unsigned char _blue_togglebars_toggle_large_button_off[];
extern const unsigned char _blue_togglebars_toggle_large_button_on[];
extern const unsigned char _dark_icons_back_arrow_48[];
extern const unsigned char _dark_icons_next_arrow_48[];
extern const unsigned char _dial[];
extern const unsigned char _dial_center[];
extern const unsigned char _dial_kph[];
extern const unsigned char _dial_rpm[];
extern const unsigned char _fon_41111[];
extern const unsigned char _temp[];
extern const unsigned char _temp_c[];

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { _battery_gauge, 0, 120, 600, 0, 0, 120, 600, touchgfx::Bitmap::RGB888 },
    { _blue_togglebars_toggle_large_button_off, 0, 128, 38, 0, 0, 128, 38, touchgfx::Bitmap::RGB888 },
    { _blue_togglebars_toggle_large_button_on, 0, 128, 38, 0, 0, 128, 38, touchgfx::Bitmap::RGB888 },
    { _dark_icons_back_arrow_48, 0, 28, 48, 6, 16, 6, 14, touchgfx::Bitmap::ARGB8888 },
    { _dark_icons_next_arrow_48, 0, 28, 48, 16, 17, 7, 12, touchgfx::Bitmap::ARGB8888 },
    { _dial, 0, 527, 293, 75, 83, 377, 210, touchgfx::Bitmap::ARGB8888 },
    { _dial_center, 0, 50, 50, 8, 8, 34, 34, touchgfx::Bitmap::ARGB8888 },
    { _dial_kph, 0, 360, 360, 54, 53, 253, 252, touchgfx::Bitmap::ARGB8888 },
    { _dial_rpm, 0, 360, 360, 53, 53, 254, 253, touchgfx::Bitmap::ARGB8888 },
    { _fon_41111, 0, 800, 480, 0, 0, 800, 480, touchgfx::Bitmap::RGB888 },
    { _temp, 0, 53, 38, 25, 4, 3, 25, touchgfx::Bitmap::ARGB8888 },
    { _temp_c, 0, 48, 35, 0, 0, 0, 0, touchgfx::Bitmap::ARGB8888 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}
uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}

