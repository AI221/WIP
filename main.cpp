#include <ncurses.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>


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



char grid[WORLD_SIZE_Y+1][WORLD_SIZE_X+1] = {};
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
int p_health = 100;


int inv[8] = {0,1,2,3,4,5};
int inv_max = 5;
int inv_highlighted = 0;


//Move this to a header file...
bool getSolid(int y, int x);


//Enemie stuff. Easier to be OO, and I need practice with C++-style OO

int zombieDamage = 5;
int zombieTicksNeeded = 2;
int zombieSight = 70;
class Enemie
{
    public:  
        int px,py,myDirection;
        static int ammount;
        int ticksPassed = 0;
        int health = 100;
        Enemie(int y,int x)
        {
            py = y;
            px = x;
            ammount++;
            myDirection = 0;
        };
        ~Enemie()
        {
            //rip
            ammount--;
        };
        int move(int direction)
        {
            switch(direction)
            {
                case 0: //w
                    if (!getSolid(py-1,px))
                    {
                        py--;
                    }
                    break;
                case 1: //s
                    if (!getSolid(py+1,px))
                    {
                        py++;
                    }
                    break;
                case 2: //a
                    if (!getSolid(py,px-1))
                    {
                        px--;
                    }
                    break;
                case 3: //d
                    if (!getSolid(py,px+1))
                    {
                        px++;
                    }
                    break;


            }
            myDirection = direction;
            
            return 0;      
        };
        int render(int halfrow, int halfcol)
        {
            attron(COLOR_PAIR(52));
            mvprintw(py-p_posy+halfrow,px-p_posx+halfcol,"O");
            attroff(COLOR_PAIR(52));
        };
        int tick()
        {
            if (ticksPassed >= zombieTicksNeeded)
            {
                int diffy = p_posy-py;
                int diffx = p_posx-px;
                int adiffy = std::abs(diffy);
                int adiffx = std::abs(diffx);
                if (adiffy +  adiffx <= zombieSight) //this should be a radius
                {
                    bool yIsGreater = adiffy > adiffx;
                    if (diffy > 0 && yIsGreater)
                    {
                        if (!getSolid(py+1,px))
                        {
                            py++;
                        } 
                    }
                    else if (diffy < 0 && yIsGreater)
                    {
                        if (!getSolid(py-1,px))
                        {
                            py--;
                        }
                        
                    }
                    else if (diffx > 0 && !yIsGreater)
                    {
                        if (!getSolid(py,px+1))
                        {
                            px++;
                        }
                    }
                    else
                    {
                        if (!getSolid(py,px-1))
                        {
                            px--;
                        }
                    }
                    //this is part of this if statement because this cannot be true without the above being true       
                    if (adiffy <= 1 && diffx <= 1)
                    {
                        p_health = p_health-zombieDamage;
                    }
                }
                ticksPassed = -1;
            }
            ticksPassed++;

            return 0;
        };

              



};

int Enemie::ammount =0;




//////

//Enemie* enemies[1] = {new Enemie(5,5)};
std::vector<Enemie*> enemies;

class bomb()
{
    public:
        int px,py,myDirection

        int tick()
        {
            

            return 0;   
        };
};



















int gameRound = 0;
int ticksToNextRound = 900; //aprox. 1 minute
void gameMasterTick()
{
    if (Enemie::ammount <= 0)
    {
        gameRound++;
        ticksToNextRound = 900;
    }
    else
    {
    }
        

}

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


bool getOutOfWorld(int y,int x)
{
    return (y < 0 || y > WORLD_SIZE_Y || x < 0 || x > WORLD_SIZE_X);
}

int render()
{
    init_pair(1,COLOR_BLACK,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_WHITE);
    init_pair(3,COLOR_BLACK,COLOR_BLACK);
    init_pair(4,COLOR_BLACK,COLOR_BLACK);
    init_pair(5,COLOR_BLACK,COLOR_BLACK);
    init_pair(6,COLOR_MAGENTA,COLOR_GREEN);
    init_pair(51,COLOR_BLACK,COLOR_BLACK); // colors after 51 reserved; ids restricted to <51
    init_pair(52,COLOR_RED,COLOR_RED); //enenmie color

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
            if (getOutOfWorld(realposy,realposx)) 
            {
                color = COLOR_PAIR(51);
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
    //Draw enemie

    //enemies[0]->render(halfrow,halfcol);

    for (Enemie* i : enemies)
    {
        i->render(halfrow,halfcol);
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
        mvprintw(3,0,"phealth: %d  ",p_health);
    #endif 
    refresh();
    return 0;
}


bool getSolid(int y, int x)
{
    bool isInEnemie = false;
    for (Enemie* i : enemies)
    {
        if (i->py == y && i->px == x)
        {
            isInEnemie = true;
        }
    }
    return ( isInEnemie || getOutOfWorld(y,x) || id_isSolid[grid[y][x]] || (p_posy == y && p_posx == x)); //This won't segfault because, if it's out of bounds, it will be true and C++ will stop checking further because the or statement is already satisfied.
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
    if (!(getOutOfWorld(y,x) || x==0 || y==0) )
        grid[y][x] = what;                   
    return 0;
}


bool on = true;
std::chrono::milliseconds ticktimespan(66);
int enemiehandler()
{
    while(on)
    {
        //enemies[0]->tick();
        for (Enemie* i : enemies)
        {
            i->tick();
        }
        std::this_thread::sleep_for(ticktimespan);        
    }
    return 0;
}
std::chrono::milliseconds rendertimespan(33);
int renderPeriodically()
{
    while (on)
    {
        render();
        std::this_thread::sleep_for(rendertimespan);
    }
    return 0;
}

int input()
{
    initscr();
    raw();
    keypad(stdscr,true);
    noecho();
    start_color();
    curs_set(0);
    int c;
    //render();
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
            #ifdef DEBUG
                case 262:
                    p_posy=p_posy-100;
                    break;
                case 360:
                    p_posy=p_posy+100;
                    break;
                case 330:
                    p_posx=p_posx-100;
                    break;
                case 338:
                    p_posx=p_posx+100;
                    break;
                case 39:
                    enemies.reserve(1);
                    enemies.push_back(new Enemie(p_posx+1,p_posy));
                    break;
            #endif
                
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
        //render();
    }
    endwin();
    return 0;
}
int main()
{
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
        //zero-out the world grid
        for (int y = 0;y < WORLD_SIZE_Y;y++)
        {
            for (int x = 0;x < WORLD_SIZE_X;x++)
            {
                grid[y][x] = 0;
            }
        }   
        std::thread mainthread(input);
        std::thread enemiethread(enemiehandler);
        std::thread renderthread(renderPeriodically);

        mainthread.join();
        enemiethread.join();

        return 0;
                    
    #endif
}
