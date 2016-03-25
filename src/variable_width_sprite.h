#ifndef VARIABLE_WIDTH_H_
#define VARIABLE_WIDTH_H_

#include "sprite.h"

struct VariableWidthSprite : public Sprite
{
    VariableWidthSprite(Graphics& graphics, const std::string& file_path,
        units::Pixel source_x, units::Pixel source_y, units::Pixel initial_width, units::Pixel height)
        : Sprite(graphics, file_path, source_x, source_y, initial_width, height)
    {};

    void setWidth(units::Pixel width)
    {
        source_rect_.w = width;
    };
};

#endif //VARIABLE_WIDTH_H_