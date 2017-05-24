#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Executer.h"

enum class TicTaeToeCell
{
	Empty,
	X,
	O
};

struct TicTaeToeTurn
{
	TicTaeToeTurn(unsigned short row, unsigned short col, TicTaeToeCell value)
		: Row(row)
		, Col(col)
		, Value(value)
	{}
	unsigned short Row;
	unsigned short Col;
	TicTaeToeCell Value;
};

struct TicTaeToeBoard
{
	static const unsigned short BoardLength = 3;
	TicTaeToeCell Board[BoardLength][BoardLength];
private:
	TicTaeToeTurn lastMove = { static_cast<unsigned short>(-1), static_cast<unsigned short>(-1), TicTaeToeCell::Empty };
public:

	TicTaeToeBoard()
	{
		std::memset(Board, 0, sizeof(Board));
	}
	bool IsTurnValid(const TicTaeToeTurn& turn) const
	{
		return turn.Row < BoardLength && turn.Col < BoardLength &&
			this->Board[turn.Row][turn.Col] != TicTaeToeCell::Empty;
	}
	void ApplyTurn(const TicTaeToeTurn& turn)
	{
		this->Board[turn.Row][turn.Col] = turn.Value;
		this->lastMove = turn;
	}
	void Print() const
	{
		char symbolMapping[] = { '-', 'X', 'O' };
		for (size_t i = 0; i < BoardLength; i++)
		{
			for (size_t j = 0; j < BoardLength; j++)
			{
				std::cout << symbolMapping[(unsigned)Board[i][j]] << ' ';
			}
			std::cout << std::endl;
		}
	}
	GameState IsLastMoveWinning() const
	{
		if (this->lastMove.Value == TicTaeToeCell::Empty)
		{
			return GameState::StillPlaying; // no moves yet
		}
		// Check the row
		bool activePlayerIsWinnerOnRow = true;
		for (size_t i = 0; i < BoardLength; i++)
		{
			activePlayerIsWinnerOnRow &= this->Board[this->lastMove.Row][i] == this->lastMove.Value;
		}
		// Check col
		bool activePlayerIsWinnerOnCol = true;
		for (size_t i = 0; i < BoardLength; i++)
		{
			activePlayerIsWinnerOnCol &= this->Board[i][this->lastMove.Col] == this->lastMove.Value;
		}
		// Check main diag
		bool activePlayerIsWinnerOnMainDiag = true;
		for (size_t i = 0; i < BoardLength; i++)
		{
			activePlayerIsWinnerOnMainDiag &= this->Board[i][i] == this->lastMove.Value;
		}
		bool activePlayerIsWinnerOnReverseDiag = true;
		for (size_t i = 0; i < BoardLength; i++)
		{
			activePlayerIsWinnerOnReverseDiag &= this->Board[i][BoardLength - i - 1] == this->lastMove.Value;
		}
		bool isPlayerWinning = activePlayerIsWinnerOnRow || activePlayerIsWinnerOnCol ||
			activePlayerIsWinnerOnMainDiag || activePlayerIsWinnerOnReverseDiag;
		if (isPlayerWinning)
			return GameState::Winning;
		bool isBoardPlayable =
			std::any_of(&this->Board[0][0], &this->Board[BoardLength - 1][BoardLength],
				[](TicTaeToeCell cell) { return cell == TicTaeToeCell::Empty; });
		if (!isBoardPlayable)
			return GameState::NoMoves;
		return GameState::StillPlaying;
	}
};

class ITicTaeToeClient
{
public:
	virtual void SetIsGoingFirst(bool goesFirst)
	{
		this->m_Symbol = static_cast<TicTaeToeCell>(goesFirst + 1);
	}
	virtual TicTaeToeTurn NextTurn(const TicTaeToeBoard& board) = 0;
	virtual std::string GetName() const = 0;
protected:
	TicTaeToeCell m_Symbol;
};

class NextFielder : public ITicTaeToeClient
{
	unsigned short counterx = 0;
	unsigned short countery = 0;

public:
	virtual void SetIsGoingFirst(bool goesFirst)
	{
		ITicTaeToeClient::SetIsGoingFirst(goesFirst);
		this->counterx = this->countery = 0;
	}

	virtual TicTaeToeTurn NextTurn(const TicTaeToeBoard& board) override
	{
		for (int i = 0; i < TicTaeToeBoard::BoardLength; i++)
		{
			for (int j = 0; j < TicTaeToeBoard::BoardLength; j++)
			{
				if (board.Board[i][j] == TicTaeToeCell::Empty)
				{
					return TicTaeToeTurn(static_cast<unsigned short>(i), static_cast<unsigned short>(j), m_Symbol);
				}
			}
		}
		return TicTaeToeTurn(static_cast<unsigned short>(-1), static_cast<unsigned short>(-1), m_Symbol);
	}
	virtual std::string GetName() const override
	{
		return "Next Freer";
	}
};

class LastRower : public ITicTaeToeClient
{
	unsigned short colCounter = 0;
public:
	virtual void SetIsGoingFirst(bool goesFirst)
	{
		ITicTaeToeClient::SetIsGoingFirst(goesFirst);
		this->colCounter = 0;
	}
	virtual TicTaeToeTurn NextTurn(const TicTaeToeBoard& board) override
	{
		return TicTaeToeTurn(TicTaeToeBoard::BoardLength - 1, this->colCounter++, this->m_Symbol);
	}
	virtual std::string GetName() const override
	{
		return "Last Rower";
	}
};