#include <conio.h>
#include <iostream>
#include <time.h>
using namespace std;

#define word unsigned short

int N = 10, M = 20, mineNum = 35, notMine = 0;
clock_t start_t;
bool debug = false;
char fx[8][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}};

bool genmap(char *map);
char operate(char *map, word &x, word &y);
bool rsolve(char *map, word x, word y);
void solve(char *map, word x, word y);
void print(char *map);
void printdot(char *map, char c);

int main()
{
    char *map = (char *)malloc(sizeof(char) * N * M), flag = 0;
    while (true)
    {
        debug = false;
        if (genmap(map) == false)
            cout << "error parser!";
        else
        {
            print(map);
            word x = 0, y = 0;
            start_t = clock();
            while (true)
            {
                if ((flag = operate(map, x, y)) != 0)
                    break;
                if (rsolve(map, x, y))
                    break;
                cout << "\033[1;1H\033[34mn=" << notMine << "  \n"; //small trick
            }
        }
        switch (flag)
        {
        case 0:
            system("pause");
            break;
        case 1:
            N = 10, M = 20, mineNum = 35;
            break;
        case 2:
            N = 20, M = 40, mineNum = 160;
            break;
        case 3:
            N = 28, M = 100, mineNum = 700;
            break;
        case 4:
            cin >> N >> M >> mineNum;
            break;
        case 'q':
            return 0;
        }
        free(map);
        map = (char *)malloc(sizeof(char) * N * M);
    }
    return 0;
}

bool genmap(char *map)
{
    int len, tmp;
    notMine = len = N * M;
    if (len < mineNum)
        return false;
    for (word i = 0; i < len; i++)
        map[i] = 'O';
    srand((unsigned)time(NULL));

    for (word i = 0; i < mineNum; i++)
    {
        if (map[tmp = rand() % len] == '*')
            continue;
        map[tmp] = '*', notMine--;
    }

    return true;
}

char operate(char *map, word &x, word &y)
{
    int c, tt = 0;
    bool flg;
    while (true)
    {
        tt++;
        if (tt >> 8)
        {
            tt = 0;
            cout << "\033[1;10H\033[35mtime=" << (clock() - start_t) / CLOCKS_PER_SEC << "s\n";
        }

        cout << "\033[" << x + 2 << ';' << y + 1 << 'H';
        if (_kbhit())
        {
            c = _getch();
            switch (c)
            {
            case 'z':
            case 'Z':
                return 0;
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;
            case 'q':
            case 'Q':
                return 'q';
            case 'r':
            case 'R':
                return 'r';
            case 'd':
            case 'D':
                debug = debug == true ? false : true;
                print(map);
            }
            if (c == 'x' || c == 'X') //mark
            {
                flg = true;
                switch (map[x * M + y])
                {
                case 'O':
                    map[x * M + y] = ',';
                    break;
                case ',':
                    map[x * M + y] = 'O';
                    break;
                case '*':
                    map[x * M + y] = 'X';
                    break;
                case 'X':
                    map[x * M + y] = '*';
                    break;
                default:
                    flg = false;
                }
                if (flg)
                    printdot(map, map[x * M + y]);
            }
            if (c == 224)
                switch (c = _getch())
                {
                case 72: //up
                    if (x > 0)
                        x--;
                    break;
                case 80: //down
                    if (x < N - 1)
                        x++;
                    break;
                case 75: //left
                    if (y > 0)
                        y--;
                    break;
                case 77: //right
                    if (y < M - 1)
                        y++;
                    break;
                }
        }
    }
}

bool rsolve(char *map, word x, word y)
{
    if (map[x * M + y] == '*')
    {
        debug = true;
        print(map);
        cout << "\033[31mGG\n";
        return true; //GG
    }
    else
    {
        solve(map, x, y);
        //print(map);
        if (notMine == 0)
        {
            cout << "\033[" << N + 2 << ';' << 1 << 'H';
            cout << "\033[32msolved!!!\n";
            return true; //solved
        }
    }
    return false; //next round
}

void solve(char *map, word x, word y)
{
    map[x * M + y] = '0';
    notMine--;
    word nx, ny, num = 0, next[8] = {0};
    for (word i = 0; i < 8; i++)
    {
        nx = fx[i][0] + x, ny = fx[i][1] + y;
        if (nx >= N || ny >= M)
            continue;
        if (map[nx * M + ny] == '*' || map[nx * M + ny] == 'X')
            map[x * M + y]++;
        if (map[nx * M + ny] == 'O' || map[nx * M + ny] == ',')
            next[num++] = i;
    }
    printdot(map, map[x * M + y]);
    if (map[x * M + y] != '0')
        return;
    for (word i = 0; i < num; i++)
    {
        nx = fx[next[i]][0] + x, ny = fx[next[i]][1] + y;
        if (map[nx * M + ny] == 'O' || map[nx * M + ny] == ',')
        {
            cout << "\033[" << nx + 2 << ';' << ny + 1 << 'H';
            solve(map, nx, ny);
        }
    }
}

void print(char *map)
{
    system("cls");
    cout << "\033[34mn=" << notMine << '\n';
    char c;
    if (debug)
    {
        for (word i = 0; i < N; i++)
        {
            for (word j = 0; j < M; j++)
            {
                c = map[i * M + j];
                if (c == ',' || c == 'O')
                    cout << "\033[33m"; //yellow
                else if (c == 'X' || c == '*')
                    cout << "\033[31m"; //red
                else if (c == '0')
                    cout << "\033[30m"; //black
                else
                    cout << "\033[32m"; //green
                cout << c;
            }
            cout << '\n';
        }
    }
    else
    {
        for (word i = 0; i < N; i++)
        {
            for (word j = 0; j < M; j++)
            {
                c = map[i * M + j];
                if (c == '*' || c == 'O')
                    cout << "\033[33m" << 'O'; //yellow
                else if (c == ',' || c == 'X') //x: marked and is mine
                    cout << "\033[31m" << 'X'; //red
                else if (c == '0')
                    cout << "\033[30m0";
                else
                    cout << "\033[32m" << c; //green
            }
            cout << '\n';
        }
    }
}

void printdot(char *map, char c)
{
    if (debug)
    {
        if (c == ',' || c == 'O')
            cout << "\033[33m"; //yellow
        else if (c == 'X' || c == '*')
            cout << "\033[31m"; //red
        else if (c == '0')
            cout << "\033[30m"; //black
        else
            cout << "\033[32m"; //green
        cout << c;
    }
    else
    {
        if (c == '*' || c == 'O')
            cout << "\033[33m" << 'O'; //yellow
        else if (c == ',' || c == 'X') //x: marked and is mine
            cout << "\033[31m" << 'X'; //red
        else if (c == '0')
            cout << "\033[30m0";
        else
            cout << "\033[32m" << c; //green
    }
}