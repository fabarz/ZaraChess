#include "ZaraChess.h"

CLI::ZaraChess::ZaraChess()
{
}

CLI::ZaraChess::ZaraChess(String ^ fen)
{
}

String ^ CLI::ZaraChess::Dump()
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

String ^ CLI::ZaraChess::ListMoves()
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

String ^ CLI::ZaraChess::GetBestMove()
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

String ^ CLI::ZaraChess::SideToMove()
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

void CLI::ZaraChess::UndoMove()
{
	throw gcnew System::NotImplementedException();
}

bool CLI::ZaraChess::CheckMove(String ^ move)
{
	return false;
}

int CLI::ZaraChess::EvaluateMove(String ^ move)
{
	return 0;
}

void CLI::ZaraChess::MakeMove(String ^ move)
{
	throw gcnew System::NotImplementedException();
}
