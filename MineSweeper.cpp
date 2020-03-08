#include <conio.h>
#include <iostream>
#include <time.h>
#include <windows.h>
using namespace std;

#define word unsigned short

int N = 0, M = 0, mineNum = 0;
char fx[8][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}};

bool genmap(char *map);
void operate(char *map, word &x, word &y);
bool rsolve(char *map, word x, word y);
void solve(char *map, word x, word y);

int main()
{
    cin >> N >> M >> mineNum;
    //getchar();
    char *map = (char *)malloc(sizeof(char) * N * M);
    /*
    for (word i = 0; i < N; i++)
        for (word j = 0; j < M; j++)
            cin >> map[i * M + j];
*/
    system("cls");
    genmap(map);
    for (word i = 0; i < N; i++)
    {
        for (word j = 0; j < M; j++)
            cout << map[i * M + j];
        cout << '\n';
    }
    word x = 0, y = 0;
    while (true)
    {
        operate(map, x, y);
        system("cls");
        if (rsolve(map, x, y))
            break;
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
    /*
    word *mine = (word *)malloc(sizeof(word) * mineNum), loc1 = 0, loc2 = 0, r; //loc1: mine[] loc, loc2: map loc
    loc2 = rand() % len;
    mine[0] = loc2, map[loc2] = '*';
    for (word i = 1; i < mineNum; i++)
    {
        r = rand() % len; //!!!!!!!!!!!!
        while ((r + loc2 + 1) % len > mine[loc1])
            r -= (mine[loc1] + len - loc2) % len, loc2 = mine[loc1], loc1 = (loc1 + 1) % i;
        loc2 = (r + loc2 + 1) % len;
        while (loc2 == mine[loc1])
            loc2 = (loc2 + 1) % len, loc1 = (loc1 + 1) % i;
        mine[i] = loc2, map[loc2] = '*';
    }
    */
    return true;
}
void operate(char *map, word &x, word &y)
{
    static HANDLE m = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cxy = {y, x};
    SetConsoleCursorPosition(m, cxy);
    int c;
    while (true)
    {
        if (_kbhit())
        {
            c = _getch();
            if (c == 'z') //open
                break;
            if (c == 'x') //mark
                continue;
            if (c == 224)
                switch (c = _getch())
                {
                case 72: //up
                    if (x > 0)
                    {
                        x--;
                        cout << "\033[1A";
                    }
                    break;
                case 80: //down
                    if (x < N - 1)
                    {
                        x++;
                        cout << "\033[1B";
                    }
                    break;
                case 75: //left
                    if (y > 0)
                    {
                        y--;
                        cout << "\033[1D";
                    }
                    break;
                case 77: //right
                    if (y < M - 1)
                    {
                        y++;
                        cout << "\033[1C";
                    }
                    break;
                }
        }
    }
}
bool rsolve(char *map, word x, word y)
{
    if (map[x * M + y] == '*')
    {
        cout << "GG\n";
        return true; //GG
    }
    else
    {
        solve(map, x, y);
        word num = 0;
        char c;
        for (word i = 0; i < N; i++)
        {
            for (word j = 0; j < M; j++)
            {
                c = map[i * M + j];
                if (c == '.')
                    num++;
                if (c != '*')
                    cout << c;
                else
                    cout << '.';
            }
            cout << '\n';
        }
        if (num == 0)
        {
            cout << "solved!!!\n";
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
        if (map[nx * M + ny] == '*')
            map[x * M + y]++;
        if (map[nx * M + ny] == '.')
            next[num++] = i;
    }
    if (map[x * M + y] != '0')
        return;
    for (word i = 0; i < num; i++)
        solve(map, fx[next[i]][0] + x, fx[next[i]][1] + y);
}