#include <conio.h>
#include <iostream>
#include <time.h>
using namespace std;

#define word unsigned short

int N = 0, M = 0, mineNum = 0;
char fx[8][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}};

bool genmap(char *map);
void operate(char *map, word &x, word &y);
bool rsolve(char *map, word x, word y);
void solve(char *map, word x, word y);
word print(char *map);
void debug(char *map);

int main()
{
    cout << "input rows, cols, mine number:\n";
    cin >> N >> M >> mineNum;
    char *map = (char *)malloc(sizeof(char) * N * M);
    system("cls");
    if (genmap(map) == false)
        cout << "error parser!";
    else
    {
        print(map);
        word x = 0, y = 0;
        while (true)
        {
            operate(map, x, y);
            if (rsolve(map, x, y))
                break;
        }
    }

    system("pause");
    return 0;
}

bool genmap(char *map)
{
    word len = N * M;
    if (len < mineNum)
        return false;
    for (word i = 0; i < len; i++)
        map[i] = '.';
    srand((unsigned)time(NULL));

    for (word i = 0; i < mineNum; i++)
        map[rand() % len] = '*';

    return true;
}
void operate(char *map, word &x, word &y)
{
    int c;
    bool flg;
    while (true)
    {
        cout << "\033[" << x + 1 << ';' << y + 1 << 'H';
        if (_kbhit())
        {
            c = _getch();
            if (c == 'z' || c == 'Z') //open
                break;
            if (c == 'x' || c == 'X') //mark
            {
                flg = true;
                switch (map[x * M + y])
                {
                case '.':
                    map[x * M + y] = ',';
                    break;
                case ',':
                    map[x * M + y] = '.';
                    break;
                case '*':
                    map[x * M + y] = 'x';
                    break;
                case 'x':
                    map[x * M + y] = '*';
                    break;
                default:
                    flg = false;
                }
                if (flg)
                    print(map);
            }
            if (c == 'd' || c == 'D') //debug(cheat?)
                debug(map);
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
        debug(map);
        cout << "\033[31mGG\n";
        return true; //GG
    }
    else
    {
        solve(map, x, y);
        if (print(map) == 0)
        {
            cout << "\033[32msolved!!!\n";
            return true; //solved
        }
    }
    return false; //next round
}

void solve(char *map, word x, word y)
{
    map[x * M + y] = '0';
    word nx, ny, num = 0, next[8] = {0};
    for (word i = 0; i < 8; i++)
    {
        nx = fx[i][0] + x, ny = fx[i][1] + y;
        if (nx >= N || ny >= M)
            continue;
        if (map[nx * M + ny] == '*' || map[nx * M + ny] == 'x')
            map[x * M + y]++;
        if (map[nx * M + ny] == '.' || map[nx * M + ny] == ',')
            next[num++] = i;
    }
    if (map[x * M + y] != '0')
        return;
    for (word i = 0; i < num; i++)
        solve(map, fx[next[i]][0] + x, fx[next[i]][1] + y);
}

word print(char *map)
{
    system("cls");
    //cout << "\033[34mz: open | x: mark | d: debug\n";
    word num = 0;
    char c;
    for (word i = 0; i < N; i++)
    {
        for (word j = 0; j < M; j++)
        {
            c = map[i * M + j];
            if (c == '.' || c == ',') //,: marked and not mine
                num++;
            if (c == '*' || c == '.')
                cout << "\033[33m.";       //yellow
            else if (c == ',' || c == 'x') //x: marked and is mine
                cout << "\033[31mx";       //red
            else if (c == '0')
                cout << "\033[30m0";
            else
                cout << "\033[32m" << c; //green
        }
        cout << '\n';
    }
    return num;
}

void debug(char *map)
{
    system("cls");
    //cout << "\033[33mOps...\n";
    char c;
    for (word i = 0; i < N; i++)
    {
        for (word j = 0; j < M; j++)
        {
            c = map[i * M + j];
            if (c == ',' || c == '.')
                cout << "\033[33m"; //yellow
            else if (c == 'x' || c == '*')
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