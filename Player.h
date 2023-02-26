#ifndef PLAYER_H
#define PLAYER_H
#include "Card.h"
#include "Deck.h"

class Player
{
private:
    int m_score{0};

public:
    Player();

    const Card &drawCard(Deck &deck);

    int score() const;

    bool isBust() const;
};

#endif
