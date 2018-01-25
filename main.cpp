#include <iostream>
#include <curses.h>
#include <windows.h>
#include <string>
#include <cstdlib>

using namespace std;

int current_getch;
int doloop = 1;
static WINDOW *mainwnd;
static WINDOW *screen;
WINDOW *my_win;
int playerY = 26, playerX = 24;
int shootY=100, shootX=100;
int counterS, counterE, counterA, counterB, counterB1;
int numberOfEnemies;
int score;
boolean strike = false;
int poziom, diffE, diffA, diffB, diffB1, speedS=10;
int LR;
int ammo = 5;
boolean reloaded = true;
int bombYA=200, bombXA=200;
int bombYB=300, bombXB=300;
int bombYC=400, bombXC=400;



void porgramSpeed (int speed)
{
    speed = speed;
    diffE = diffE/speed;
    diffA = diffA/speed;
    diffB = diffB/speed;
    diffB1 = diffB1/speed;
    speedS = speedS/speed;
}

struct enemy
{
  int posY;
  int pos;
  enemy *next;
};

bool isEmpty(enemy *p)
{
    if (p == NULL)
        return true;
    else
        return false;
}

void logo (void)
{
    cout << "  /$$$$$$  /$$   /$$ /$$$$$$  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$   /$$                       " << endl;
    cout << " /$$__  $$| $$  | $$|_  $$_/ /$$__  $$| $$  /$$/| $$_____/| $$$ | $$                       " << endl;
    cout << "| $$  \__/| $$  | $$  | $$  | $$  \__/| $$ /$$/ | $$      | $$$$| $$                       " << endl;
    cout << "| $$      | $$$$$$$$  | $$  | $$      | $$$$$/  | $$$$$   | $$ $$ $$                       " << endl;
    cout << "| $$      | $$__  $$  | $$  | $$      | $$  $$  | $$__/   | $$  $$$$                       " << endl;
    cout << "| $$    $$| $$  | $$  | $$  | $$    $$| $$\  $$ | $$      | $$\  $$$                       " << endl;
    cout << "|  $$$$$$/| $$  | $$ /$$$$$$|  $$$$$$/| $$ \  $$| $$$$$$$$| $$ \  $$                       " << endl;
    cout << " \______/ |__/  |__/|______/ \______/ |__/  \__/|________/|__/  \__/                       " << endl;
    cout << "                                                                                           " << endl;
    cout << "             /$$$$$$ /$$   /$$ /$$    /$$  /$$$$$$  /$$$$$$$  /$$$$$$$$ /$$$$$$$   /$$$$$$ " << endl;
    cout << "            |_  $$_/| $$$ | $$| $$   | $$ /$$__  $$| $$__  $$| $$_____/| $$__  $$ /$$__  $$" << endl;
    cout << "              | $$  | $$$$| $$| $$   | $$| $$  \ $$| $$  \ $$| $$      | $$  \ $$| $$  \__/" << endl;
    cout << "              | $$  | $$ $$ $$|  $$ / $$/| $$$$$$$$| $$  | $$| $$$$$   | $$$$$$$/|  $$$$$$ " << endl;
    cout << "              | $$  | $$  $$$$ \  $$ $$/ | $$__  $$| $$  | $$| $$__/   | $$__  $$ \____  $$" << endl;
    cout << "              | $$  | $$\  $$$  \  $$$/  | $$  | $$| $$  | $$| $$      | $$  \ $$ /$$  \ $$" << endl;
    cout << "             /$$$$$$| $$ \  $$   \  $/   | $$  | $$| $$$$$$$/| $$$$$$$$| $$  | $$|  $$$$$$/" << endl;
    cout << "            |______/|__/  \__/    \_/    |__/  |__/|_______/ |________/|__/  |__/ \______/ " << endl;
    cout << endl;
}

void menu (void)
{
    cout << "STEROWANIE" << endl;
    cout << "PORUSZANIE LEWO/PRAWO - 4/6 (numpad)" << endl;
    cout << "STRZAL - SPACJA" << endl;
    cout << "WYJSCIE Z GRY- q" << endl << endl;
    cout << "WYBIERZ POZIOM TRUDNOSCI" << endl;
    cout << "1. LATWY" << endl;
    cout << "2. SREDNI" << endl;
    cout << "3. TRUDNY" << endl;
    cout << "4. WYJSCIE" << endl;
}

void insertFirstEnemy (enemy *&p, enemy *&n, int pos, int posY)
{
    enemy *t = new enemy;
    t->pos = pos;
    t->posY = posY;
    t->next = NULL;
    p = t;
    n = t;
}

void insertEnemies (enemy *&p, enemy *&n, int pos, int posY)
{
    if(isEmpty(p))
        insertFirstEnemy(p, n, pos, posY);
    else
    {
    enemy *t = new enemy;
    t->pos = pos;
    t->posY = posY;
    t->next = NULL;
    n->next = t;
    n = t;
    }
}

void clearList (enemy *&p, enemy *&n)
{
    if (p==n)
    {
        delete p;
        p = NULL;
        n = NULL;
    }
    else
    {
        enemy *t = p;
        p = p->next;
        delete t;
    }
}

void displayEnemies(enemy *current)
{
    while(current != NULL)
    {
        mvwprintw(screen,current->posY,current->pos,"C");
        current = current-> next;
        wrefresh(screen);
        refresh();
    }
}

void moveEnemies1 (enemy *current)
{
    if (counterE == diffE)
    {
        while(current != NULL)
        {
            mvwprintw(screen,current->posY,current->pos," ");
            current = current-> next;
        }
    }
}

void moveEnemies2 (enemy *current)
{
    if (counterE == diffE)
    {
       while(current != NULL)
        {
            if(LR < 8)
            {
            current->pos = current->pos + 1;
            current = current->next;
            }
            if(LR > 7)
            {
            current->pos = current->pos - 1;
            current = current->next;
            }
        }
        LR++;
        if (LR == 16) LR=0;
        counterE=0;
    }
}

void screen_init(void)
{
  mainwnd = initscr();
  noecho();
  cbreak();
  nodelay(mainwnd, TRUE);
  refresh();
  wrefresh(mainwnd);
  screen = newwin(29, 50, 1, 1);
  box(screen, ACS_VLINE, ACS_HLINE);
}

void initShoot(int x, int y)
{
    shootX = x;
    shootY = y - 1;
}

void shootDown (enemy *&p)
{
    enemy *t = p;

        for (int i = 0; i<numberOfEnemies; i++)
        {
         if(shootY == t->posY)
            {
            if(shootX == t->pos)
            {
                score++;
                t->pos = 100;
                strike=true;
            }
        }
        t = t->next;
    }
}

void reloading (void)
{
    mvwprintw(screen,28,42,"Reload!");
    if (reloaded == true)
    {
        counterA = 0;
        reloaded = false;
    }
    if (counterA == diffA)
    {
        ammo = 5;
        reloaded = true;
    }
}

static void update_display(void)
{
  curs_set(0);
  mvwprintw(screen,playerY,playerX,"$");
  mvwprintw(screen,28,1,"Score: %d",score);
  if (ammo == 0) reloading();
  else mvwprintw(screen,28,42,"Ammo: %d",ammo);
  wrefresh(screen);
  refresh();
}

void player_control (void)
{
    if (current_getch == 52)
    {
        if(playerX > 1 )  playerX--;
    }
    if (current_getch == 54)
    {
        if(playerX < 48 ) playerX++;
    }
}

void delete_player(void)
{
  curs_set(0);
  mvwprintw(screen,playerY,playerX," ");
 // mvwprintw(screen,28,1," ");
  wrefresh(screen);
  refresh();
}

void updateShoot(void)
{
  if(shootY > 1)
  {
        if(strike==false)
        {
        shootY--;
        mvwprintw(screen,shootY,shootX,"!");
        }
        else
        {
        mvwprintw(screen,shootY,shootX," ");
        shootX =100; shootY = 100;
        }
  }
}

void shoot (void)
{
    if (ammo > 0 && current_getch == 32)
    {
       mvwprintw(screen,shootY,shootX," ");
       initShoot(playerX, playerY);
       ammo--;
       strike = false;
    }
    if (counterS == speedS)
    {
        mvwprintw(screen,shootY,shootX," ");
        updateShoot();
        counterS = 0;
    }
}

void initBombA (void)
{
    bombYA = 2;
    bombXA = 1+(rand()%48);
}

void updateBombA (void)
{
    if(bombYA < 27)
       {
           bombYA++;
           mvwprintw(screen,bombYA,bombXA,"*");
       }
}

void bombA (void)
{
    if (counterB == diffB)
    {
        mvwprintw(screen,bombYA,bombXA," ");
        initBombA();
    }
    if (counterB1 == diffB1)
    {
        mvwprintw(screen,bombYA,bombXA," ");
        updateBombA();
    }
}

void initBombB (void)
{
    bombYB = 2;
    bombXB = 1+(rand()%48);
}

void updateBombB (void)
{
    if(bombYB < 27)
       {
           bombYB++;
           mvwprintw(screen,bombYB,bombXB,"*");
       }
}

void bombB (void)
{
    if (counterB == diffB+50)
    {
        mvwprintw(screen,bombYB,bombXB," ");
        initBombB();
    }
    if (counterB1 == diffB1)
    {
        mvwprintw(screen,bombYB,bombXB," ");
        updateBombB();
    }
}

void initBombC (void)
{
    bombYC = 2;
    bombXC = 1+(rand()%48);
}

void updateBombC (void)
{
    if(bombYC < 27)
       {
           bombYC++;
           mvwprintw(screen,bombYC,bombXC,"*");
       }
}

void bombC (void)
{
    if (counterB == diffB+100)
    {
        mvwprintw(screen,bombYC,bombXC," ");
        initBombC();
        counterB = 0;
    }
    if (counterB1 == diffB1)
    {
        mvwprintw(screen,bombYC,bombXC," ");
        updateBombC();
        counterB1 = 0;
    }
}

int life(void)
{
    if(bombYA == playerY)
    {
        if(bombXA == playerX)
            return 0;
    }
    if(bombYB == playerY)
    {
        if(bombXB == playerX)
            return 0;
    }
    if(bombYC == playerY)
    {
        if(bombXC == playerX)
            return 0;
    }
    else
        return 1;
}

int main(void)
{
    enemy *p = NULL;
    enemy *n = NULL;
    logo();
    menu();
    cin >> poziom;
    if (poziom == 1)
    {
        numberOfEnemies=20;
        diffE=300;
        diffA=1000;
        diffB=450;
        diffB1=20;
        for (int i=0; i<numberOfEnemies; i++)
        {
            insertEnemies(p, n, i*2+2, 2);
        }
    }
    if (poziom == 2)
    {
        numberOfEnemies = 40;
        diffE = 200;
        diffA = 1500;
        diffB=400;
        diffB1=15;
        for (int i=0; i<20; i++)
        {
            insertEnemies(p, n, i*2+2, 2);
        }
        for (int i=0; i<numberOfEnemies-20; i++)
        {
            insertEnemies(p, n, i*2+1, 3);
        }
    }
    if (poziom == 3)
    {
        numberOfEnemies=60;
        diffE=100;
        diffA=2000;
        diffB=240;
        diffB1=14;
        for (int i=0; i<20; i++)
        {
            insertEnemies(p, n, i*2+2, 2);
        }
        for (int i=0; i<20; i++)
        {
            insertEnemies(p, n, i*2+1, 3);
        }
        for (int i=0; i<numberOfEnemies-40; i++)
        {
            insertEnemies(p, n, i*2+2, 4);
        }
    }
    if (poziom == 4)
    {
        return 0;
    }
    screen_init();
    porgramSpeed(7);  // W PRZYPADKU ZBYT DUZEJ SZYBKOSCI, NALEZY ZMNIEJSZYC TA WARTOSC
    while (doloop)
    {
    current_getch = getch();
    if (current_getch == 113) doloop = 0;
    if (score == numberOfEnemies) doloop = 0;
    delete_player();
    shoot();
    shootDown(p);
    bombA();
    bombB();
    bombC();
    if (life() == 0) doloop = 0;
    moveEnemies1(p);
    moveEnemies2(p);
    displayEnemies(p);
    player_control();
    update_display();
    counterS++; counterE++; counterA++; counterB++, counterB1++;
    Sleep(1);
    }
    for (int i = 0; i<numberOfEnemies; i++)  clearList(p,n);
    endwin();
    cout << "GAME OVER" << endl;
    cout << "YOUR SCORE: " << score;
    Sleep(3000);
    return 0;
}
