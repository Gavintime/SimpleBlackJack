#include "Card.h"
#include <iostream>
#include <array>
#include <algorithm> // shuffle, max
#include <random>    // mt19937, seed_seq, random_device
#include <cassert>

// Maximum score before losing.
constexpr int g_maximumScore{21};

// Minimum score that the dealer has to have.
constexpr int g_minimumDealerScore{17};

class Deck
{
public:
    using DeckType = std::array<Card, 52>;
    using Index = DeckType::size_type;

private:
    DeckType m_deck{};
    Index m_cardIndex{0};

public:
    Deck()
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

    void print() const
    {
        for (const auto &card : m_deck)
        {
            card.print();
            std::cout << ' ';
        }

        std::cout << '\n';
    }

    void shuffle()
    {
        // create a rng using a seed comprised of 8 random_device calls
        static std::random_device rd;
        static std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        static std::mt19937 mt{ss};
        std::shuffle(m_deck.begin(), m_deck.end(), mt);
        m_cardIndex = 0;
    }

    const Card &dealCard()
    {
        assert(m_cardIndex < m_deck.size() && "Ran out of cards to draw!");
        return m_deck[m_cardIndex++];
    }
};

class Player
{
private:
    int m_score{0};

public:
    Player() = default;

    const Card &drawCard(Deck &deck)
    {
        const Card &card{deck.dealCard()};
        m_score += card.value();
        return card;
    }

    int score() const
    {
        return m_score;
    }

    bool isBust() const
    {
        return m_score > g_maximumScore;
    }
};

bool playerWantsHit()
{
    while (true)
    {
        std::cout << "(h) to hit, or (s) to stand: ";

        char ch{};
        std::cin >> ch;

        switch (ch)
        {
        case 'h':
            return true;
        case 's':
            return false;
        }
    }
}

// Returns true if the player went bust. False otherwise.
bool playerTurn(Deck &deck, Player &player)
{
    while (true)
    {
        if (player.isBust())
        {
            // This can happen even before the player had a choice if they drew 2
            // aces.
            std::cout << "You busted!\n";
            return true;
        }

        if (!playerWantsHit())
        {
            // The player didn't go bust and doesn't want to hit
            return false;
        }

        // didn't bust, wants to hit
        std::cout << "You draw a ";
        player.drawCard(deck).print();
        std::cout << '\n';
        std::cout << "You now have " << player.score() << '\n';
    }
}

// Returns true if the dealer went bust. False otherwise.
bool dealerTurn(Deck &deck, Player &dealer, const Player &player)
{
    // Draw cards until we reach the minimum value or beat's the players score
    while (dealer.score() < std::max(g_minimumDealerScore, player.score()))
    {
        std::cout << "The dealer draws a ";
        dealer.drawCard(deck).print();
        std::cout << '\n';
        std::cout << "The dealer now has " << dealer.score() << '\n';

        // If the dealer's score is too high, they went bust.
        if (dealer.isBust())
        {
            std::cout << "The dealer busted!\n";
            return true;
        }
    }

    return false;
}

bool playBlackjack(Deck &deck)
{
    // Create the dealer and give them 1 card.
    Player dealer{};
    std::cout << "The dealer is showing: " << dealer.drawCard(deck).value() << '\n';

    // Create the player and give them 2 cards.
    Player player{};
    std::cout << "You have: " << player.drawCard(deck).value() + player.drawCard(deck).value() << '\n';

    if (playerTurn(deck, player))
    {
        // The player went bust, the dealer wins.
        return false;
    }

    if (dealerTurn(deck, dealer, player))
    {
        // The dealer went bust, the player wins.
        return true;
    }

    return (player.score() > dealer.score());
}

int main()
{
    Deck deck{};
    deck.shuffle();
    if (playBlackjack(deck))
    {
        std::cout << "You win!\n";
    }
    else
    {
        std::cout << "You lose!\n";
    }
}
