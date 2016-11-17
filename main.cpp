#include <ncurses.h>
#include <string>
#include <cstring>
#include <unistd.h>


//******Config definitions*********
#define WORLD_SIZE_X 500
#define WORLD_SIZE_Y 500

//******Debug definitions********

//#define UNITTEST true
#define DEBUG true
//#define IDDEBUG true


//***********Includes needed only for certain definitions************
#ifdef UNITTEST
    #include <iostream>
#endif



int grid[WORLD_SIZE_Y][WORLD_SIZE_X] = {};
char id_char[] = {
    '.',
    ' ',
    '+',
    '|',
    '-',
    '*',
};
bool id_isSolid[] = {
    false,
    false,
    true,
    true,
    true,
    false,
};


int p_posx = 0;
int p_posy = 0;
int p_direction = 0;


int inv[8] = {0,1,2,3,4,5};
int inv_max = 5;
int inv_highlighted = 0;


//Move this to a header file...
bool getSolid(int y, int x);


//Enemie stuff. Easier to be OO, and I need practice with C-style OO
class Enemie
{
    public:  
        int px,py,myDirection;
        static int ammount;
        int player_move(int direction);
        Enemie(int y,int x)
        {
            py = y;
            px = px;
            ammount++;
            myDirection = 0;
        };
        int move(int direction)
        {
            switch(direction)
            {
                case 0: //w
                    if (!getSolid(p_posy-1,p_posx))
                    {
                        p_posy--;
                    }
                    break;
                case 1: //s
                    if (!getSolid(p_posy+1,p_posx))
                    {
                        p_posy++;
                    }
                    break;
                case 2: //a
                    if (!getSolid(p_posy,p_posx-1))
                    {
                        p_posx--;
                    }
                    break;
                case 3: //d
                    if (!getSolid(p_posy,p_posx+1))
                    {
                        p_posx++;
                    }
                    break;


            }
            myDirection = direction;
            
            return 0;
        };

        



};

//int Enemie::ammount =0;




//////



void blankFillWorld()
{
    for (int i = 0; i <= WORLD_SIZE_Y; i++)
    {
        for (int o = 0; o <= WORLD_SIZE_X; o++)
        {
            grid[i][o] = 0;
        }
    }
}



int render()
{
    init_pair(1,COLOR_BLACK,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_WHITE);
    init_pair(3,COLOR_BLACK,COLOR_BLACK);
    init_pair(4,COLOR_BLACK,COLOR_BLACK);
    init_pair(5,COLOR_BLACK,COLOR_BLACK);
    init_pair(6,COLOR_MAGENTA,COLOR_GREEN);
    init_pair(50,COLOR_BLACK,COLOR_BLACK);

    int row,col;
    getmaxyx(stdscr,row,col);

    int halfrow = row/2;
    int halfcol = col/2;
    for (int y=0; y <=row;y++)
    {
        for (int x=0; x <=col;x++)
        {
            int realposy = y+p_posy-halfrow;
            int realposx = x+p_posx-halfcol;
            int color = COLOR_PAIR(1);
            char realprint = '?';
            if ( realposy  < 0 || realposy > WORLD_SIZE_Y || realposx < 0 || realposx > WORLD_SIZE_X) 
            {
                color = COLOR_PAIR(50);
                realprint = '~';
                
            }
            else
            {
                color = COLOR_PAIR(grid[realposy][realposx]+1);
                realprint = id_char[grid[realposy][realposx]];
            }

            attron(color);//grid[realposy][realposx]));
            #ifdef IDDEBUG
                if (realprint != '~')
                    mvprintw(y,x,"%d",grid[realposy][realposx]);
            #else
                mvprintw(y,x,"%c",realprint);
            #endif
            attroff(color);

        }
    }
    char pchar = '?';
    switch (p_direction)
    {
        case 0:
            pchar = '^';
            break;
        case 1:
            pchar = 'V';
            break;
        case 2:
            pchar = '<';
            break;
        case 3:
            pchar = '>';
            break;
    }
    //print player
    mvprintw((row/2),(col/2),"%c",pchar);


    #ifdef DEBUG
        mvprintw(0,0,"p_posy: %d   ",p_posy);
        mvprintw(1,0,"p_posx: %d   ",p_posx);
        mvprintw(2,0,"inv_highlighted: %d   ",inv_highlighted);
    #endif 
    refresh();
    return 0;
}
bool getOutOfWorld(int y,int x)
{
    return (y < 0 || y > WORLD_SIZE_Y || x < 0 || x > WORLD_SIZE_X);
}


bool getSolid(int y, int x)
{
    return ( getOutOfWorld(y,x)|| id_isSolid[grid[y][x]]); //This won't segfault because, if it's out of bounds, it will be true and C++ will stop checking further because the or statement is already satisfied.
}

int player_move(int direction)
{
    switch(direction)
    {
        case 0: //w
            if (!getSolid(p_posy-1,p_posx) && direction == p_direction)
            {
                p_posy--;
            }
            break;
        case 1: //s
            if (!getSolid(p_posy+1,p_posx) && direction == p_direction)
            {
                p_posy++;
            }
            break;
        case 2: //a
            if (!getSolid(p_posy,p_posx-1) && direction == p_direction)
            {
                p_posx--;
            }
            break;
        case 3: //d
            if (!getSolid(p_posy,p_posx+1) && direction == p_direction)
            {
                p_posx++;
            }
            break;


    }
    p_direction = direction;
    
    return 0;

}

int placeObj(int y,int x,int what)
{
    if (!getOutOfWorld(y,x))
        grid[y][x] = what;                   
    return 0;
}

int main()
{
    // Colors for the IDs, initColor(id)

    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_WHITE);
    init_pair(3,COLOR_BLACK,COLOR_BLACK);
    init_pair(4,COLOR_BLACK,COLOR_BLACK);
    init_pair(5,COLOR_BLACK,COLOR_BLACK);
    init_pair(6,COLOR_GREEN,COLOR_MAGENTA);

    attron(COLOR_PAIR(3));//grid[realposy][realposx]));
    #ifdef UNITTEST

        //Variable setting
        grid[1][1] = 2; //a solid


        std::cout << "1. Test solid at -1,0. Should be true." << std::endl;       
        std::cout << getSolid(-1,0) << std::endl;
        std::cout << "2. Test solid at 1,1. Should be true." << std::endl;       
        std::cout << getSolid(1,1) << std::endl;
        std::cout << "3. Test solid at 1,0. Should be false." << std::endl;       
        std::cout << getSolid(1,0) << std::endl;
        std::cout << "5. Test solid at 0,0. Should be false." << std::endl;       
        std::cout << getSolid(0,0) << std::endl;



    #else
        /////////

        grid[3][3] = 2;
        grid[3][4] = 2;
        grid[3][5] = 2;
        grid[3][6] = 2;
        grid[4][3] = 2;
        grid[5][3] = 2;
        grid[6][3] = 2;
        grid[3][6] = 2;
        grid[4][6] = 2;
        grid[5][6] = 2;
        grid[6][6] = 2;
        grid[130][40] = 2;


        ////////

        initscr();
        raw();
        keypad(stdscr,true);
        noecho();
        start_color();
        int c;
        bool on = true;
        while (on)
        {
            c = getch();
            switch(c)
            {
                case 119: //w
                    player_move(0);
                    break;
                case 115: //s
                    player_move(1);
                    break;
                case 97: //a
                    player_move(2);
                    break;
                case 100: //d
                    player_move(3);
                    break;
                case 32: //space
                    switch(p_direction)
                    {
                        case(0):
                            placeObj(p_posy-1,p_posx,inv[inv_highlighted]);
                            break;
                        case(1):
                            placeObj(p_posy+1,p_posx,inv[inv_highlighted]);
                            break;
                        case(2):
                            placeObj(p_posy,p_posx-1,inv[inv_highlighted]);
                            break;
                        case(3):
                            placeObj(p_posy,p_posx+1,inv[inv_highlighted]);
                            break;
                    }
                    break;
                case 259: //up arrw
                    p_direction = 0;
                    break;
                case 258: //down arrw
                    p_direction = 1;
                    break;
                case 260: //left arrw
                    p_direction = 2;
                    break;
                case 261: //right arrw
                    p_direction = 3;
                    break;
                case 27: //escape
                    //make this show a menu. for now, it will quit
                    on = false;
                    break;
                    
                default:
                    //mvprintw(0,0,"%3d",c);
                    if (c >= 49 && c<= 57)
                    {
                        int wouldbe_set = c-49;
                        if (wouldbe_set <= inv_max)
                        {
                            inv_highlighted = wouldbe_set;
                        }
                    }


            }
            render();
        }
        endwin();
    #endif 
    return 0;

        


}

