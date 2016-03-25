#include "number_sprite.h"
#include "sprite.h"

namespace {

    const std::string kSpritePath = "content/TextBox.pbm";
    const units::Game kSourceX = units::kHalfTile;
    const units::Game kSourceY = 7 * units::kHalfTile;
    const units::Game kSourceWidth = units::kHalfTile;
    const units::Game kSourceHeight = units::kHalfTile;
}

//num digits will ensure we're right justified
NumberSprite::NumberSprite(Graphics& graphics, int number, int num_digits)
{
    padding_ = 0.0f;
    assert(number >= 0); //if not, crash
    int digit_count = 0;
    do
    {
        const int digit = number % 10; //number of digits 
        reverse_digits_.push_back(std::shared_ptr<Sprite>(new Sprite(graphics, kSpritePath,
            units::gameToPixel(digit * kSourceX), units::gameToPixel(kSourceY), 
            units::gameToPixel(kSourceHeight), units::gameToPixel(kSourceWidth)
        )));
        number /= 10; //divide by 10 to get the other numbers
        ++digit_count;
    } while (number != 0);
    
    //if num_digits = 0, no shifting.
    //if num_digits = the amount of digits, no shifting.
    assert(num_digits == 0 || num_digits >= digit_count); //so we don't end up with garbled digits.
    padding_ = num_digits == 0 ? 0.0f : units::kHalfTile * (num_digits - digit_count);
}

void NumberSprite::draw(Graphics& graphics, units::Game x, units::Game y)
{
    for (size_t i = 0; i < reverse_digits_.size(); i++)
    {
        //i = size of the vector - 1, offset is 0
        //0 = size of vector - 1 - i, offset = 0
        //                         size of digit
        const units::Game offset = units::kHalfTile * (reverse_digits_.size() - 1 - i);
        reverse_digits_[i]->draw(graphics, x + offset + padding_, y);
    }
}