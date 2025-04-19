#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <string>
#include "Matrix.h"


class Sudo
{
	short const D{ 9 };		//wielkosc matrycy 9 x 9
	int inVal_{ 1 };		//wartosc wpisywana
	int Row_{ 0 };			//wiersz
	int Column_{ 0 };		//kolumna

	HANDLE kolor = GetStdHandle(STD_OUTPUT_HANDLE);

	bool checkInVal_(const std::vector< std::vector<int>>& Matrix) const
	{
		if (!checkRow_(Matrix) &&
			!checkColumn_(Matrix) &&
			!checkBox_(Matrix))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool checkColumn_(const std::vector< std::vector<int>>& Matrix) const
	{
		for (int Row_{ 0 }; Row_ < D; Row_++)
		{
			if (Matrix[Row_][Column_] == inVal_)
			{
				return true;
			}
		}
		return false;
	}

	bool checkRow_(const std::vector< std::vector<int>>& Matrix) const
	{
		for (int Column_{ 0 }; Column_ < D; Column_++)
		{
			if (Matrix[Row_][Column_] == inVal_)
			{
				return true;
			}
		}
		return false;
	}

	bool checkBox_(const std::vector< std::vector<int>>& Matrix) const
	{
		int rBox{}, cBox{};
		rBox = (Row_ - Row_ % 3);
		cBox = (Column_ - Column_ % 3);

		for (int row_box{ 0 }; row_box < 3; row_box++)
		{
			for (int col_box{ 0 }; col_box < 3; col_box++)
			{
				if (Matrix[rBox + row_box][cBox + col_box] == inVal_)
				{
					return true;
				}
			}
		}
		return false;
	}

	void nextCell_() //nastêpna komórka
	{
		if (Column_ < D - 1)
		{
			Column_++;
		}
		else
		{
			Column_ = 0;
			Row_++;
		}
	}

	void previousCell_() //poprzednia kmórka
	{
		if (Column_ > 0)
		{
			Column_--;
		}
		else
		{
			Column_ = D - 1;
			Row_--;
		}

	}

	void backtrack_(std::vector< std::vector<int>>& Matrix0, std::vector< std::vector<int>>& MatrixREF)
	{
		Matrix0[Row_][Column_] = 0; //aktualn¹ komórkê wykasuj
		previousCell_();			//cofnij siê o 1
		while (MatrixREF[Row_][Column_] != 0) //albo dalej bo komórka jest zabroniona
		{
			previousCell_();
		}
		inVal_ = Matrix0[Row_][Column_]; // pobierz aktualn¹ wartoœæ
		inVal_++;
	}

	void sudoStart(std::vector< std::vector<int>>& Matrix0, const std::vector< std::vector<int>>& Matrix) const
	{
		for (int i{ 0 }; i < D; i++)
		{
			for (int j{ 0 }; j < D; j++)
			{
				Matrix0[i][j] = Matrix[i][j];
			}
		}
	}

	bool sudoSolution(std::vector< std::vector<int>>& Matrix0, std::vector< std::vector<int>>& MatrixREF)
	{
		sudoStart(Matrix0, MatrixREF);
		while (Row_ < D && Column_ < D)// (Row_ < 9)
		{
			if (inVal_ <= D && MatrixREF[Row_][Column_] == 0)
			{
				if (checkInVal_(Matrix0))
				{
					Matrix0[Row_][Column_] = inVal_;
					if (Row_ == D - 1 && Column_ == D - 1)
					{
						return true;
					}
					nextCell_();
					while (MatrixREF[Row_][Column_] != 0)
					{
						nextCell_();
					}
					inVal_ = 1;
				}
				else
				{	//nie pasuje?? zwieksz o 1
					inVal_++;
				}
			}
			else
			{
				if (Row_ <= 0 && Column_ <= 0 && inVal_ == D + 1)
				{
					return false;
				}
				backtrack_(Matrix0, MatrixREF);
			}
		}
	}

	void inRowColVal(std::vector< std::vector<int>>& Matrix)
	{
		//zabezpieczyæ przed wartosciami ujemnymi 
		size_t inRow{ 0 }, inCol{ 0 }, inVal{ 0 };

		std::cout << "Podaj wiersz: ";
		std::cin >> inRow;
		inRow--;
		std::cout << "Podaj kolumnê: ";
		std::cin >> inCol;
		inCol--;
		std::cout << "Podaj wartoœæ: ";
		std::cin >> inVal;

		if (inVal > 0 && inVal <= 9 &&
			inRow >= 0 && inRow < 9 &&
			inCol >= 0 && inCol < 9)
		{
			if (checkInVal_(Matrix))
			{
				Matrix[Row_][Column_] = inVal;
			}
			else
			{
				std::cout << inVal << " juz jest\n";
			}
		}
		else
		{
			std::cout << "zla wartoœci, podaj liczby w przedziale od 1 do 9\n";
			inRowColVal(Matrix);
		}

	}

	void printSudo(std::vector< std::vector<int>>& Matrix_00, std::vector< std::vector<int>>& Matrix)
	{
		SetConsoleTextAttribute(kolor, 14);

		std::cout << "\n    1 2 3   4 5 6   7 8 9 \n";
		for (int i{ 0 }; i < D; i++)
		{
			if (i % 3 == 0)
			{
				SetConsoleTextAttribute(kolor, 14);
				std::cout << "   -----------------------\n";
			}

			for (int j{ 0 }; j < D; j++)
			{
				if (j == 0)
				{
					SetConsoleTextAttribute(kolor, 14);
					std::cout << i + 1 << " | ";
				}

				if (Matrix_00[i][j] == 0)
				{
					std::cout << " " << " ";
				}
				else
				{
					if (Matrix_00[i][j] == Matrix[i][j])
					{
						SetConsoleTextAttribute(kolor, 11);
						std::cout << Matrix_00[i][j] << " ";
					}
					else
					{
						SetConsoleTextAttribute(kolor, 13);
						std::cout << Matrix_00[i][j] << " ";
					}
				}

				if ((j + 1) % 3 == 0)
				{
					SetConsoleTextAttribute(kolor, 14);
					std::cout << "| ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << "   -----------------------\n";
	}

	std::string fromFile(int& karetka)
	{
		std::fstream plik;
		std::string slowo;
		karetka--;
		plik.open("Matrix.txt", plik.in);
		if (!(plik.is_open()))
		{
			std::cout << "Plik nie owarty!" << std::endl;
			return "False!!";
		}
		else
		{
			plik.seekg(0, plik.end);
			const auto koniec{ plik.tellg() };
			constexpr size_t sudokuLenth{ 84 };  
			if (sudokuLenth * karetka > koniec)
			{
				karetka = koniec / sudokuLenth;

			}
			plik.seekg(sudokuLenth * karetka);

			while (!getline(plik, slowo, ';'))
			{
				plik >> slowo;
			}
			plik.close();
			return slowo;
		}
	}

	void copyWord(std::string slowo) const
	{
		constexpr int ASCII_Zero{ 48 };
		int k{ 0 };
		for (int i{ 0 }; i < D; i++)
		{
			for (int j{ 0 }; j < D; j++)
			{
				Sudo_Ref[i][j] = static_cast<int>(slowo[k]) - ASCII_Zero;
				k++;
			}
		}
	}

	bool endSudo(std::vector< std::vector<int>>& Matrix) const
	{
		int k{ 0 };
		for (int i{ 0 }; i < D; i++)
		{
			for (int j{ 0 }; j < D; j++)
			{
				return k *= Matrix[i][j];
			}
		}
	}

public:
	void solutionSudoku()
	{
		SetConsoleTextAttribute(kolor, 14);

		std::cout << "Czy chcesz rozwi¹zaæ samemu?? ";
		std::string wybor;
		std::cin >> wybor;

		std::cout << "Które Sudoku: ";
		int nrSudoku = 0;
		std::cin >> nrSudoku;
		copyWord(fromFile(nrSudoku));
		sudoStart(Sudo_00, Sudo_Ref);

		if (wybor == "tak" || wybor == "t" || wybor == "y" || wybor == "yes")
		{
			while (!endSudo(Sudo_00))
			{
				printSudo(Sudo_00, Sudo_Ref);
				inRowColVal(Sudo_00);
			}
		}
		else
		{
			printSudo(Sudo_00, Sudo_Ref);
			if (!sudoSolution(Sudo_00, Sudo_Ref))
			{
				std::cout << " Brak rozwiazania :( " << std::endl;
			}

		}
		printSudo(Sudo_00, Sudo_Ref);
	}



}; //end Class

