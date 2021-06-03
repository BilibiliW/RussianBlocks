#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
using namespace std;

#define X_DingWei   20
#define Y_DingWei   5
#define X_Width     12
#define Y_Hight     20
#define X_Absolute  X_DingWei*2+2
#define Y_Absolute  Y_DingWei+1

int x_actul;
int y_actul;
int top=0;
int game_over=0;
void get_pos_arg(int x, int y){
    x_actul=x*2+X_Absolute;
    y_actul=y+Y_Absolute;
}

int block_id_;
int change_=1;
int map_block[20][12]={0};
int map_block22[6][4]={
0,0,0,0,
0,1,0,0,
0,0,0,1,
0,1,1,0,
};
const int Blocks[15][8]=
{
{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,0,3},
{0,0,1,0,0,1,1,1},
{0,0,1,0,1,1,1,2},{0,1,1,1,2,0,2,1},{0,0,0,1,0,2,1,2},{0,0,0,1,1,0,2,0},
{1,0,1,1,1,2,0,2},{0,0,0,1,1,1,2,1},{0,0,0,1,0,2,1,0},{0,0,1,0,2,0,2,1},
{0,0,0,1,1,1,1,2},{0,1,1,0,1,1,2,0},
{0,1,0,2,1,0,1,1},{0,0,1,0,1,1,2,1}
};
class RussiaBlocks{
public:
    enum colors{
        RED,
        BLUE,
        GREEN,
        BLACK,
        WHITE,
    };
    enum director{
        left,
        right,
        up,
        down,
    };
    void set_pos(int x, int y);
    void Screen_init(int x, int y);
    //void set_Color(enum colorss);
    void set_Color(int colorss);
    void draw_block(int B_id,int i,int j);
    void draw_block(int B_id,int i,int j ,int Color);
    void flush_block(int B_id,int i,int j);
    void redraw_block(int B_id, int dir_,int x, int y);
    void change_shape(int id);
    void key_scan();
    int check_(int B_id,int x, int y);
    int check_map();
    void redraw_map();
    void eliminate_line(int line_No);
    void mark();
    int get_block_id();
    int Block_id=0;
    int dir_=down;

//private:
    int X_pos=20;
    int Y_pos=10;
    int X_middle=0;
    int dis_x,dis_y;
    int y_limit=0;

    void testmap(){
        set_Color(BLUE);
        for(int i=0;i<20;i++){
            for(int j=0;j<12;j++){
                set_pos(j-20,i);
                if(map_block[i][j]==1)
                    set_Color(RED);
                else
                    set_Color(BLUE);
                cout<<"¡ö";
            }
    }
}
};
void RussiaBlocks::redraw_map(){
    for(int i=0;i<Y_Hight;i++){
        for(int j=0;j<X_Width;j++){
            set_Color(BLUE);
            set_pos(j,i);
            if(map_block[i][j]==0)
                cout<<"  ";
            else
                cout<<"¡ö";
        }
    }
}
void RussiaBlocks::eliminate_line(int line_No){
    for(int i=line_No;i>top;i--){
        for(int j=0;j<X_Width;j++){
            map_block[i][j]=map_block[i-1][j];
        }
    }
}
int RussiaBlocks::check_map(){
    int is_ok=0,is_top=0,result=1;
    for(int i=Y_Hight-1;i>=0;i--){
        is_ok=0;
        is_top=0;
        for(int j=0;j<X_Width;j++){
            if(i==0){
                if(map_block[i][j]==1)
                    game_over=1;
            }
            if(map_block[i][j]==1)
                is_ok++;
            else
                is_top++;
        }
        if(is_top==X_Width){
            top=Y_Hight-1-i;
            set_pos(-20,-3);cout<<top;
            break;
        }
        if(is_ok == X_Width){
            eliminate_line(i);
            //redraw_map();
            i=Y_Hight;
            result=2;
        }
    }
//    if(top>=Y_Hight-0)
//        game_over=1;
    return result;
}
int RussiaBlocks::check_(int B_id,int x, int y){
    int map_i,map_j;
    for(int i=0;i<4;i++){
        map_i=Blocks[B_id][i*2]+x;
        map_j=Blocks[B_id][i*2+1]+y;
        if(map_j >= y_limit || map_i>= X_Width || map_i < 0 || map_block[map_j][map_i]==1)
            return 1;
    }
    return 0;
}
void RussiaBlocks::mark(){
    int map_i,map_j;
    for(int i=0;i<4;i++){
        map_i=Blocks[block_id_][i*2]+dis_x;
        map_j=Blocks[block_id_][i*2+1]+dis_y;
        map_block[map_j][map_i]=1;
    }
}
void RussiaBlocks::redraw_block(int B_id,int dir_,int x,int y){
    int result=0;
    int redraw_map_flag=0;
    flush_block(block_id_,x,y);
    switch(dir_){
    case left:
        if(result=check_(B_id,dis_x-1,dis_y) == 0)
            dis_x-=1;
        break;
    case right:
        if(result=check_(B_id,dis_x+1,dis_y) == 0)
            dis_x+=1;
        break;
    case up:
        change_shape(B_id);
        break;
    case down:
        if(result=check_(B_id,dis_x,dis_y+1) == 1){
            mark();
            redraw_map_flag=check_map();
            change_=1;
        }
        else
            dis_y+=1;
        break;
    }
    if(redraw_map_flag==1)
        draw_block(block_id_,dis_x,dis_y,BLUE);
    else if(redraw_map_flag==2)
        redraw_map();
    else
        draw_block(block_id_,dis_x,dis_y);
}
void RussiaBlocks::change_shape(int id){
    int result;
    switch(id){
        case 0:
            if(result=check_(1,dis_x,dis_y)==0)
            block_id_=1;
            break;
        case 1:
            if(result=check_(0,dis_x,dis_y)==0)
            block_id_=0;
            break;
        case 3:
            if(result=check_(4,dis_x,dis_y)==0)
                block_id_=4;
            break;
        case 4:
            if(result=check_(5,dis_x,dis_y)==0)
                block_id_=5;
            break;
        case 5:
            if(result=check_(6,dis_x,dis_y)==0)
                block_id_=6;
            break;
        case 6:
            if(result=check_(3,dis_x,dis_y)==0)
                block_id_=3;
            break;
        case 7:
            if(result=check_(8,dis_x,dis_y)==0)
                block_id_=8;
            break;
        case 8:
            if(result=check_(9,dis_x,dis_y)==0)
                block_id_=9;
            break;
        case 9:
            if(result=check_(10,dis_x,dis_y)==0)
                block_id_=10;
            break;
        case 10:
            if(result=check_(7,dis_x,dis_y)==0)
                block_id_=7;
            break;
        case 11:
            if(result=check_(12,dis_x,dis_y)==0)
            block_id_=12;
            break;
        case 12:
            if(result=check_(11,dis_x,dis_y)==0)
                block_id_=11;
            break;
        case 13:
            if(result=check_(14,dis_x,dis_y)==0)
                block_id_=14;
            break;
        case 14:
            if(result=check_(13,dis_x,dis_y)==0)
                block_id_=13;
            break;
    }
}
int RussiaBlocks::get_block_id(){
    return rand()%15;
}
void RussiaBlocks::set_pos(int x, int y){
    get_pos_arg(x,y);
    COORD pos={x_actul, y_actul};
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(std_out, pos);
}
//void RussiaBlocks::set_Color(enum colors){
void RussiaBlocks::set_Color(int colors){
    HANDLE std_out=GetStdHandle(STD_OUTPUT_HANDLE);
    switch (colors){
        case RED:
            SetConsoleTextAttribute(std_out,FOREGROUND_RED );
            break;
        case BLUE:
            SetConsoleTextAttribute(std_out,FOREGROUND_BLUE);
            break;
        case GREEN:
            SetConsoleTextAttribute(std_out,FOREGROUND_GREEN);
            break;
//        case WHITE:
//            SetConsoleTextAttribute(std_out,FOREGROUND_WHITE);
//            break;
//        case BLACK:
//            SetConsoleTextAttribute(std_out,FOREGROUND_BLACK);
//            break;

    }
}

void RussiaBlocks::Screen_init(int x_leng, int y_width){

    set_Color(RED);

    set_pos(-1, -1);
    for(int i=0; i<x_leng+2; i++)
        cout<<"¡ö";

    set_pos(-1, y_width);
    for(int i=0; i<x_leng+2; i++)
        cout<<"¡ö";

    for(int i=0;i<y_width+1;i++){
        set_pos(-1, i);
        cout<<"¡ö";
        set_pos(x_leng, i);
        cout<<"¡ö";
    }

    X_middle=x_leng/2-1;
    y_limit=Y_Hight;
}

void RussiaBlocks::draw_block(int B_id, int x, int y){
    set_Color(GREEN);
    for(int i=0;i<4;i++){
        //if(Blocks[B_id][i*2+1]+y > Y_Hight-top)game_over=1;
        set_pos(Blocks[B_id][i*2]+x,Blocks[B_id][i*2+1]+y);
        cout<<"¡ö";
    }
}
void RussiaBlocks::draw_block(int B_id, int x, int y,int Color){
    set_Color(Color);
    for(int i=0;i<4;i++){
        set_pos(Blocks[B_id][i*2]+x,Blocks[B_id][i*2+1]+y);
        cout<<"¡ö";
    }
}
void RussiaBlocks::flush_block(int B_id,int x, int y){
    //set_Color(GREEN);
    for(int i=0;i<4;i++){
        set_pos(Blocks[B_id][i*2]+x,Blocks[B_id][i*2+1]+y);
        cout<<"  ";
    }

}
void RussiaBlocks::key_scan(){
		if(_kbhit())
		{
			int key, key2;
			key = _getch();
			if (key==224)
			{
				key2 = _getch();

				if (key2 == 72)
				{
					dir_=up;
				}
                else if (key2== 80)
				{
                    dir_=down;
				}
				else if (key2== 75)
				{
                    dir_=left;
				}
				else if (key2== 77)
				{
                    dir_=right;
				}
				redraw_block(block_id_,dir_,dis_x,dis_y);
			}
			else if(key == 32){
                //Pause();
                while(1){
                    if(_kbhit()){
                        key = _getch();
                        if(key == 32)break;
                        Sleep(20);
                    }
                }

			}
		}
}

int main()
{
    RussiaBlocks mao;
    mao.Screen_init(X_Width,Y_Hight);
    mao.Block_id= rand()%15;

    int dir_=mao.down;
    int i_times=0;

    while(!game_over){
        if(change_ == 1){
            block_id_=mao.get_block_id();
            mao.dis_x=mao.X_middle;
            mao.dis_y=0;
            mao.draw_block(block_id_,mao.dis_x,mao.dis_y);
            change_=0;
        }
        if(i_times++ <= 20){
            mao.key_scan();
            Sleep(20);
            mao.testmap();
        }
        else{
            i_times=0;
            mao.redraw_block(block_id_,dir_,mao.dis_x,mao.dis_y);
        }
    }
    return 0;
}
