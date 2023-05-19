#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

class Board
{
private:
	bool turn;
	int s1, s2;
public:
	int** state;
	bool flag = 0;
	Board(bool turn)
	{
		turn = turn;
		s1 = 2;
		s2 = 2;
		state = new int*[8];
		for(int i = 0; i < 8; i++)
		{
			state[i] = new int[8];
		}
	}

	int getTurn()
	{
		int trueTurn = turn;
		trueTurn++;
		return trueTurn;
	}

	int getS1()
	{
		return s1;
	}
	int getS2()
	{
		return s2;
	}

	void setS1(int dx = 1)
	{
		s1 += dx;
	}

	void setS2(int dx = 1)
	{
		s2 += dx;
	}
	Board& reset_board()
	{
		for(int i = 0; i < 8; i++)
		{
			for(int j = 0; j < 8; j++)
			{
			state[i][j] = 0;
			}
		}
		state[3][3] = 1;
		state[4][4] = 1;
		state[3][4] = 2;
		state[4][3] = 2;
		return *this;
	}

	Board& place_disks(int x, int y)
	{
		int trueTurn= turn;
		trueTurn++;
		if(state[x][y] != 0)
			return *this;
		// columns
		for(int i = x+1; i < 8; i++)
		{
			if(state[i][y] == 0)
				break;
			if(state[i][y] == trueTurn)
			{
				for(int k = x+1; k < i; k++)
				{
					state[k][y] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}
		for(int i = x-1; i >= 0; i--)
		{
			if(state[i][y] == 0)
				break;
			if(state[i][y] == trueTurn)
			{
				for(int k = x-1; k > i; k--)
				{
					state[k][y] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}

		// rows
		for(int i = y+1; i < 8; i++)
		{
			if(state[x][i] == 0)
				break;
			if(state[x][i] == trueTurn)
			{
				for(int k = y+1; k < i; k++)
				{
					state[x][k] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}
		for(int i = y-1; i >= 0; i--)
		{
			if(state[x][i] == 0)
				break;
			if(state[x][i] == trueTurn )
			{
				for(int k = y-1; k > i; k--)
				{
					state[x][k] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}

		// diag
		for(int i = x+1, j = y+1; i < 8 && j < 8; i++, j++)
		{
			if(state[i][j] == 0)
				break;
			if(state[i][j] == trueTurn)
			{
				for(int k = x+1, z = y+1; k < i && z < j; k++, z++)
				{
					state[k][z] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}
		for(int i = x-1, j = y-1; i >= 0 && j >= 0; i--, j--)
		{
			if(state[i][j] == 0)
				break;
			if(state[i][j] == trueTurn)
			{
				for(int k = x-1, z = y-1; k > i && z > j; k--, z--)
				{
					state[k][z] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}

		// anti diag
		for(int i = x+1, j = y-1; i < 8 && j >= 0; i++, j--)
		{
			if(state[i][j] == 0)
				break;
			if(state[i][j] == trueTurn)
			{
				for(int k = x+1, z = y-1; k < i && z > j; k++, z--)
				{
					state[k][z] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}

		for(int i = x-1, j = y+1; i >= 0 && j < 8; i--, j++)
		{
			if(state[i][j] == 0)
				break;
			if(state[i][j] == trueTurn)
			{
				for(int k = x-1, z = y+1; k > i && z < j; k--, z++)
				{
					state[k][z] = trueTurn;
					if(!turn)
					{
						s1++;
						s2--;
					}
					else
					{
						s2++;
						s1--;
					}
					flag = 1;
				}
				break;
			}
		}
		
		if(flag)
		{
			state[x][y] = trueTurn;
			if(!turn)
			{
				s1++;
			}
			else
			{
				s2++;
			}
			turn = !turn;
		}
		return *this;
	}
};

void ShowBoard(Board board)
{
	system("clear");
	cout << "Scores>\t\t\t      " << "$ Othello $" << "\t\t\t" << "\n";
	cout << "Player 1: " << board.getS1() << "\nPlayer 2: " << board.getS2() << "\n";
	cout << "\t\t\t" << " 1 " << " 2 " << " 3 " << " 4 " << " 5 " << " 6 " << " 7 " << " 8 " << "\t" << "\n";
	cout << "\t\t\t------------------------\n";
	for(int i = 0; i < 8; i++)
	{
		switch (i+1)
		{
			case 1:
				cout << "\t\t" <<  "    a  |" ;
				break;
			case 2:
				cout << "\t\t" <<  "    b  |" ;
				break;
			case 3:
				cout << "\t\t" <<  "    c  |" ;
				break;
			case 4:
				cout << "\t\t" <<  "    d  |" ;
				break;
			case 5:
				cout << "\t\t" <<  "    e  |" ;
				break;
			case 6:
				cout << "\t\t" <<  "    f  |" ;
				break;
			case 7:
				cout << "\t\t" <<  "    g  |" ;
				break;
			case 8:
				cout << "\t\t" <<  "    h  |" ;
				break;
			default:
				cout << "\t\t" <<  "    ?  |" ;
				break;
		}

		for(int j = 0; j < 8; j++)
		{
			cout << " " << board.state[i][j] << " ";
		}
	cout << "|" << endl;
	}
	cout << "\t\t\t------------------------\n";
}

int char_to_int(char ch)
{
	int temp;
	switch (ch)
	{
	case 'a':
		temp = 0;
		break;
	case 'b':
		temp = 1;
		break;
	case 'c':
		temp = 2;
		break;
	case 'd':
		temp = 3;
		break;
	case 'e':
		temp = 4;
		break;
	case 'f':
		temp = 5;
		break;
	case 'g':
		temp = 6;
		break;
	case 'h':
		temp = 7;
		break;
	default:
		break;
	}
	return temp;
}

// void winner(Board& board)
// {
// 	board.reset_board();
// 	board.state[3][3] = 0;
// 	board.state[4][4] = 0;
// 	board.state[3][4] = 0;
// 	board.state[4][3] = 0;
// 	int temp = 0;
// 	if (board.getS1() > board.getS2())
// 		temp = 1;
// 	else
// 		temp = 2;
// 	for(int i = 0; i < 8; i++)
// 	{
// 		for(int j = 0; j < 8; j++)
// 		{
// 			board.state[i][j] = temp;
// 		}
// 	}
// }

int main()
{
	Board board(0);
	string input;
	char ch;
	int x,y,count = 0;
	int errflg = 0;
	int checker = 0;
	ifstream test;
	test.open("test.in");
	int mode = 0;
	system("clear");
	board.reset_board();
	board.state[3][3] = 0;
	board.state[4][4] = 0;
	board.state[3][4] = 0;
	board.state[4][3] = 0;
	ShowBoard(board);
	cout << "Choose playing mode (1 for vs cpu, 2 for vs player, 3 for cpu vs cpu)> ";
	cin >> mode;
	board.reset_board();
	if (mode == 3)
		checker = 1;
	while(1)
	{
		fl1:
		if(count > 60)
			break;
		if (mode == 1)
		{
			checker = board.getTurn();
			checker--;
		}
		

		ShowBoard(board);
		if(errflg && board.flag == 0)
		{
			if(checker == 0)
			{
			cout << "You must choose properly! try again ...\n";
			cout << "Input Player " << board.getTurn() << "> ";
			}
		}
		else
		{
			if(checker == 0)
				cout << "\nInput Player " << board.getTurn() << "> ";
			else
			{
				test.seekg(0);
				if (mode == 1)
					sleep_for(1s);
				if (mode == 3)
					sleep_for(500ms);
			}
		}
		board.flag = 0;
		if(checker == 0)
			cin >> input;
		else
			getline(test, input);

		if (input.length() != 2)
		{
			errflg = 1;
			goto fl1;
		}
		ch = input.at(0);
		input = input.at(1);
		istringstream(input) >> y;
		y--;
		x = char_to_int(ch);
		if(x > 8 || x < 0 || y > 8 || y < 0)
		{
			errflg = 1;
			goto fl1;
		}
		board.place_disks(x,y);
		if(!board.flag && count != 60)
		{
			errflg = 1;
			goto fl1;
		}
		else
		{
			errflg = 0;
			count++;
		}
	}
	int temp = 0;
	if (board.getS1() > board.getS2())
		temp = 1;
	else
		temp = 2;
	
	cout << "\t\t    *** The Winner is PLAYER " << temp << " ***" << endl;
	return 0;
}
