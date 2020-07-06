using namespace ZcLibCli;
using namespace System;

void help() {
	Console::Write(" This program accepts the following commands: \n");
	Console::Write("\t init  : To start a new game.\n");
	Console::Write("\t go    : To let computer play against itself.\n");
	Console::Write("\t fen   : To set up a position in fen notation.\n");
	Console::Write("\t undo  : to undo a move.\n");
	Console::Write("\t e2e4  : or any legal move to make a move.\n");
	Console::Write("\t list  : to show a list of legal moves.\n");
	Console::Write("\t depth : to set search depth.\n");
	Console::Write("\t exit  : to exit this program.\n");
	Console::Write("\t help  : to show this help screen.\n");

	Console::Write("\nPress enter to continue.\n");

	Console::ReadKey();

	Console::Write(" Ok.\n");
}

int main()
{
	int depth = 5;
	String ^ fen = fen = "r6k/6pp//6N/2Q/1K";
	ZaraChess ^ game = gcnew ZaraChess(fen);
	game->depth = depth;
	Console::WriteLine("Using depth " + depth);
	help();
	Console::WriteLine(game->Dump());
	String ^ command;
	String ^ op;
	String ^ halfMove;
	int no = 1;
	bool computerMove = false;
	bool go = false;
	int v = -1;
	while(command != "exit")
	{
		if (!go && computerMove == false) {
			String ^ com = Console::ReadLine();
			if (String::IsNullOrEmpty(com))
			{
				continue;
			}
			command = com->ToLower();
		}
		else {
			command = "look";
			computerMove = false;
		}

		if (command == "exit")
			break;

		else if (command == "init") {
			no = 1;
			delete game;
			game = gcnew ZaraChess();
			game->depth = depth;
			Console::WriteLine(game->Dump());
			continue;
		}

		else if (command == "go") {
			go = true;
			continue;
		}

		else if (command == "list") {

			op = game->ListMoves();
			Console::WriteLine(op);
			continue;

		}

		else if (command == "show") {
			Console::WriteLine(game->Dump());
		}

		else if (command == "look") {

			op = game->GetBestMove();
			halfMove = op;

			Console::Clear();
			Console::WriteLine("\n\t\t **** " + (unsigned int)game->Nodes + " nodes evaluated. ***\n");

			if (game->SideToMove() == "BLACK")
				Console::Write(no + ". ... ");
			else
				Console::Write(no + ". ");

			Console::WriteLine(op + " ");

			/*
			if (abs(hamfMove.value) >= CHECKMATE) {

				cout << " CHECKMATE in " << (int)(game->depth - (abs(hamfMove.value) - CHECKMATE)) << endl;

			}
			else {
				cout << (int)hamfMove.value << ":" << v << endl;
			}
			*/

			if (!game->Nodes) {
				Console::WriteLine(game->Dump());
				go = false;

				Console::WriteLine("  <<<<< GAME OVER ! >>>>>>  ");
				continue;
			}

		}

		else if (command == "undo") {
			game->UndoMove();
			Console::WriteLine(game->Dump());
			continue;

		}

		else if (command == "fen") {
			String ^ fen;
			while (String::IsNullOrEmpty(fen))
			{
				Console::WriteLine("Enter the fen notation : ");
				fen = Console::ReadLine();
			}
			game = gcnew ZaraChess(fen);
			Console::WriteLine(game->Dump());
			continue;
		}

		else if (command == "depth") {
			String ^ dep;
			while (String::IsNullOrEmpty(dep) || !int::TryParse(dep, depth))
			{
				Console::WriteLine("Enter depth:");
				dep = Console::ReadLine();
			}
			depth = Convert::ToInt32(dep);
			game->depth = depth;
			Console::WriteLine("Using depth " + depth);
			continue;
		}
		else if (command == "help" || command == "?") {
			help();
			continue;
		}

		else {
			v = -1;
			halfMove = command;

			if (game->SideToMove() == "BLACK") {
				//hamfMove.promotePiece = 'q';
			}
			else {
				//hamfMove.promotePiece = 'Q';
			}

			command = "look";
			computerMove = true;
		}

		if (game->CheckMove(halfMove)) {

			if (game->SideToMove() == "BLACK") {
				no++;
			}

			int v1 = game->EvaluateMove(halfMove);

			Console::WriteLine(" First I thought the value was = " + (int)v1);

			game->MakeMove(halfMove);
			Console::WriteLine(game->Dump());

		}
		else {

			computerMove = false;

			if (v == 0) {
				Console::Write("\n Remise \n");
			}

			go = false;
		}
	}

	Console::Write("End.\n");
	Console::ReadKey();

	return 0;
}