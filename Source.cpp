#include <iostream>
#include <array>
#include <random>
#include <vector>
#include <algorithm>

enum class Ranks
{
	two = 2,
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
	Ace,

	maxCard
};

enum class Suits
{
	Clubs,
	Diamonds,
	Hearts,
	Spades,

};

struct Card
{
	Ranks rank{};
	Suits suit{};
};

void printCard(const Card& card)
{
	switch (card.rank)
	{
	case Ranks::two: std::cout << '2'; break;
	case Ranks::three: std::cout << '3'; break;
	case Ranks::four: std::cout << '4'; break;
	case Ranks::five: std::cout << '5'; break;
	case Ranks::six: std::cout << '6'; break;
	case Ranks::seven: std::cout << '7'; break;
	case Ranks::eight: std::cout << '8'; break;
	case Ranks::nine: std::cout << '9'; break;
	case Ranks::ten: std::cout << "10"; break;
	case Ranks::Jack: std::cout << 'J'; break;
	case Ranks::Queen: std::cout << 'Q'; break;
	case Ranks::King: std::cout << 'K'; break;
	case Ranks::Ace: std::cout << 'A'; break;
	default: std::cout << '?'; break;
	}

	switch (card.suit)
	{
	case Suits::Clubs: std::cout << 'C'; break;
	case Suits::Diamonds: std::cout << 'D'; break;
	case Suits::Spades: std::cout << 'S'; break;
	case Suits::Hearts: std::cout << 'H'; break;

	default: std::cout << '?'; break;
	}
}

using DeckOfCard = std::array<Card, 52>;

DeckOfCard& createDeck(DeckOfCard& card)
{
	int count = 0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 2; j <= 14; ++j)
		{
			card[count].suit = static_cast<Suits>(i);
			card[count].rank = static_cast<Ranks>(j);

			++count;

		}
	}

	return card;
}

void printDeck(const DeckOfCard& deck)
{
	for (const auto& i : deck)
	{
		printCard(i);
		std::cout << ' ';
	}
}

void shuffleDeck(DeckOfCard& deck)
{
	std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
	std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card& card)
{
	if (static_cast<int>(card.rank) <= 10 && static_cast<int>(card.rank) >= 2)
		return static_cast<int>(card.rank);
	else if (card.rank == Ranks::Ace)
		return 11;
	else
		return 10;
}

void printDealerInitialCards(const DeckOfCard& deck)
{
	std::cout << "Dealer's cards: ";
	printCard(deck[1]);
	std::cout << ' ';
	printCard(deck[3]);
	std::cout << '\n';
}

std::string playBlackJack(DeckOfCard& deck)
{
	shuffleDeck(deck);
	int ace{};

	// print users card
	std::cout << "Your cards: ";
	printCard(deck[0]);
	std::cout << ' ';
	printCard(deck[2]);
	std::cout << '\n';

	int userScore{ getCardValue(deck[0]) + getCardValue(deck[2]) };
	int dealerScore{ getCardValue(deck[1]) + getCardValue(deck[3]) };

	// xi bang
	if (userScore == 22 && dealerScore != 22)
	{
		printDealerInitialCards(deck);
		return "Player win with 2 ACES!!";

	}
	else if (userScore == 22 && dealerScore == 22)
	{
		printDealerInitialCards(deck);
		return "Tie win with 2 ACES!!";
	}
	else if (userScore != 22 && dealerScore == 22)
	{
		printDealerInitialCards(deck);
		return "Dealer win with 2 ACES!!";
	}

	// xi Zach
	if (userScore == 21 && dealerScore != 21)
	{
		printDealerInitialCards(deck);
		return "Player win with blackjack!!";
	}
	else if (userScore == 21 && dealerScore == 21)
	{
		printDealerInitialCards(deck);
		return "Tie win with blackjack!!";
	}
	else if (userScore != 21 && dealerScore == 21)
	{
		printDealerInitialCards(deck);
		return "Dealer win with blackjack!!";
	}

	// Check for user's Ace 
	if (deck[0].rank == Ranks::Ace)
	{
		std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
		std::cin >> ace;
		userScore = ace + getCardValue(deck[2]);
	}
	else if (deck[2].rank == Ranks::Ace)
	{
		std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
		std::cin >> ace;
		userScore = ace + getCardValue(deck[0]);
	}



	std::string userTurn{};
	std::string dealerTurn{};
	int count = 4;

	// User turn: 
	while(true)
	{
		std::cout << "Current score: " << userScore << '\n';
		std::cout << "Your turn(hit or stand): ";
		std::cin >> userTurn;
		if (userTurn == "stand" && userScore < 16)
		{
			std::cout << "Error: your score is lower than 16!!\n";
			continue;
		}
		else if (userTurn == "stand")
			break;
		else
		{
			std::cout << "new card: ";
			printCard(deck[count]); 
			std::cout << '\n';
			if (deck[count].rank == Ranks::Ace)
			{
				std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
				std::cin >> ace;
				userScore += ace;
			}
			else
				userScore += getCardValue(deck[count]);

			++count;
			if (userScore > 21)
			{
				std::cout << userScore << ": user bust.";
				return "Dealer win!";
			}
		}
	} 


	// print dealer card
	printDealerInitialCards(deck);
	
	// Check for dealer's Ace 
	if (deck[1].rank == Ranks::Ace)
	{
		std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
		std::cin >> ace;
		dealerScore = ace + getCardValue(deck[3]);
	}
	else if (deck[3].rank == Ranks::Ace)
	{
		std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
		std::cin >> ace;
		dealerScore = ace + getCardValue(deck[1]);		
	}

	// Dealer turn: 
	while (true)
	{
		std::cout << "Dealer's score: " << dealerScore << '\n';
		std::cout << "Dealer's turn(hit or stand): ";
		std::cin >> dealerTurn;
		if (dealerTurn == "stand" && dealerScore < 15)
		{
			std::cout << "Error: Dealer's score is lower than 15!!\n";
			continue;
		}
		else if (dealerTurn == "stand")
			break;
		else
		{
			std::cout << "new card: ";
			printCard(deck[count]);
			std::cout << '\n';
			if (deck[count].rank == Ranks::Ace)
			{
				std::cout << "You pulled an Ace. Choose 1 or 11 as its value: ";
				std::cin >> ace;
				dealerScore += ace;
			}
			else
				dealerScore += getCardValue(deck[count]);

			++count;
			if (dealerScore > 21)
			{
				std::cout << dealerScore << ": dealer bust.";
				return "Player win!!";
			}
		}
	}

	if (userScore > dealerScore)
		return "Player win!!";
	else if (userScore == dealerScore)
		return "Tie";
	else
		return "Dealer win!";

}

int main()
{
	DeckOfCard deckOfCard = createDeck(deckOfCard);
	std::cout << playBlackJack(deckOfCard) << '\n';
	//printDeck(deckOfCard);

	return 0;
}