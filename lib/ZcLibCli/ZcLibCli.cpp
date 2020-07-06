#include "pch.h"
#include <msclr/marshal.h>
using namespace msclr::interop;
#include "ZcLibCli.h"

using namespace System::Runtime::InteropServices;
static const char* string_to_char_array(String^ string)
{
	const char* str = (const char*)(Marshal::StringToHGlobalAnsi(string)).ToPointer();
	return str;
}

ZcLibCli::ZaraChess::ZaraChess() : ManagedObject<ChessGame>(new ChessGame())
{
}

ZcLibCli::ZaraChess::ZaraChess(String ^ fen) : ManagedObject<ChessGame>(new ChessGame(string_to_char_array(fen)))
{
}

String ^ ZcLibCli::ZaraChess::Dump()
{
	const char * op = m_Instance->dump();
	String ^ str = marshal_as<String ^>(op);
	return str;
}

String ^ ZcLibCli::ZaraChess::ListMoves()
{
	const char * op = m_Instance->list();
	String ^ str = marshal_as<String ^>(op);
	return str;
}

String ^ ZcLibCli::ZaraChess::GetBestMove()
{
	const char * op = m_Instance->best();
	String ^ str = marshal_as<String ^>(op);
	return str;
}

int ZcLibCli::ZaraChess::GetBestValue()
{
	return m_Instance->bestValue();
}

String ^ ZcLibCli::ZaraChess::SideToMove()
{
	const char * op = m_Instance->side();
	String ^ str = marshal_as<String ^>(op);
	return str;
}

void ZcLibCli::ZaraChess::
UndoMove()
{
	m_Instance->undoMove();
}

bool ZcLibCli::ZaraChess::
CheckMove(String ^ move)
{
	const char * mv = string_to_char_array(move);
	return m_Instance->check(mv);
}

int ZcLibCli::ZaraChess::
EvaluateMove(String ^ move)
{
	const char * mv = string_to_char_array(move);
	return m_Instance->evaluate(mv);
}

void ZcLibCli::ZaraChess::
MakeMove(String ^ move)
{
	const char * mv = string_to_char_array(move);
	return m_Instance->make(mv);
}

String ^ ZcLibCli::ZaraChess::GetStack()
{
	const char * op = m_Instance->getStack();
	String ^ str = marshal_as<String ^>(op);
	return str;
}
