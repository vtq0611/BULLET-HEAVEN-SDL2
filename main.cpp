#include"game.h"

using namespace std;

struct toaDo
{
    int x, y;
};

class doiTuong{
public:
    SDL_Texture* nv;
    SDL_Rect khung_nv;
    SDL_Renderer* renderer;

    void khoiTao (string s, SDL_Renderer* _renderer)
    {
        renderer = _renderer;
        nv = loadTexture (s, renderer);
        SDL_QueryTexture (nv, NULL, NULL, &khung_nv.h, &khung_nv.w);
    }

    void setKichThuoc (float delta_h, float delta_w)
    {
        khung_nv.h /= delta_h;
        khung_nv.w /= delta_w;
    }

    void setToaDo (int x, int y)
    {
        khung_nv.x = x;
        khung_nv.y = y;
    }

    void hienThi()
    {
        SDL_RenderCopy (renderer, nv, NULL, &khung_nv);
    }

    /*
    void Animation(){
        int denta = 1;
        for (int i = 0; i<10; i++)
        {
            if (i>5) denta = -denta;
            khung_nv.h += denta*5;
            khung_nv.w += denta*5;
            this -> hienThi();
            SDL_RenderPresent(renderer);
            SDL_Delay(250);
        }
    }
    */
};

void Menu (string bg, SDL_Renderer* renderer)
{
    SDL_Texture *back_ground = loadTexture(bg, renderer);
    SDL_RenderCopy(renderer, back_ground, NULL, NULL);
    doiTuong play[4];
    play[0].khoiTao("./anh//BatDau.bmp", renderer);
    play[1].khoiTao("./anh//ChoiLai.bmp", renderer);
    play[2].khoiTao("./anh//AmThanh.bmp", renderer);
    play[3].khoiTao("./anh//CaiDat.bmp", renderer);

     for (int i = 0; i<4; i++)
        {
            play[i].setToaDo(200 + i*play[i].khung_nv.w*2, 300);
            play[i].setKichThuoc(1.5, 1.5);
        }

    int selection = 0;

    int denta = 1;
    for (int j = 0; j<10; j++)
    {
        if (j>5) denta = -denta;
        play[selection].khung_nv.h += denta*5;
        play[selection].khung_nv.w += denta*5;
        SDL_RenderCopy(renderer, back_ground, NULL, NULL);
        play[selection].hienThi();
        for (int i = 0; i<4; i++)
        {
            if (i == j) continue;
            play[i].hienThi();
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(250);
    }


    SDL_RenderPresent (renderer);

}

class dan : public doiTuong{
 public:
     int type; //0: lua, 1: gio, 2: bang
     int dam;
     int speed;
     int slow;

     void khoiTao (SDL_Renderer* _renderer, int stype)
     {
         dam = 50;
         speed = 10;
         slow = 0;
         type = stype;
         string s;
         switch(type){
            case 0:
            {
                dam = 50;
                s = "./anh//dan1.bmp";
                break;
            }

            case 1: {
                speed = 7;
                s = "./anh//dan2.bmp";
                break;
            }

            case 2: {
                slow = 3;
                s = "./anh//dan3.bmp";
                break;
            }
         }

         doiTuong :: khoiTao(s, _renderer);
     }

     void effects (int sdam, int sspeed){
        dam += sdam;
        speed += sspeed;
     }

     void danDiChuyen (int deltaX, int deltaY)
    {
        khung_nv.x += deltaX*speed;
        khung_nv.y += deltaY*speed;
    }
};

class nhanVat : public doiTuong /// tau
{
public:
    int hp_max, hp;
    int speed;
    int dam;
    vector <dan> dayDan;
    void khoiTao (string s, SDL_Renderer* _renderer, int shp, int sspeed, int sdam)
    {
        doiTuong :: khoiTao(s, _renderer);
        hp_max = shp; hp = hp_max;
        speed = sspeed;
        dam = sdam;
    }

    void diChuyen (int deltaX, int deltaY)
    {
        if ((khung_nv.x <= 0 && deltaX == -1)
            || (khung_nv.x >= (SCREEN_WIDTH - khung_nv.w) && deltaX==1)
            || (khung_nv.y <= 0 && deltaY == -1)
            || (khung_nv.y >= (SCREEN_HEIGHT - khung_nv.h) && deltaY == 1))
        {
            //deltaX *= -1;
            //deltaY *= -1;
            return;
        }

        khung_nv.x += deltaX*speed;
        khung_nv.y += deltaY*speed;
        cout <<khung_nv.x << ' ' << khung_nv.y << endl;
    }

    void run ()
    {
        int animation = 0;
        string img[] = {"./anh//nv11.bmp", "./anh//nv12.bmp",
                        "./anh//nv13.bmp", "./anh//nv14.bmp",
                        "./anh//nv15.bmp", "./anh//nv16.bmp"};
        nv = loadTexture(img[animation], renderer);
        animation++;
        if (animation > 5) animation = 0;
    }

    void khoiTao(int type) {
        toaDo td_dan;
        td_dan.x = khung_nv.x + khung_nv.w/2;
        td_dan.y = khung_nv.y;

        dan Dan;
        Dan.khoiTao(renderer, type);
        Dan.setToaDo(td_dan.x, td_dan.y);
        Dan.setKichThuoc(2,4);
        dayDan.push_back(Dan);
        //cout << bullet.size() << endl;
    }

    void hienThi () {
        doiTuong :: hienThi();
        //cout << dayDan.size() << endl;
        for (int i = 0; i<dayDan.size(); i++)
        {
            dayDan[i].danDiChuyen(0, -1);
            if (dayDan[i].khung_nv.y<=0)
            {
                dayDan.erase(dayDan.begin()+i);
                i--;
                continue;
            }
            dayDan[i].hienThi();

        }
    }
};

class enemies : public doiTuong /// tau
{
public:
    int hp;
    int speed;
    int dam;
    vector <dan> dayDan;
    void khoiTao (string s, SDL_Renderer* _renderer, int shp,  int sspeed, int sdam)
    {
        doiTuong :: khoiTao(s, _renderer);
        speed = sspeed;
        dam = sdam;
    }

    void khoiTao(int type) {
        toaDo td_dan;
        td_dan.x = khung_nv.x + khung_nv.w/2;
        td_dan.y = khung_nv.y;

        dan Dan;
        Dan.khoiTao(renderer, type);
        Dan.setToaDo(td_dan.x, td_dan.y);
        Dan.setKichThuoc(2,4);
        dayDan.push_back(Dan);
        //cout << bullet.size() << endl;
    }

    void hienThidan () {
        doiTuong :: hienThi();
        //cout << dayDan.size() << endl;
        for (int i = 0; i<dayDan.size(); i++)
        {
            //di chuyen
            if (dayDan[i].khung_nv.y<=0)
            {
                dayDan.erase(dayDan.begin()+i);
                i--;
                continue;
            }
            dayDan[i].hienThi();

        }
    }
};

class listBoss{
public:
    vector <enemies> listboss;
    int type;
    void khoiTao (SDL_Renderer* renderer, int stype)
    {
        string s;
        type = stype;
        switch(type)
        {
        case 0:
            {
                enemies x;
                x.khoiTao("./anh//boss1.bmp", renderer, 200, 2, 100);
                for (int i = 0; i<20; i++)
                {
                    if (i<10) x.setToaDo(-i*(x.khung_nv.w*2), 100);
                    else x.setToaDo(SCREEN_WIDTH+(i%10)*(x.khung_nv.w*2), 100);
                    listboss.push_back(x);
                }
                break;
            }
        case 1:
            {
                enemies x;
                x.khoiTao("./anh//boss1.bmp", renderer, 200, 2, 100);
                for (int i = 0; i<20; i++)
                {
                    if (i<10) x.setToaDo(-i*(x.khung_nv.w*2), 100);
                    else x.setToaDo(SCREEN_WIDTH+(i%10)*(x.khung_nv.w*2), 100);
                    listboss.push_back(x);
                }
                break;
            }
        }
    }

    void hienThi()
    {
        for (int i =0; i<listboss.size(); i++) //listboss la vecto luu n dich
        {
            switch (type)
            {
            case 0 :
                {
                    listboss[i].khung_nv.x += listboss[i].speed;
                    listboss[i].khung_nv.y = 100 + 100*cos(listboss[i].khung_nv.x*3.14/180);
                    break;
                }
            case 1 :
                {
                    if (i<10)
                    {
                        listboss[i].khung_nv.x += listboss[i].speed;
                        listboss[i].khung_nv.y = 100 + 100*cos(listboss[i].khung_nv.x*3.14/180);
                    }
                    else
                    {
                        listboss[i].khung_nv.x -= listboss[i].speed;
                        listboss[i].khung_nv.y = 100 + 100*sin(listboss[i].khung_nv.x*3.14/180);
                    }
                    //cout << i << ' ' << listboss[i].khung_nv.x << ' ' << listboss[i].khung_nv.y << endl;
                    break;
                }
            }
            listboss[i].hienThi();
        }
    }
};

int main (int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer = NULL;
    SDL_Event g_event;

    initSDL (window, renderer);

    //Menu("./anh//menu.png", renderer);

    SDL_Texture* back_ground = loadTexture("./anh//bg.png", renderer);
    SDL_RenderCopy (renderer, back_ground, NULL, NULL); //cap nhat len man hinh hien thi

    toaDo td_rocket;
    td_rocket.x = 0;
    td_rocket.y = 640;
    nhanVat rocket;
    rocket.khoiTao("./anh//nv11.bmp", renderer, 2000, 100, 100);
    rocket.setToaDo(td_rocket.x, td_rocket.y);
    rocket.setKichThuoc(4,4);
    rocket.hienThi();

    listBoss boss;
    boss.khoiTao(renderer, 1);

    int deltaX = 0, deltaY = 0;
    int tocDoBan = 0;
    while (true)
    {
        tocDoBan++;
        if (SDL_PollEvent (&g_event)) ///neu co su kien ban phim
        {
            switch (g_event.type){
                case SDL_KEYDOWN: //co phim duoc bam
                {
                    cout << g_event.key.keysym.sym << endl;
                    switch (g_event.key.keysym.sym)
                    {
                        case SDLK_UP:                        {
                            rocket.diChuyen(0, -1);
                            //deltaX = 0;
                            //deltaY = -1;
                            break;
                        }
                        case SDLK_DOWN:
                        {
                            rocket.diChuyen(0, 1);
                            //deltaX = 0;
                            //deltaY = 1;
                            break;
                        }
                        case SDLK_LEFT:
                        {
                            rocket.diChuyen(-1, 0);
                            //deltaX = -1;
                            //deltaY = 0;
                            break;
                        }
                        case SDLK_RIGHT:
                        {
                            rocket.diChuyen(1, 0);
                            //deltaX = 1;
                            //deltaY = 0;
                            break;
                        }
                        case SDLK_SPACE:
                        {
                            if (tocDoBan >= 3)
                            {
                                rocket.khoiTao(0);
                                tocDoBan = 0;
                                break;
                            }
                        }
                        case SDLK_q:
                        {
                            if (tocDoBan >= 10)
                            {
                                rocket.khoiTao(1);
                                tocDoBan = 0;
                                break;
                            }

                        }
                        case SDLK_w:
                        {
                            if (tocDoBan >= 20)
                            {
                                rocket.khoiTao(2);
                                tocDoBan = 0;
                                break;
                            }

                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: //click chuot
                {

                    cout << g_event.button.x << ", " << g_event.button.y << endl;
                    break;
                }
            }
        }
        //rocket.diChuyen (deltaX, deltaY);
        //rocket.run();
        SDL_RenderCopy(renderer, back_ground, NULL, NULL);
        rocket.hienThi();
        boss.hienThi();
        SDL_RenderPresent (renderer);//xuat ra window
    }

    SDL_RenderPresent (renderer);
    getch();
    return 0;
}
