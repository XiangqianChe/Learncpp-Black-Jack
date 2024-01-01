#include <iostream>
#include <array>
#include <cassert>
#include <algorithm>
#include "Random.h"
namespace Setting
{
	constexpr int bustValue{ 21 };
	constexpr int stopValue{ 17 };
}

struct Card
{
	enum Rank
	{
		rank_ace,
		rank_2,
		rank_3,
		rank_4,
		rank_5,
		rank_6,
		rank_7,
		rank_8,
		rank_9,
		rank_10,
		rank_jack,
		rank_queen,
		rank_king,
		max_ranks,
	};

	enum Suit
	{
		suit_club,
		suit_diamond,
		suit_heart,
		suit_spade,
		max_suits,
	};

	Rank rank{};
	Suit suit{};
	static constexpr std::array allRanks{ rank_ace, rank_2, rank_3, rank_4, rank_5, rank_6,
		rank_7, rank_8, rank_9, rank_10, rank_jack, rank_queen, rank_king };
	static constexpr std::array allSuits{ suit_club, suit_diamond, suit_heart, suit_spade };

	int value() const
	{
		static constexpr std::array RankValues{ 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
		return RankValues[rank];
	}

	friend std::ostream& operator<<(std::ostream& out, const Card& card)
	{
		static constexpr std::array ranks{ 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
		static constexpr std::array suits{ 'C', 'D', 'H', 'S' };
		out << ranks[card.rank] << suits[card.suit];
		return out;
	}
};

class Deck
{
public:
	Deck()
	{
		std::size_t count{};
		for (auto suit : Card::allSuits)
			for (auto rank : Card::allRanks)
				m_cards[count++] = Card{ rank, suit };
	}

	void shuffle()
	{
		std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
		m_nextCardIndex = 0;
	}

	Card dealCard()
	{
		assert(m_nextCardIndex != std::size(m_cards) && "Cards run out");
		return m_cards[m_nextCardIndex++];
	}
private:
	std::array<Card, 52> m_cards{};
	std::size_t m_nextCardIndex{};
};

class Player
{
public:
	int getScore() const { return m_score; }
	void addToScore(Card card)
	{
		m_score += card.value();
		if (card.rank == Card::rank_ace)
			++m_aces;
	}
	void consumeAces()
	{
		while (m_score > Setting::bustValue && m_aces > 0)
		{
			m_score -= 10;
			--m_aces;
		}
	}
private:
	int m_score{};
	int m_aces{};
};

bool dealerTurn(Player& dealer, Deck& deck)
{
	while (dealer.getScore() < Setting::stopValue)
	{
		Card card{ deck.dealCard() };
		dealer.addToScore(card);
		dealer.consumeAces();
		std::cout << "The dealer flips a " << card
			<< ". They now have: " << dealer.getScore() << '\n';
	}
	if (dealer.getScore() > Setting::bustValue)
	{
		std::cout << "The dealer went bust!\n";
		return true;
	}
	return false;
}

void ignoreLine() { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }

bool playerTurn(Player& player, Deck& deck)
{
	while (true)
	{
		std::cout << "(h) to hit, or (s) to stand: ";
		char hitOrStand{};
		std::cin >> hitOrStand;
		if (hitOrStand == 's') break;
		if (hitOrStand == 'h')
		{
			Card card{ deck.dealCard() };
			player.addToScore(card);
			player.consumeAces();
			std::cout << "You were dealt " << card
				<< ". You now have: " << player.getScore() << '\n';
		}
		ignoreLine();
		if (player.getScore() > Setting::bustValue)
		{
			std::cout << "You went bust!\n";
			return true;
		}
	}
	return false;
}

enum class Result
{
	playerWin,
	playerLose,
	tie,
};

Result play()
{
	Deck deck{};
	deck.shuffle();
	Player dealer{};
	dealer.addToScore(deck.dealCard());
	std::cout << "The dealer is showing: " << dealer.getScore() << '\n';
	Player player{};
	player.addToScore(deck.dealCard());
	player.addToScore(deck.dealCard());
	std::cout << "You have score: " << player.getScore() << '\n';
	if (playerTurn(player, deck))
		return Result::playerLose;
	if (dealerTurn(dealer, deck))
		return Result::playerWin;
	if (player.getScore() == dealer.getScore())
		return Result::tie;
	return (player.getScore() > dealer.getScore()) ? Result::playerWin : Result::playerLose;
}

int main()
{
	switch (play())
	{
	case Result::playerWin:
		std::cout << "You win!\n";
		break;
	case Result::playerLose:
		std::cout << "You lose!\n";
		break;
	case Result::tie:
		std::cout << "It's a tie.\n";
		break;
	}
	return 0;
}