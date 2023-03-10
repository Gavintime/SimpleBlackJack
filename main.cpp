#include "Deck.h"
#include "Player.h"
#include "Config.h"
#include <iostream>
#include <algorithm> // max

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
    while (dealer.score() < std::max(Config::minimumDealerScore, player.score()))
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
