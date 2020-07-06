#pragma once
#include "ManagedObject.h"
#include "chessgame.h"

using namespace System;

namespace ZcLibCli {
	public ref class ZaraChess: ManagedObject<ChessGame>
	{
	public:
		ZaraChess();
	public:
		ZaraChess(String^ fen);
		property int depth
		{
		public:
			int get()
			{
				return m_Instance->getDepth();
			}
		public:
			void set(int value)
			{
				m_Instance->setDepth(value);
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

	public:
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
