#include "Deck.h"
#include "Card.h"
#include <iostream>
#include <random>
#include <cassert>
#include <algorithm>

Deck::Deck()
{
    Index index{0};
    for (int suit{0}; suit < static_cast<int>(Card::max_suits); ++suit)
    {
        for (int rank{0}; rank < static_cast<int>(Card::max_ranks); ++rank)
        {
            m_deck[index] = Card{static_cast<Card::Rank>(rank),
                                 static_cast<Card::Suit>(suit)};
            ++index;
        }
    }
}

void Deck::print() const
{
    for (const auto &card : m_deck)
    {
        card.print();
        std::cout << ' ';
    }

    std::cout << '\n';
}

void Deck::shuffle()
{
    // create a rng using a seed comprised of 8 random_device calls
    static std::random_device rd;
    static std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    static std::mt19937 mt{ss};
    std::shuffle(m_deck.begin(), m_deck.end(), mt);
    m_cardIndex = 0;
}

const Card &Deck::dealCard()
{
    assert(m_cardIndex < m_deck.size() && "Ran out of cards to draw!");
    return m_deck[m_cardIndex++];
}
