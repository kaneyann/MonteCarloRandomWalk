#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//--- 定数マクロ
#define STEP  10     // シミュレートするステップ数
#define AREA  20     // マップの広さ
#define NUM   500    // 乱数の個数
#define LIMIT 10     // 1行あたりの読み込み文字上限
enum Direction { UP, DOWN, RIGHT, LEFT};

//--- グローバル変数
int map[AREA][AREA] = {0};    // マップ
int data[NUM] = {0};          // 入力データ格納配列
int c = 0;                    // 使用する乱数のカウンタ

//--- 関数プロトタイプ宣言
int data_input();                      // データの読み込み
void print_map();                      // マップの表示
void five_simu(int num1, int num2);    // 5セットの点(4つ)のシミュレート
void four_simu(int num1, int num2);    // 4セットの点(4つ)のシミュレート
void start_simu();                     // 4セットの点(出発地点)のシミュレート
void move(int *row, int *col, int *direction);     // 移動処理
void cross_road(int *i, int *j, int *dir);         // 十字路の時の移動
void corner(int *i, int *j, int *dir);             // 曲がり角の時の移動
void t_junction_end(int *i, int *j, int *dir);     // 丁字路(突き当り)の時の移動
void t_junction_side(int *i, int *j, int *dir);    // 丁字路(横から)の時の移動
void show_drunkard(int i, int j);      // 酔っ払いの動きを表示
void calc_move(int *total);            // 総移動距離の計算

//============================================
// main関数
//============================================
int main(void)
{
    //###--- デバッグ用変数
    int sum_set = 0;
    int total_move = 0;    // 総移動距離
    int i, j;
    
    srand((unsigned)time(NULL));
    
    //--- データの生成方法1(テキストから乱数を読み込む)
    //if ( data_input() == -1 ) { return 0; }
    
    //--- データの生成方法2(ここで乱数を発生させる)
    for ( i = 0; i < NUM; i++ ) {
        data[i] = rand() % 10;
    }
    
    //--- 5セットの点(4つ)についてシミュレート
    five_simu(1, AREA-5);
    five_simu(5, AREA-5);
    five_simu(3, AREA-7);
    five_simu(3, AREA-3);
    
    //--- 4セットの点(4つ)についてシミュレート
    four_simu(2, AREA-6);
    four_simu(4, AREA-6);
    four_simu(2, AREA-4);
    four_simu(4, AREA-4);

    //--- 4セットの点(出発地点)についてシミュレート
    start_simu();
    
    // 総移動距離の計算
    calc_move(&total_move);
    
    //--- マップの表示
    print_map();
    
    // 合計セット数の計算
    for ( i = 0; i < AREA; i++ ) {
        for ( j = 0; j < AREA; j++ ) {
            sum_set += map[i][j];
        }
    }
    printf("合計セット数       = %d\n", sum_set);
    printf("使用した乱数の個数 = %d\n", c);
    printf("平均移動距離       = %d\n", total_move / 40);
    
    return 0;
}

//============================================
// データの読み込み
//============================================
int data_input()
{
    FILE *fp;
    char *filename = "randomdata.txt";
    char readline[LIMIT] = {'\0'};
    int i, j;
    
    if ((fp = fopen(filename, "r")) == NULL) {
        puts("ファイルのオープンに失敗しました");
        return -1;
    }
    
    i = 0;
    while ( fgets(readline, LIMIT, fp) != NULL ) {
        data[i++] = atoi(readline);
    }
    fclose(fp);
    return 1;
}

//============================================
// マップの表示
//============================================
void print_map()
{
    int i, j;
    
    for ( i = 0; i < AREA; i++  ) {
        for ( j = 0; j < AREA; j++ ) {
            if ( map[i][j] == 0 ) { 
                printf(" ･");
            } else {
                printf("%2d", map[i][j]);
            }
        }
        puts("");
    }    
}

//============================================
// 5セットの点(4つ)のシミュレート
//============================================
void five_simu(int num1, int num2)
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 4点を識別
    if      ( num1 == 1 && num2 == AREA-5 ) { direction = UP;   }
    else if ( num1 == 5 && num2 == AREA-5 ) { direction = DOWN; }
    else if ( num1 == 3 && num2 == AREA-7 ) { direction = LEFT; }
    else if ( num1 == 3 && num2 == AREA-3 ) { direction = RIGHT;}
    
    //--- 10ステップを5セット分シミュレート
    for ( k = 0; k < 5; k++ ) {
        row = num1;    // 行
        col = num2;    // 列
        for ( l = 0; l < STEP; l++ ) {
            //--- 移動処理
            move(&row, &col, &direction);
            c++;    // 次の乱数へ
            //--- 酔っ払いの動きを表示
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- デバッグ用
        //printf("i, j = %d, %d\n\n", row, col);
    }
}

//============================================
// 4セットの点(4つ)のシミュレート
//============================================
void four_simu(int num1, int num2)
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 4点を識別(1方向目用)
    if      ( num1 == 2 && num2 == AREA-6 ) { direction = UP;   }
    else if ( num1 == 4 && num2 == AREA-6 ) { direction = DOWN; }
    else if ( num1 == 2 && num2 == AREA-4 ) { direction = UP; }
    else if ( num1 == 4 && num2 == AREA-4 ) { direction = DOWN;}
    
    //--- 10ステップを4セット分シミュレート
    for ( k = 0; k < 4; k++ ) {
        if ( k == 2 ) {
            //--- 4点を識別(2方向目用)
            if      ( num1 == 2 && num2 == AREA-6 ) { direction = LEFT; }
            else if ( num1 == 4 && num2 == AREA-6 ) { direction = LEFT; }
            else if ( num1 == 2 && num2 == AREA-4 ) { direction = RIGHT;}
            else if ( num1 == 4 && num2 == AREA-4 ) { direction = RIGHT;}       
        }
        row = num1;    // 行
        col = num2;    // 列
        for ( l = 0; l < STEP; l++ ) {
            //--- 移動処理
            move(&row, &col, &direction);
            c++;
            //--- 酔っ払いの動きを表示
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- デバッグ用
        //printf("i, j = %d, %d\n", row, col);
    }
}

//============================================
// 4セットの点(出発地点)のシミュレート
//============================================
void start_simu()
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 10ステップを4セット分シミュレート
    for ( k = 0; k < 4; k++ ) {
        if      ( k == 0 ) { direction = UP;   }
        else if ( k == 1 ) { direction = DOWN; }
        else if ( k == 2 ) { direction = LEFT; }
        else if ( k == 3 ) { direction = RIGHT;}
        row = 3;         // 行
        col = AREA-5;    // 列
        for ( l = 0; l < STEP; l++ ) {
            //--- 移動処理
            move(&row, &col, &direction);
            c++;
            //--- 酔っ払いの動きを表示
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- デバッグ用
        //printf("i, j = %d, %d\n", row, col);
    }
}

//============================================
// 移動処理
//============================================
void move(int *row, int *col, int *direction)
{
    //--- 十字路のとき
    if ( *row > 0 && *col < AREA-1 ) {
        cross_road(row, col, direction);
    }
    //--- 曲がり角のとき
    else if ( *row == 0 && *col == AREA-1 ) {
        corner(row, col, direction);
    }            
    //--- 丁字路(突き当たり)のとき
    else if ( *row == 0 && *direction == UP || ( *col == AREA-1 && *direction == RIGHT ) ) {
        t_junction_end(row, col, direction);
    }
    //--- 丁字路(横から)のとき
    else if ( ( *row == 0 && ( *direction == LEFT || *direction == RIGHT ) ) || ( *col == AREA-1 && ( *direction == UP || *direction == DOWN ) ) ) {
        t_junction_side(row, col, direction);
    }    
}

//============================================
// 十字路の時の移動
//============================================
void cross_road(int *i, int *j, int *dir)
{
    //--- 上を向いているとき
    if ( *dir == UP ) {
        if      ( data[c] <= 4 )                 { *i -= 1;         *dir = UP;   }    // 直進(and 上を向く)
        else if ( data[c] == 5 || data[c] == 6 ) {         *j += 1; *dir = RIGHT;}    // 右折(and 右を向く)
        else if ( data[c] == 7 || data[c] == 8 ) {         *j -= 1; *dir = LEFT; }    // 左折(and 左を向く)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // 後退(and 下を向く)
    }
    //--- 下を向いているとき
    else if ( *dir == DOWN ) {
        if      ( data[c] <= 4 )                 { *i += 1;         *dir = DOWN; }    // 直進
        else if ( data[c] == 5 || data[c] == 6 ) {         *j -= 1; *dir = LEFT; }    // 右折
        else if ( data[c] == 7 || data[c] == 8 ) {         *j += 1; *dir = RIGHT;}    // 左折
        else if ( data[c] == 9 )                 { *i -= 1;         *dir = UP;   }    // 後退
    }
    //--- 左を向いているとき
    else if ( *dir == LEFT ) {
        if      ( data[c] <= 4 )                 {         *j -= 1; *dir = LEFT; }    // 直進
        else if ( data[c] == 5 || data[c] == 6 ) { *i -= 1;         *dir = UP;   }    // 右折
        else if ( data[c] == 7 || data[c] == 8 ) { *i += 1;         *dir = DOWN; }    // 左折
        else if ( data[c] == 9 )                 {         *j += 1; *dir = RIGHT;}    // 後退
    }
    //--- 右を向いているとき
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 4 )                 {         *j += 1; *dir = RIGHT;}    // 直進
        else if ( data[c] == 5 || data[c] == 6 ) { *i += 1;         *dir = DOWN; }    // 右折
        else if ( data[c] == 7 || data[c] == 8 ) { *i -= 1;         *dir = UP;   }    // 左折
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // 後退
    }
}

//============================================
// 曲がり角の時の移動
//============================================
void corner(int *i, int *j, int *dir)
{
    while ( data[c] <= 4 ) { c++; }    // 0-4のときはやり直し    
    //--- 上を向いているとき}
    if ( *dir == UP ) {
        if      ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // 曲がる(and 左を向く)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // 後退(and 下を向く)
    }
    //--- 右を向いているとき
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // 曲がる(and 下を向く)
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // 後退(and 左を向く)
    }
}

//============================================
// 丁字路(突き当り)の時の移動
//============================================
void t_junction_end(int *i, int *j, int *dir)
{
    while ( data[c] <= 4 ) { c++; }    // 0-4のときはやり直し
    //--- 上を向いているとき}
    if ( *dir == UP ) {
        if      ( data[c] == 5 || data[c] == 6 ) {         *j += 1; *dir = RIGHT;}    // 右折
        else if ( data[c] == 7 || data[c] == 8 ) {         *j -= 1; *dir = LEFT; }    // 左折
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // 後退
    }
    //--- 右を向いているとき
    else if ( *dir == RIGHT ) {
        if      ( data[c] == 5 || data[c] == 6 ) { *i += 1;         *dir = DOWN; }    // 右折
        else if ( data[c] == 7 || data[c] == 8 ) { *i -= 1;         *dir = UP;   }    // 左折
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // 後退
    }
}

//============================================
// 丁字路(横から)の時の移動
//============================================
void t_junction_side(int *i, int *j, int *dir)
{
    //--- 上を向いているとき
    if ( *dir == UP ) {
        if      ( data[c] <= 4 )                 { *i -= 1;         *dir = UP;   }    // 直進
        else if ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // 曲がる(and 左を向く)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // 後退(and 下を向く)
    }
    //--- 下を向いているとき
    else if ( *dir == DOWN ) {
        if      ( data[c] <= 4 )                 { *i += 1;         *dir = DOWN; }    // 直進
        else if ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // 曲がる(and 左を向く)
        else if ( data[c] == 9 )                 { *i -= 1;         *dir = UP;   }    // 後退
    }
    //--- 左を向いているとき
    else if ( *dir == LEFT ) {
        if      ( data[c] <= 4 )                 {         *j -= 1; *dir = LEFT; }    // 直進
        else if ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // 曲がる(and 下を向く)
        else if ( data[c] == 9 )                 {         *j += 1; *dir = RIGHT;}    // 後退
    }
    //--- 右を向いているとき
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 4 )                 {         *j += 1; *dir = RIGHT;}    // 直進
        else if ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // 曲がる(and 下を向く)
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // 後退
    }
}

//============================================
// 酔っ払いの動きを表示
//============================================
void show_drunkard(int i, int j)
{
    map[i][j]++;    
    print_map();
    Sleep(10);
    system("cls");     // コンソール画面をクリア
    map[i][j]--;
}

//============================================
// 総移動距離の計算
//============================================
void calc_move(int *total)
{
    int start[2] = {3, 15};    // スタート地点
    int diff_r, diff_c;        // スタート地点との差(行と列)
    int ct = 0;                // 全体としての差
    int i, j;
    
    for ( i = 0; i < AREA; i++ ) {
        for ( j = 0; j < AREA; j++ ) {
            if ( map[i][j] != 0 ) {    // たどり着いたことがない点は無視
                // 行成分の差
                diff_r = start[0] - i;
                ct += ( ( diff_r > 0 ) ? diff_r : -diff_r );
                // 列成分の差
                diff_c = start[1] - j;
                ct += ( ( diff_c > 0 ) ? diff_c : -diff_c );
                // (i, j)にたどり着いた回数 * 全体としての差 = 合計距離
                *total += map[i][j] * ct;
                ct = 0;
            }
        }
    }
}
