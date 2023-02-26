#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <array>

class Deck
{
public:
    using DeckType = std::array<Card, 52>;
    using Index = DeckType::size_type;

private:
    DeckType m_deck{};
    Index m_cardIndex{0};

public:
    Deck();

    void print() const;

    void shuffle();

    const Card &dealCard();
};

#endif
