#pragma once
#include <iostream>
#include <vector>
#include <ctime>

enum class GameState
{
	StillPlaying,
	Winning,
	NoMoves
};

template<typename CompetitorInterfaceType, typename CompetitionState, typename CompetitionTurn>
class Executer
{
public:
	using CompetitorList = std::vector<CompetitorInterfaceType*>;
	Executer(const CompetitorList& competitors, int maxStuckAttempts)
		: competitors(competitors)
		, maxStuckAttempts(maxStuckAttempts)
	{
		srand(time(NULL));
	}
	CompetitorInterfaceType* Brawl(CompetitorInterfaceType& first, CompetitorInterfaceType& second, bool waitForKeypress, int attemptsLeft)
	{
		if (attemptsLeft == 0)
		{
			CompetitorInterfaceType* winner = (rand() % 100) < 50 ? &first : &second;
			std::cout << "These guys can't beat each other! I'll pick on random who wins: "
				<< winner->GetName() << std::endl;
			return winner;
		}

		std::cout << "--- Brawl! " << first.GetName() << " Vs. " << second.GetName() << std::endl;
		first.SetIsGoingFirst(true);
		second.SetIsGoingFirst(false);
		CompetitionState state;
		CompetitorInterfaceType* nextPlayer = &first;
		GameState gameState;
		while ((gameState = state.IsLastMoveWinning()) == GameState::StillPlaying)
		{
			nextPlayer = (nextPlayer == &first) ? &second : &first;
			const CompetitionTurn turn = nextPlayer->NextTurn(state);
			if (state.IsTurnValid(turn))
			{
				std::cout << "Cheating move by " << nextPlayer->GetName() << "!" << std::endl;
				nextPlayer = (nextPlayer == &first) ? &second : &first;
				gameState = GameState::Winning;
				break;
			}
			state.ApplyTurn(turn);
			state.Print();
			std::cout << "*********" << std::endl;
			if (waitForKeypress)
			{
				std::cin.get();
			}
		}
		if (gameState == GameState::Winning)
		{
			std::cout << "We've got a winner! " << nextPlayer->GetName() << " is the winner!" << std::endl;
			return nextPlayer;
		}
		else
		{
			std::cout << "Game stuck! Replaying and switching sides!" << std::endl;
			return Brawl(second, first, waitForKeypress, attemptsLeft - 1);
		}
	}
	void Run(bool waitForKeypressBetweenRounds)
	{
		while (competitors.size() > 1)
		{
			CompetitorList winners;
			for (size_t i = 0; i < this->competitors.size() - 1; i += 2)
			{
				auto winner = this->Brawl(*this->competitors[i], *this->competitors[i + 1], waitForKeypressBetweenRounds, this->maxStuckAttempts);
				winners.push_back(winner);
			}
			using namespace std;
			std::swap(winners, competitors);
		}
	}
private:
	CompetitorList competitors;
	int maxStuckAttempts;
};
	