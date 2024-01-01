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

struct Player
{
	int score{};
};

bool dealerTurn(Player& dealer, Deck& deck)
{
	while (dealer.score < Setting::stopValue)
	{
		Card nextCard{ deck.dealCard() };
		dealer.score += nextCard.value();
		std::cout << "The dealer flips a " << nextCard
			<< ". They now have: " << dealer.score << '\n';
	}
	if (dealer.score > Setting::bustValue)
	{
		std::cout << "The dealer went bust!\n";
		return true;
	}
	return false;
}

bool play()
{
	Deck deck{};
	deck.shuffle();
	Player dealer{ deck.dealCard().value() };
	std::cout << "The dealer is showing: " << dealer.score << '\n';
	Player player{ deck.dealCard().value() + deck.dealCard().value() };
	std::cout << "You have score: " << player.score << '\n';
	if (dealerTurn(dealer, deck))
		return true;
	return player.score > dealer.score;
}

int main()
{
	bool playerWin(play());
	std::cout << "You " << (playerWin ? "win" : "lose") << "!\n";

	return 0;
}