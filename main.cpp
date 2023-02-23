#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <cassert>

enum class Rank
{
    Ace,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    Jack,
    Queen,
    King,

    max_ranks
};

enum class Suit
{
    club,
    diamond,
    heart,
    spade,

    max_suits
};

struct Card
{
    Rank rank{};
    Suit suit{};
};

using Deck = std::array<Card, 52>;
using CardIndex = Deck::size_type;

constexpr int g_dealerMaxScore{17};
constexpr int g_maxHandScore{21};

void printCard(const Card &card)
{

    switch (card.rank)
    {
    case Rank::Ace:
        std::cout << 'A';
        break;
    case Rank::Jack:
        std::cout << 'J';
        break;
    case Rank::Queen:
        std::cout << 'Q';
        break;
    case Rank::King:
        std::cout << 'K';
        break;
    default:
        std::cout << static_cast<int>(card.rank) + 1;
        break;
    }

    switch (card.suit)
    {
    case Suit::club:
        std::cout << 'C';
        break;
    case Suit::diamond:
        std::cout << 'D';
        break;
    case Suit::heart:
        std::cout << 'H';
        break;
    case Suit::spade:
        std::cout << 'S';
        break;
    // required for max_suits
    default:
        assert(false && "impossible card suit");
        break;
    }
}

void printDeck(const Deck &deck)
{
    for (const auto &card : deck)
    {
        printCard(card);
        std::cout << ' ';
    }
    std::cout << '\n';
}

Deck createDeck()
{
    Deck deck{};

    CardIndex cardIndex{0};
    for (int s{0}; s < static_cast<int>(Suit::max_suits); ++s)
    {
        for (int r{0}; r < static_cast<int>(Rank::max_ranks); ++r)
        {
            deck[cardIndex].rank = static_cast<Rank>(r);
            deck[cardIndex].suit = static_cast<Suit>(s);
            ++cardIndex;
        }
    }
    return deck;
}

void shuffleDeck(Deck &deck)
{
    static std::random_device rd;
    static std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    static std::mt19937 mt{ss};

    std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card &card)
{
    switch (card.rank)
    {
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return 10;
    case Rank::Ace:
        return 11;
    default:
        return static_cast<int>(card.rank) + 1;
    }
}

// draw the next card from a given deck, print what it is, then return it's value
int drawCard(const Deck &deck, const bool reset = false)
{
    static CardIndex drawIndex{0};
    if (reset)
    {
        drawIndex = 0;
    }

    Card newCard{deck[drawIndex++]};

    printCard(newCard);

    return getCardValue(newCard);
}

bool getPlayerAction()
{
    std::cout << "Enter hit or stand: ";
    std::string choice{};
    std::cin >> choice;

    // TODO: input checking
    return choice == "hit";
}

bool playBackjack(const Deck &deck)
{

    std::cout << "A new game of Blackjack has started!\n";

    // The dealer gets one card to start
    std::cout << "The dealer drew ";
    int dealerScore{drawCard(deck)};
    std::cout << '\n';

    // The player gets two cards to start
    std::cout << "You drew ";
    int playerScore{drawCard(deck)};
    std::cout << " and ";
    playerScore += drawCard(deck);
    std::cout << '\n';

    // print starting scores
    std::cout << "Starting dealer score: " << dealerScore << '\n';
    std::cout << "Starting player score: " << playerScore << '\n';

    // The player goes first
    // A player can repeatedly “hit” or “stand”
    // If the player “stands”, their turn is over, and their score is calculated based on the cards they have been dealt
    // If the player “hits”, they get another card and the value of that card is added to their total score
    while (playerScore < g_maxHandScore)
    {
        if (!getPlayerAction())
        {
            break;
        }
        std::cout << "You drew ";
        playerScore += drawCard(deck);
        std::cout << "\nYour score is " << playerScore << '\n';
    }

    // If the player goes over a score of 21, they bust and lose immediately
    if (playerScore >= g_maxHandScore)
    {
        return false;
    }

    // The dealer goes after the player
    // The dealer repeatedly draws until they reach a score of 17 or more, at which point they stand
    while (dealerScore < g_dealerMaxScore && dealerScore < playerScore)
    {
        std::cout << "The dealer drew ";
        dealerScore += drawCard(deck);
        std::cout << "\nThe dealer's score is " << dealerScore << '\n';
    }

    // If the dealer goes over a score of 21, they bust and the player wins immediately
    if (dealerScore >= g_maxHandScore)
    {
        return false;
    }

    // Otherwise, if the player has a higher score than the dealer, the player wins. Otherwise, the player loses
    return playerScore > dealerScore;
}

int main()
{
    auto deck{createDeck()};
    shuffleDeck(deck);

    if (playBackjack(deck))
    {
        std::cout << "You win!!!\n";
    }
    else
    {
        std::cout << "You lose :(\n";
    }

    return 0;
}
