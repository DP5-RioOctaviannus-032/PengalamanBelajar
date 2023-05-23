#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int panjang = 50;
const int lebar = 20;
bool gameOver = false;
int score=0;

void gotoxy(short x, short y){
    //COORD adalah struct yg berisi 2 variabel tipe SHORT, yaitu X dan Y
    COORD k = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), k);
    //Fungsi SetConsoleCursorPosition() digunakan untuk menetapkan posisi cursor di layar. 
    //Fungsi ini memiliki dua parameter, yaitu hConsoleOutput dan dwCursorPosition. 
    //hConsoleOutput adalah parameter untuk handle ke layar. Handle ke layar didapat dengan menggunakan fungsi GetStdHandle(). 
    //Sedangkan dwCursorPosition adalah sebuah parameter bertipe struct COORD yang digunakan untuk menentukan posisi cursor di layar.
    
    //Fungsi GetStdHandle digunakan untuk mendapatkan handle ke layar. 
    //Fungsi ini mempunyai satu parameter, yaitu nStdHandle. nStdHandle adalah parameter untuk standard device. 
    //Dalam kasus ini, karena yang yang akan dilakukan adalah output ke layar, maka standard device yang digunakan adalah standard output device, yaitu STD_OUTPUT_HANDLE.

    //Untuk menggunakan fungsi gotoxy harus menggunakan #include <windows.h>. Selain itu harus membuat fungsi void gotoxy(){} . Dimana di dalam fungsi tersebut terdapat COORD, fungsi COORD adalah sebagai tipe data koordinat, SetConsoleCursorPosition() yang berfungsi untuk mengeset dari posisi x=0 dan y=0 ke posisi  di x dan y yang telah di set oleh programmer.
}

enum arah{STOP=0, UP, LEFT, DOWN, RIGHT};
arah gerak;

void batas(){
    system("cls");
    // system("color 07");
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
            // system("color 04");
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
            // gerak = STOP;
            jlhEkor=0;
        }
        void gambar(){
            // system("color 0A");
            for(int i = 0; i <= jlhEkor; i++){
                gotoxy(ekorX[i],ekorY[i]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                cout<<"*";
            }
        }
        void aturan(){
            //contoh:  x=kepala
                    //sebelum update
                    // tail[0]=4
                    // tail[1]=3
                    // tail[2]=2
                    // tail[3]=1

                    //sesudah update
                    // tail[0]=5
                    // tail[1]=3
                    // tail[2]=2
                    // tail[3]=1
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
    // Ular snake;
    // Target target;
    target.update();
    snake.state_awal();
    while(!gameOver){
        batas();
        target.gambar();
        snake.gambar();
        input();
        snake.aturan();
        Sleep(200);
    }
    gotoxy((panjang/2)-2, lebar+3);
    cout<<"GAME OVER!!";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    return 0;
}