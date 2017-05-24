#include <algorithm>
#include <iostream>
#include <string>

#include "Executer.h"
#include "TicTaeToeClient.h"

int main()
{
	using TicTaeToeExecuter = Executer<ITicTaeToeClient, TicTaeToeBoard, TicTaeToeTurn>;
	TicTaeToeExecuter::CompetitorList clients = { new NextFielder(), new LastRower(), new NextFielder(), new LastRower() };
	const int MaxStuckAttempts = 3;
	const bool WaitForKeypressBetweenRounds = false;
	TicTaeToeExecuter executer(clients, MaxStuckAttempts);
	executer.Run(WaitForKeypressBetweenRounds);
	int x;
	std::cin >> x;
	return 0;
}