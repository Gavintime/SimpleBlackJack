#include "Player.h"
#include "Card.h"
#include "Deck.h"
#include "Config.h"

Player::Player() = default;

const Card &Player::drawCard(Deck &deck)
{
    const Card &card{deck.dealCard()};
    m_score += card.value();
    return card;
}

int Player::score() const
{
    return m_score;
}

bool Player::isBust() const
{
    return m_score > Config::maximumScore;
}
