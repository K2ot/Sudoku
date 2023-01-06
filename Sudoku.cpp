#include "Sudoku.h"

int main()
{
	setlocale(LC_CTYPE, "Polish");

	auto Sudoku = std::make_unique<Sudo>();
	Sudoku->solutionSudoku();
	return 0;
}

