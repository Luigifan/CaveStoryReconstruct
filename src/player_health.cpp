#include "player.h"

namespace
{
    const std::string kSpritePath = "content/TextBox.pbm";

    //Hud
    const units::Game kHealthBarX = units::tileToGame(1);
    const units::Game kHealthBarY = units::tileToGame(2);
    const units::Game kHealthFillX = 5 * units::kHalfTile;
    const units::Game kHealthFillY = units::tileToGame(2);

    const units::Game kHealthBarSourceX = 0;
    const units::Game kHealthBarSourceY = 5 * units::kHalfTile;
    const units::Game kHealthBarSourceWidth = units::tileToGame(4);
    const units::Game kHealthBarSourceHeight = units::kHalfTile;

    const units::Game kHealthFillSourceX = 0;
    const units::Game kHealthFillSourceY = 3 * units::kHalfTile;
    const units::Game kHealthBarMaxWidth = 5 * units::kHalfTile - 2;
    const units::Game kHealthBarFillHeight = units::kHalfTile;

    const units::Game kHealthNumberDrawX = units::tileToGame(3) / 2;
    const units::Game KHealthNumberDrawY = units::tileToGame(2);
    const units::Game kHealthNumberNumDigits = 2;

    const units::Game kHealthDamageSourceX = 0;
    const units::Game kHealthDamageSourceY = units::tileToGame(2);
    const units::Game kHealthDamageHeight = units::kHalfTile;
    //End HUD

    const units::MS kDamageDelay = 1500; //less than our invincibility time
}

Player::Health::Health(Graphics& graphics) : damage_(0), damage_time_(0),
    health_bar_sprite_(graphics, kSpritePath,
        units::gameToPixel(kHealthBarSourceX), units::gameToPixel(kHealthBarSourceY),
        units::gameToPixel(kHealthBarSourceWidth),
        units::gameToPixel(kHealthBarSourceHeight)
    ),
    health_fill_sprite_(graphics, kSpritePath,
        units::gameToPixel(kHealthFillSourceX), units::gameToPixel(kHealthFillSourceY),
        units::gameToPixel(kHealthBarMaxWidth),
        units::gameToPixel(kHealthBarFillHeight)
    ), 
    damage_fill_sprite_(graphics, kSpritePath, units::gameToPixel(kHealthDamageSourceX),
        units::gameToPixel(kHealthDamageSourceY),
        units::gameToPixel(0),
        units::gameToPixel(kHealthDamageHeight)),
    max_health_(6), current_health_(6)
{}

void Player::Health::draw(Graphics& graphics)
{
    //stack is regular
    //heap is pointers
    health_bar_sprite_.draw(graphics, kHealthBarX, kHealthBarY);
    health_fill_sprite_.draw(graphics, kHealthFillX, kHealthFillY);

    if (damage_ > 0)
        damage_fill_sprite_.draw(graphics, kHealthFillX + fillOffset(current_health_ - damage_), kHealthFillY);

    NumberSprite(graphics, current_health_, kHealthNumberNumDigits).draw(graphics, kHealthNumberDrawX, KHealthNumberDrawY);
}

units::Game Player::Health::fillOffset(units::HP health) const
{
    return kHealthBarMaxWidth * health / max_health_;
}

void Player::Health::update(units::MS elapsed_time)
{
    if (damage_ > 0)
    {
        damage_time_ += elapsed_time;
        if (damage_time_ > kDamageDelay)
        {
            current_health_ -= damage_;
            damage_ = 0;
        }
    }
}

bool Player::Health::takeDamage(units::HP damage)
{
    //wait until a certain amount has passed before we update our current health
    damage_ = damage;
    damage_time_ = 0;
    health_fill_sprite_.setWidth(units::gameToPixel(fillOffset(current_health_ - damage)));
    damage_fill_sprite_.setWidth(units::gameToPixel(fillOffset(damage)));
    return false;
}