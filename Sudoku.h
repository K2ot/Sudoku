#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <string>
#include "Matrix.h"


class Sudo
{
	short const D = 9;		//wielkoœæ matrycy 9 x 9
	int inVal_ = 1;			//wartoœæ wpisywana
	int Row_ = 0;			//wiersz
	int Column_ = 0;		//kolumna

	HANDLE kolor = GetStdHandle(STD_OUTPUT_HANDLE);

	//sprawdza warunki wej
	bool checkInVal_(std::vector< std::vector<int>>& Matrix)
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

	bool checkColumn_(std::vector< std::vector<int>>& Matrix)
	{
		for (int Row_ = 0; Row_ < D; Row_++)
		{
			if (Matrix[Row_][Column_] == inVal_)
			{
				return true;
			}
		}
		return false;
	}

	bool checkRow_(std::vector< std::vector<int>>& Matrix)
	{
		for (int Column_ = 0; Column_ < D; Column_++)
		{
			if (Matrix[Row_][Column_] == inVal_)
			{
				return true;
			}
		}
		return false;
	}

	bool checkBox_(std::vector< std::vector<int>>& Matrix)
	{
		int rBox, cBox;
		rBox = (Row_ - Row_ % 3);
		cBox = (Column_ - Column_ % 3);

		for (int row_box = 0; row_box < 3; row_box++)
		{
			for (int col_box = 0; col_box < 3; col_box++)
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

	void sudoStart(std::vector< std::vector<int>>& Matrix0, std::vector< std::vector<int>>& Matrix)
	{
		for (int i = 0; i < D; i++)
		{
			for (int j = 0; j < D; j++)
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
				{	//nie pasuje?? zwiêksz o 1
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
		//zabezpieczyæ przed wartoœciami ujemnymi 
		auto inRow = 0, inCol = 0, inVal = 0;

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
				std::cout << inVal << " ju¿ jest\n";
			}
		}
		else
		{
			std::cout << "z³a wartoœci, podaj liczby w przedziale od 1 do 9\n";
			inRowColVal(Matrix);
		}

	}

	void printSudo(std::vector< std::vector<int>>& Matrix_00, std::vector< std::vector<int>>& Matrix)
	{
		SetConsoleTextAttribute(kolor, 14);

		std::cout << "\n    1 2 3   4 5 6   7 8 9 \n";
		for (int i = 0; i < D; i++)
		{
			if (i % 3 == 0)
			{
				SetConsoleTextAttribute(kolor, 14);
				std::cout << "   -----------------------\n";
			}

			for (int j = 0; j < D; j++)
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
			const auto koniec = plik.tellg();

			if (84 * karetka > koniec)
			{
				karetka = koniec / 84;

			}
			plik.seekg(84 * karetka);

			while (!getline(plik, slowo, ';'))
			{
				plik >> slowo;
			}
			plik.close();
			return slowo;
		}
	}

	void copyWord(std::string slowo)
	{
		int k = 0;
		for (int i = 0; i < D; i++)
		{
			for (int j = 0; j < D; j++)
			{
				Sudo_Ref[i][j] = slowo[k];
				Sudo_Ref[i][j] -= 48;
				k++;
			}
		}
	}

	bool endSudo(std::vector< std::vector<int>>& Matrix)
	{
		int k = 0;
		for (int i = 0; i < D; i++)
		{
			for (int j = 0; j < D; j++)
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
		if (wybor == "nie") //zabezpieczyæ przed z³ym wyborem 
		{
			if (!sudoSolution(Sudo_00, Sudo_Ref))
			{
				std::cout << " Brak rozwiazania :( " << std::endl;
			}	
		}
		else
		{
			while (!endSudo(Sudo_00)) //jak zakonczyæ grê / wstrzymaæ??
			{
				printSudo(Sudo_00, Sudo_Ref);
				inRowColVal(Sudo_00);
			}
		}
			printSudo(Sudo_00, Sudo_Ref);
	}



}; //end Class

