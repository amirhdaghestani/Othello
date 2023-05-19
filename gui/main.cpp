#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

bool in_range (int input, int min, int max) {
	if ( input >= min && input <= max)
		return true;
	return false;
}

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

	bool check_toward( int x, int y, int i, int j) {
		while( true ) {
			x += i;
			y += j;
			if ( !in_range( x, 0, 7 ) || !in_range( y, 0, 7 ) )
				break;
			if ( state[ x ][ y ] == turn + 1 )
				return true;
			if ( state[ x ][ y ] == 0 )
				return false;
		}
		return false;
	}
	bool valid_home( int x, int y ) {
		if ( !in_range( x, 0, 7 ) || !in_range( y, 0, 7 ) )
			return false;
		if ( state[ x ][ y ] != 0 )
			return false;
		for( int i = -1; i < 2; i++ )
			for( int j = -1; j < 2; j++ ) {
				if ( in_range( x + i, 0, 7 ) && in_range( y + j, 0, 7 ) ) {
					if ( state[ x + i ][ y + j ] == ( !turn ) + 1 ) {
						if( check_toward( x, y, i, j ) )
							return true;
					}
				}
			}
		return false;
	}

	bool place_disks(int x, int y, bool only_checking = 0)
	{
		flag = 0;
		int trueTurn = turn;
		trueTurn++;
		if(state[x][y] != 0)
			return false;
		// columns
		for(int i = x+1; i < 8; i++)
		{
			if(state[i][y] == 0)
				break;
			if(state[i][y] == trueTurn)
			{
				for(int k = x+1; k < i; k++)
				{
					flag = 1;	
					if( only_checking )
						break;
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
					flag = 1;	
					if( only_checking ) 
						break;

					state[k][y] = trueTurn;
					if( only_checking )
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
					flag = 1;	
					if( only_checking )   
						break;

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
					flag = 1;	
					if( only_checking )  
						break;


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
					flag = 1;	
					if( only_checking )  
						break;


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
					flag = 1;	
					if( only_checking )  
						break;


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
					flag = 1;	
					if( only_checking )  
						break;


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
					flag = 1;	
					if( only_checking )  
						break;


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
				}
				break;
			}
		}
		
		if ( !only_checking && flag )
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
		return flag;
	}
};

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Texture *MarbleSpriteSheet;
SDL_Texture *gGameOver;
SDL_Rect clipMarble[ 3 ];
SDL_Rect rectMarblePlace;
int valid_house_counter = -1;
bool show_valid_house = false;

void init() {
	if ( SDL_Init( SDL_INIT_VIDEO ) >= 0 ) {
		gWindow = SDL_CreateWindow( "Othello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN );
		gRenderer = SDL_CreateRenderer( gWindow, -1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if ( gRenderer == NULL )
			printf( "Not rendered, SDL_Error: %s\n", SDL_GetError() );
		if ( !IMG_Init( IMG_INIT_PNG ) )
			printf( "Not rendered, SDL_Error: %s\n", SDL_GetError() );
	}
}

void loadMedia() {
	SDL_Surface *MarbleSpriteSurface = IMG_Load ( "Marble.png" );
	MarbleSpriteSheet = SDL_CreateTextureFromSurface( gRenderer, MarbleSpriteSurface);

	SDL_Surface *GameOverSurface = IMG_Load ( "GameOver.png" );
	gGameOver = SDL_CreateTextureFromSurface( gRenderer, GameOverSurface);

	for( int i = 0; i < 3; i++ ) {
		clipMarble[ i ].h = 100;
		clipMarble[ i ].w = 100;
		clipMarble[ i ].x = 100*i;
		clipMarble[ i ].y = 0;
	}

	SDL_FreeSurface( MarbleSpriteSurface );
	SDL_FreeSurface( GameOverSurface );

	rectMarblePlace.x = 0;
	rectMarblePlace.y = 0;
	rectMarblePlace.w = 100;
	rectMarblePlace.h = 100;
}

void drawBoard( Board &board ) {
	valid_house_counter = 0;
	for( int i = 0; i < 8; i++ )
		for( int j = 0; j < 8; j++ ) {
			SDL_Rect rect = { i*100, j*100, 100, 100 };
			SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
			SDL_RenderDrawRect( gRenderer, &rect );

			if ( board.state[ i ][ j ] == 0  )
				if ( board.valid_home( i, j ) ) {
					if ( show_valid_house ) {
						rectMarblePlace.x = i * 100;
						rectMarblePlace.y = j * 100;
						rectMarblePlace.w = 100;
						rectMarblePlace.h = 100;
						SDL_RenderCopy( gRenderer, MarbleSpriteSheet, &clipMarble[ 2 ], &rectMarblePlace);
					}
					valid_house_counter++;
				}
			
			if ( board.state[i][j] != 0 ) {
				rectMarblePlace.x = 100 * i;
				rectMarblePlace.y = 100 * j;
				rectMarblePlace.w = 100;
				rectMarblePlace.h = 100;
				SDL_RenderCopy( gRenderer, MarbleSpriteSheet, &clipMarble[ board.state[i][j] - 1], &rectMarblePlace);
			}
		}
}



int main()
{
	Board board(0);
	string input;
	char ch;
	int x = 0,y = 0,count = 0;
	int errflg = 0;
	int checker = 0;
	ifstream test;
	test.open("test.in");
	int mode = 0;
	
	bool quit = false;
	SDL_Event e;
	int mouse_pos_x = 0, mouse_pos_y = 0;
	bool is_clicked = true;

	board.reset_board();
	board.state[3][3] = 0;
	board.state[4][4] = 0;
	board.state[3][4] = 0;
	board.state[4][3] = 0;
	board.reset_board();
	
	init();
	loadMedia();

	while( !quit )
	{
		while( SDL_PollEvent( &e ) != 0 ) {
			if ( e.type == SDL_QUIT )
				quit = true;
			if ( e.type == SDL_MOUSEBUTTONUP) {
				SDL_GetMouseState( &mouse_pos_x, &mouse_pos_y);
				is_clicked = true;
			}
			if ( e.type == SDL_FINGERDOWN) {
				mouse_pos_x = e.tfinger.x * 800;
				mouse_pos_y = e.tfinger.y * 800;
				is_clicked = true;
			}
		}

		if ( board.getTurn() == 2 ) {
			srand ( time(0) );
			for( int i = 0; i < 8; i++ ) {
				for( int j = 0; j < 8; j++ ) {
					board.place_disks(rand() % 8, rand() % 8);
					if ( board.flag == 1)
						break;
				}
				if ( board.flag == 1)
						break;
			}
			is_clicked = true;
			SDL_Delay(1000);
		}
		
		if ( is_clicked && valid_house_counter != 0) {
			x = mouse_pos_x / 100;
			y = mouse_pos_y / 100;
			if ( board.place_disks(x,y) ) {}

			SDL_SetRenderDrawColor( gRenderer, 42, 125, 50, 0xFF );
			SDL_RenderClear( gRenderer );
			drawBoard( board );

			SDL_RenderPresent( gRenderer );
			is_clicked = false;
		}

		if ( valid_house_counter == 0)  {
			SDL_SetRenderDrawColor( gRenderer, 42, 125, 50, 0xFF );
			SDL_RenderClear( gRenderer );
			drawBoard( board );	
			rectMarblePlace.x = 800/2 - 150;
			rectMarblePlace.y = 800/2 - 50;
			rectMarblePlace.w = 300;
			rectMarblePlace.h = 100;
			SDL_RenderCopy( gRenderer, gGameOver, NULL, &rectMarblePlace);
			SDL_RenderPresent( gRenderer );
		}
		
	}
	
	

	return 0;
}
