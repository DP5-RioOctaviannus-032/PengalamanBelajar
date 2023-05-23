#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int panjang = 50;
const int lebar = 20;
bool gameOver = false;
int score=0;

void gotoxy(short x, short y){
    COORD k = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), k);
}

enum arah{STOP=0, UP, LEFT, DOWN, RIGHT};
arah gerak;

void batas(){
    system("cls");
    for (int i = 0; i < panjang; i++){
        gotoxy(i,0);
        //foreground = text, background = background
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED);
        cout<<" ";
        gotoxy(i,lebar - 1);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED);
        cout<<" ";
    }
    cout<<endl;

    for (int i = 1; i < lebar-1; i++){
        gotoxy(0,i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED);
        cout<<" ";
        gotoxy(panjang - 1,i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED);
        cout<<" ";
    }

    gotoxy((panjang/2)-4, lebar+2);
    cout<<"Score Anda : "<<score;
}

class Target{
    public:
        int targetX, targetY; //lokasi target
        void update(){
            targetX = rand() % (panjang-1);
            targetY = rand() % (lebar-1);

            if(targetX == 0){
                targetX += 1;
            }
            if(targetY == 0){
                targetY += 1;
            }
        }
        void gambar(){
            gotoxy(targetX, targetY);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            cout<<"O";
        }
} target;

class Ular{
    private:
        int x, y, ekorX[1000], ekorY[1000], jlhEkor;

    public:
        void state_awal(){
            x = panjang / 2;
            y = lebar / 2;
            ekorX[0] = x;
            ekorY[0] = y;
            jlhEkor=0;
        }
        void gambar(){
            for(int i = 0; i <= jlhEkor; i++){
                gotoxy(ekorX[i],ekorY[i]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                cout<<"*";
            }
        }
        void aturan(){
            //ekor[0]=4
            //ekor[1]=3
            //ekor[2]=2

            //ekor[0]=5
            //ekor[1]=4
            //ekor[2]=3
            //ekor ular
            int prevEX = ekorX[0]; //menyimpan posisi X ekor ke 0(kepala) sebelum diupdate
            int prevEY = ekorY[0];
            ekorX[0] = x; //menyimpan posisi X ekor ke 0(kepala) setelah diupdate(sekarang)
            ekorY[0] = y;
            int prev2EX, prev2EY;
            //menyimpan posisi ekor seterusnya
            for(int i = 1; i <= jlhEkor; i++){
                prev2EX = ekorX[i]; //untuk menyimpan posisi X ekor[i] sebelum diupdate (supaya bisa dipass ke prevEX karena prevEX mau dipake, ibarat variabel sementara untuk menukar posisi) 
                prev2EY = ekorY[i];
                ekorX[i] = prevEX; //untuk update posisi X ekor[i] dengan posisi X ekor[i-1] sebelum diupdate
                ekorY[i] = prevEY;
                prevEX = prev2EX; //untuk menyimpan posisi X ekor[i] sebelum diupdate
                prevEY = prev2EY;
            }

            //logika gerakan ular
            switch(gerak){
                case UP:
                    y--;
                    break;
                case LEFT:
                    x--;
                    break;
                case DOWN:
                    y++;
                    break;
                case RIGHT:
                    x++;
                    break;
                default:
                    break;
            }

            //ketika ular menabrak tembok
            if((x <= 0) || (x >= panjang) || (y <= 0) || (y >= lebar)){
                gameOver = true;
            }

            //ketika ular menabrak ekor
            for(int i = 1; i <= jlhEkor; i++){
                if(x == ekorX[i] && y ==ekorY[i]){
                    gameOver = true;
                }
            }

            //ketika ular mencapai target
            if(x == target.targetX && y == target.targetY){
                score += 10;
                jlhEkor++;
                target.update();
            }
        }
} snake;

void input(){
    //kbhit = keyboard hit
    if (_kbhit()){
        //getch = get character
        switch(getch()){
            case 'w':
                gerak = UP;
                break;
            case 'a':
                gerak = LEFT;
                break;
            case 's':
                gerak = DOWN;
                break;
            case 'd':
                gerak = RIGHT;
                break;
            default:
                break;
        }
    }
}

int main(){
    target.update();
    snake.state_awal();
    while(!gameOver){
        batas();
        target.gambar();
        snake.gambar();
        input();
        snake.aturan();
        Sleep(100);
    }
    gotoxy((panjang/2)-2, lebar+3);
    cout<<"GAME OVER!!";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    return 0;
}
