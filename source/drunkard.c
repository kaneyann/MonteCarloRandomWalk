#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//--- �萔�}�N��
#define STEP  10     // �V�~�����[�g����X�e�b�v��
#define AREA  20     // �}�b�v�̍L��
#define NUM   500    // �����̌�
#define LIMIT 10     // 1�s������̓ǂݍ��ݕ������
enum Direction { UP, DOWN, RIGHT, LEFT};

//--- �O���[�o���ϐ�
int map[AREA][AREA] = {0};    // �}�b�v
int data[NUM] = {0};          // ���̓f�[�^�i�[�z��
int c = 0;                    // �g�p���闐���̃J�E���^

//--- �֐��v���g�^�C�v�錾
int data_input();                      // �f�[�^�̓ǂݍ���
void print_map();                      // �}�b�v�̕\��
void five_simu(int num1, int num2);    // 5�Z�b�g�̓_(4��)�̃V�~�����[�g
void four_simu(int num1, int num2);    // 4�Z�b�g�̓_(4��)�̃V�~�����[�g
void start_simu();                     // 4�Z�b�g�̓_(�o���n�_)�̃V�~�����[�g
void move(int *row, int *col, int *direction);     // �ړ�����
void cross_road(int *i, int *j, int *dir);         // �\���H�̎��̈ړ�
void corner(int *i, int *j, int *dir);             // �Ȃ���p�̎��̈ړ�
void t_junction_end(int *i, int *j, int *dir);     // �����H(�˂�����)�̎��̈ړ�
void t_junction_side(int *i, int *j, int *dir);    // �����H(������)�̎��̈ړ�
void show_drunkard(int i, int j);      // ���������̓�����\��
void calc_move(int *total);            // ���ړ������̌v�Z

//============================================
// main�֐�
//============================================
int main(void)
{
    //###--- �f�o�b�O�p�ϐ�
    int sum_set = 0;
    int total_move = 0;    // ���ړ�����
    int i, j;
    
    srand((unsigned)time(NULL));
    
    //--- �f�[�^�̐������@1(�e�L�X�g���痐����ǂݍ���)
    //if ( data_input() == -1 ) { return 0; }
    
    //--- �f�[�^�̐������@2(�����ŗ����𔭐�������)
    for ( i = 0; i < NUM; i++ ) {
        data[i] = rand() % 10;
    }
    
    //--- 5�Z�b�g�̓_(4��)�ɂ��ăV�~�����[�g
    five_simu(1, AREA-5);
    five_simu(5, AREA-5);
    five_simu(3, AREA-7);
    five_simu(3, AREA-3);
    
    //--- 4�Z�b�g�̓_(4��)�ɂ��ăV�~�����[�g
    four_simu(2, AREA-6);
    four_simu(4, AREA-6);
    four_simu(2, AREA-4);
    four_simu(4, AREA-4);

    //--- 4�Z�b�g�̓_(�o���n�_)�ɂ��ăV�~�����[�g
    start_simu();
    
    // ���ړ������̌v�Z
    calc_move(&total_move);
    
    //--- �}�b�v�̕\��
    print_map();
    
    // ���v�Z�b�g���̌v�Z
    for ( i = 0; i < AREA; i++ ) {
        for ( j = 0; j < AREA; j++ ) {
            sum_set += map[i][j];
        }
    }
    printf("���v�Z�b�g��       = %d\n", sum_set);
    printf("�g�p���������̌� = %d\n", c);
    printf("���ψړ�����       = %d\n", total_move / 40);
    
    return 0;
}

//============================================
// �f�[�^�̓ǂݍ���
//============================================
int data_input()
{
    FILE *fp;
    char *filename = "randomdata.txt";
    char readline[LIMIT] = {'\0'};
    int i, j;
    
    if ((fp = fopen(filename, "r")) == NULL) {
        puts("�t�@�C���̃I�[�v���Ɏ��s���܂���");
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
// �}�b�v�̕\��
//============================================
void print_map()
{
    int i, j;
    
    for ( i = 0; i < AREA; i++  ) {
        for ( j = 0; j < AREA; j++ ) {
            if ( map[i][j] == 0 ) { 
                printf(" �");
            } else {
                printf("%2d", map[i][j]);
            }
        }
        puts("");
    }    
}

//============================================
// 5�Z�b�g�̓_(4��)�̃V�~�����[�g
//============================================
void five_simu(int num1, int num2)
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 4�_������
    if      ( num1 == 1 && num2 == AREA-5 ) { direction = UP;   }
    else if ( num1 == 5 && num2 == AREA-5 ) { direction = DOWN; }
    else if ( num1 == 3 && num2 == AREA-7 ) { direction = LEFT; }
    else if ( num1 == 3 && num2 == AREA-3 ) { direction = RIGHT;}
    
    //--- 10�X�e�b�v��5�Z�b�g���V�~�����[�g
    for ( k = 0; k < 5; k++ ) {
        row = num1;    // �s
        col = num2;    // ��
        for ( l = 0; l < STEP; l++ ) {
            //--- �ړ�����
            move(&row, &col, &direction);
            c++;    // ���̗�����
            //--- ���������̓�����\��
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- �f�o�b�O�p
        //printf("i, j = %d, %d\n\n", row, col);
    }
}

//============================================
// 4�Z�b�g�̓_(4��)�̃V�~�����[�g
//============================================
void four_simu(int num1, int num2)
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 4�_������(1�����ڗp)
    if      ( num1 == 2 && num2 == AREA-6 ) { direction = UP;   }
    else if ( num1 == 4 && num2 == AREA-6 ) { direction = DOWN; }
    else if ( num1 == 2 && num2 == AREA-4 ) { direction = UP; }
    else if ( num1 == 4 && num2 == AREA-4 ) { direction = DOWN;}
    
    //--- 10�X�e�b�v��4�Z�b�g���V�~�����[�g
    for ( k = 0; k < 4; k++ ) {
        if ( k == 2 ) {
            //--- 4�_������(2�����ڗp)
            if      ( num1 == 2 && num2 == AREA-6 ) { direction = LEFT; }
            else if ( num1 == 4 && num2 == AREA-6 ) { direction = LEFT; }
            else if ( num1 == 2 && num2 == AREA-4 ) { direction = RIGHT;}
            else if ( num1 == 4 && num2 == AREA-4 ) { direction = RIGHT;}       
        }
        row = num1;    // �s
        col = num2;    // ��
        for ( l = 0; l < STEP; l++ ) {
            //--- �ړ�����
            move(&row, &col, &direction);
            c++;
            //--- ���������̓�����\��
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- �f�o�b�O�p
        //printf("i, j = %d, %d\n", row, col);
    }
}

//============================================
// 4�Z�b�g�̓_(�o���n�_)�̃V�~�����[�g
//============================================
void start_simu()
{
    int direction;
    int row, col;
    int k, l;
    
    //--- 10�X�e�b�v��4�Z�b�g���V�~�����[�g
    for ( k = 0; k < 4; k++ ) {
        if      ( k == 0 ) { direction = UP;   }
        else if ( k == 1 ) { direction = DOWN; }
        else if ( k == 2 ) { direction = LEFT; }
        else if ( k == 3 ) { direction = RIGHT;}
        row = 3;         // �s
        col = AREA-5;    // ��
        for ( l = 0; l < STEP; l++ ) {
            //--- �ړ�����
            move(&row, &col, &direction);
            c++;
            //--- ���������̓�����\��
            show_drunkard(row, col);
        }
        map[row][col]++;
        //###--- �f�o�b�O�p
        //printf("i, j = %d, %d\n", row, col);
    }
}

//============================================
// �ړ�����
//============================================
void move(int *row, int *col, int *direction)
{
    //--- �\���H�̂Ƃ�
    if ( *row > 0 && *col < AREA-1 ) {
        cross_road(row, col, direction);
    }
    //--- �Ȃ���p�̂Ƃ�
    else if ( *row == 0 && *col == AREA-1 ) {
        corner(row, col, direction);
    }            
    //--- �����H(�˂�������)�̂Ƃ�
    else if ( *row == 0 && *direction == UP || ( *col == AREA-1 && *direction == RIGHT ) ) {
        t_junction_end(row, col, direction);
    }
    //--- �����H(������)�̂Ƃ�
    else if ( ( *row == 0 && ( *direction == LEFT || *direction == RIGHT ) ) || ( *col == AREA-1 && ( *direction == UP || *direction == DOWN ) ) ) {
        t_junction_side(row, col, direction);
    }    
}

//============================================
// �\���H�̎��̈ړ�
//============================================
void cross_road(int *i, int *j, int *dir)
{
    //--- ��������Ă���Ƃ�
    if ( *dir == UP ) {
        if      ( data[c] <= 4 )                 { *i -= 1;         *dir = UP;   }    // ���i(and �������)
        else if ( data[c] == 5 || data[c] == 6 ) {         *j += 1; *dir = RIGHT;}    // �E��(and �E������)
        else if ( data[c] == 7 || data[c] == 8 ) {         *j -= 1; *dir = LEFT; }    // ����(and ��������)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // ���(and ��������)
    }
    //--- ���������Ă���Ƃ�
    else if ( *dir == DOWN ) {
        if      ( data[c] <= 4 )                 { *i += 1;         *dir = DOWN; }    // ���i
        else if ( data[c] == 5 || data[c] == 6 ) {         *j -= 1; *dir = LEFT; }    // �E��
        else if ( data[c] == 7 || data[c] == 8 ) {         *j += 1; *dir = RIGHT;}    // ����
        else if ( data[c] == 9 )                 { *i -= 1;         *dir = UP;   }    // ���
    }
    //--- ���������Ă���Ƃ�
    else if ( *dir == LEFT ) {
        if      ( data[c] <= 4 )                 {         *j -= 1; *dir = LEFT; }    // ���i
        else if ( data[c] == 5 || data[c] == 6 ) { *i -= 1;         *dir = UP;   }    // �E��
        else if ( data[c] == 7 || data[c] == 8 ) { *i += 1;         *dir = DOWN; }    // ����
        else if ( data[c] == 9 )                 {         *j += 1; *dir = RIGHT;}    // ���
    }
    //--- �E�������Ă���Ƃ�
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 4 )                 {         *j += 1; *dir = RIGHT;}    // ���i
        else if ( data[c] == 5 || data[c] == 6 ) { *i += 1;         *dir = DOWN; }    // �E��
        else if ( data[c] == 7 || data[c] == 8 ) { *i -= 1;         *dir = UP;   }    // ����
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // ���
    }
}

//============================================
// �Ȃ���p�̎��̈ړ�
//============================================
void corner(int *i, int *j, int *dir)
{
    while ( data[c] <= 4 ) { c++; }    // 0-4�̂Ƃ��͂�蒼��    
    //--- ��������Ă���Ƃ�}
    if ( *dir == UP ) {
        if      ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // ���(and ��������)
    }
    //--- �E�������Ă���Ƃ�
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // ���(and ��������)
    }
}

//============================================
// �����H(�˂�����)�̎��̈ړ�
//============================================
void t_junction_end(int *i, int *j, int *dir)
{
    while ( data[c] <= 4 ) { c++; }    // 0-4�̂Ƃ��͂�蒼��
    //--- ��������Ă���Ƃ�}
    if ( *dir == UP ) {
        if      ( data[c] == 5 || data[c] == 6 ) {         *j += 1; *dir = RIGHT;}    // �E��
        else if ( data[c] == 7 || data[c] == 8 ) {         *j -= 1; *dir = LEFT; }    // ����
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // ���
    }
    //--- �E�������Ă���Ƃ�
    else if ( *dir == RIGHT ) {
        if      ( data[c] == 5 || data[c] == 6 ) { *i += 1;         *dir = DOWN; }    // �E��
        else if ( data[c] == 7 || data[c] == 8 ) { *i -= 1;         *dir = UP;   }    // ����
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // ���
    }
}

//============================================
// �����H(������)�̎��̈ړ�
//============================================
void t_junction_side(int *i, int *j, int *dir)
{
    //--- ��������Ă���Ƃ�
    if ( *dir == UP ) {
        if      ( data[c] <= 4 )                 { *i -= 1;         *dir = UP;   }    // ���i
        else if ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 { *i += 1;         *dir = DOWN; }    // ���(and ��������)
    }
    //--- ���������Ă���Ƃ�
    else if ( *dir == DOWN ) {
        if      ( data[c] <= 4 )                 { *i += 1;         *dir = DOWN; }    // ���i
        else if ( data[c] <= 8 )                 {         *j -= 1; *dir = LEFT; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 { *i -= 1;         *dir = UP;   }    // ���
    }
    //--- ���������Ă���Ƃ�
    else if ( *dir == LEFT ) {
        if      ( data[c] <= 4 )                 {         *j -= 1; *dir = LEFT; }    // ���i
        else if ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 {         *j += 1; *dir = RIGHT;}    // ���
    }
    //--- �E�������Ă���Ƃ�
    else if ( *dir == RIGHT ) {
        if      ( data[c] <= 4 )                 {         *j += 1; *dir = RIGHT;}    // ���i
        else if ( data[c] <= 8 )                 { *i += 1;         *dir = DOWN; }    // �Ȃ���(and ��������)
        else if ( data[c] == 9 )                 {         *j -= 1; *dir = LEFT; }    // ���
    }
}

//============================================
// ���������̓�����\��
//============================================
void show_drunkard(int i, int j)
{
    map[i][j]++;    
    print_map();
    Sleep(10);
    system("cls");     // �R���\�[����ʂ��N���A
    map[i][j]--;
}

//============================================
// ���ړ������̌v�Z
//============================================
void calc_move(int *total)
{
    int start[2] = {3, 15};    // �X�^�[�g�n�_
    int diff_r, diff_c;        // �X�^�[�g�n�_�Ƃ̍�(�s�Ɨ�)
    int ct = 0;                // �S�̂Ƃ��Ă̍�
    int i, j;
    
    for ( i = 0; i < AREA; i++ ) {
        for ( j = 0; j < AREA; j++ ) {
            if ( map[i][j] != 0 ) {    // ���ǂ蒅�������Ƃ��Ȃ��_�͖���
                // �s�����̍�
                diff_r = start[0] - i;
                ct += ( ( diff_r > 0 ) ? diff_r : -diff_r );
                // �񐬕��̍�
                diff_c = start[1] - j;
                ct += ( ( diff_c > 0 ) ? diff_c : -diff_c );
                // (i, j)�ɂ��ǂ蒅������ * �S�̂Ƃ��Ă̍� = ���v����
                *total += map[i][j] * ct;
                ct = 0;
            }
        }
    }
}
