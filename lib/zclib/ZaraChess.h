//Entity.h
#pragma once
#include "ManagedObject.h"
#include "chessgame.h"
using namespace System;
namespace CLI
{
	public ref class ZaraChess : public ManagedObject<ChessGame>
	{
	public:

		ZaraChess();
		ZaraChess(String^ fen);
		property int depth
		{
		public:
			int get()
			{
				return m_Instance->depth;
			}
		private:
			void set(int value)
			{
				m_Instance->depth = value;
			}
		}
		property int Nodes
		{
		public:
			int get()
			{
				return m_Instance->nodes;
			}
		}

		String^ Dump();
		String^ ListMoves();
		String^ GetBestMove();
		String^ SideToMove();
		void UndoMove();
		bool CheckMove(String^ move);
		int EvaluateMove(String^ move);
		void MakeMove(String^ move);
	};
}
