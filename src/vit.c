/*********************************************************************************************
*改编自http://en.allexperts.com/q/C-1587/text-editor-using-ncurses.htm 2008-11-04
 * ******************************************************************************************/
#include <curses.h>     //curses中包含stdio.h
#include <string.h>
#define CTRL(c) ((c) & 037)
#define FONTCOLOR COLOR_WHITE
#define NUMBERCOLOR COLOR_RED
#define SYSCOL 4
#define SYSROW 2
#define FONTCOLOR1 COLOR_BLACK
#define FONTCOLOR2 COLOR_RED
int row, col;   //全局光标坐标,初始值0,0
int file_row, file_col;   //文件行列值0,0
bool saved;
bool isWelcome;
char file_name[10];
//WINDOW *number_line;    //显示行号窗口
//WINDOW *status_bar;
//设置
void init_curses()  //初始化光标环境
{
    initscr();
    /*
    if(has_colors())
    {
        start_color();
        init_pair(1, FONTCOLOR, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
    }
    */
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    //curs_set(0);      //0表示不显示光标
    //refresh();
}
void statusbar(char *str)   //状态栏
{
    char message[COLS];
    //mvwaddstr(status_bar,LINES-SYSROW,0,str);
    //char position[6];
    //sprintf(position,"%2d,%-2d",row+1,col-SYSCOL+1); //中间对齐
    //mvwaddstr(status_bar,LINES-SYSROW, COLS-6, position);
    //mvaddstr(LINES-SYSROW, COLS-6, position);
    if(!strcmp(file_name, ""))
        mvprintw(LINES-SYSROW,0,"%s","[No Name]");
    else
        mvprintw(LINES-SYSROW,0,"%s",file_name);
    mvprintw(LINES-SYSROW,COLS-6,"%2d,%-3d",file_row,file_col);
    //usleep(10);
}
void set_command_line(char *str)//设置命令行
{
    //mvwaddstr(status_bar,1, 0, str);
    mvaddstr(LINES-1, 0, str);
    wrefresh(stdscr);
}
len(int lineno)
{
    int linelen = COLS - 1;

    while (linelen >= 0 && mvinch(lineno, linelen) == ' ')
        linelen--;
    return linelen + 1;
}
/*初始化*/
void init(int argc,char **argv)
{
    saved=1;
    FILE *fd;

    init_curses();  //初始光标
    setline(1,0);
    if (argc == 1)  //无参数
    {
        strcpy(file_name, "");
        statusbar(NULL);
        Show_Welcome();
    }
    else    //有参数,只读第一个，忽略其他
    {
        isWelcome=0;
        //file_name=argv[1];
        strcpy(file_name,argv[1]);
        read_file(file_name);
    } col=SYSCOL;
}
read_file(char *fn)    //读文件
{
    FILE *fd;
    fd = fopen(fn, "rb");
    if (fd == NULL)         //如果文件不存在
        return;
    file_row= 1;//文件行号
    int row= 0;//编辑器行号
    int length = SYSCOL;//计算每行长度
    int c;
    bool newline=0;
    while ((c = getc(fd)) != EOF)
    {
        if (row == LINES - SYSROW)   //末尾两行空出来
            break;
        if(newline)
        {
            setline(file_row,row); //设置行号
            newline=0;
        }
        if (c == '\n')      //如果遇到换行
        {
            row++;
            file_row++;
            length=SYSCOL;
            newline=1;
            continue;
        }
        if(!(length%COLS))  //如果超长
        {
            row++;
            length = SYSCOL;
            move(row,SYSCOL);
        }
        addch(c);
        length++;
    }
    fclose(fd);
    if(has_colors())
    {
        start_color();
        init_pair(1, FONTCOLOR, COLOR_BLACK);
        init_pair(2, FONTCOLOR1, COLOR_BLACK);
        init_pair(3,FONTCOLOR2,COLOR_BLACK);
        //attron(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
    }
    for(;row<LINES - SYSROW;row++)
        mvprintw(row,2,"~");
    if(has_colors())
    {
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);
        //standout();
    }
}
setline(int line,int editor_line)  //设置行号：在editor_line前三列显示line值
{
    /*
     * 前三列用来显示行号用
     * 循环从低位到高位分解line为一个一个字符，并从右到左显示出来
     * 显示用addch(char figure);
     * 左移一位用move(row,col--);
     *
     *2008年5月5日上午，田磊写了本函数的大致过程，主要是数字转化字符串的思路，但行号显示有问题。
     *
     * 2008年5月5日下午，我，肖山，肥坚一起探讨，终于把这个行号弄出来了！在这里感谢两位了。
     */
    //standend();
    if(has_colors())
    {
        start_color();
        init_pair(1, FONTCOLOR, COLOR_BLACK);
        init_pair(2, FONTCOLOR1, COLOR_BLACK);
        init_pair(3,FONTCOLOR2,COLOR_BLACK);
        //attron(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
    }
    //col=SYSCOL-2;   //从这里输出个位
    //move(editor_line,col);
    mvprintw(editor_line,0,"%3d",line);
    /*
    int div;//商
    int rest;//余数
    do
    {
        div=line/10;
        rest=line%10;
        //move(0,0);
        //waddch(number_line,rest+48);
        addch(rest+48);
        move(editor_line,--col);
        line=div;
    }
    while(div);
    */
    move(editor_line,SYSCOL);   //从这里输出文本数据
    if(has_colors())
    {
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);
        //standout();
    }
    //wrefresh(editor_line);
}
Show_Welcome(void)     //初始页
{
    int iLine = LINES/2 -6;     //LINES代表当前运行终端行数
    int iCol  = COLS/2;         //COLS代表当前运行终端列数
    mvaddstr(iLine, iCol-7,"VIT - Vi Tiny");
    mvaddstr(iLine+2, iCol-6,"Version 1.0");
    mvaddstr(iLine+3, iCol-6,"by D.C.T.X.");
    mvaddstr(iLine+4, iCol-22,"VIT is open source and freely distributable");
    mvaddstr(iLine+6, iCol-21,"Help the survivers of earthquake in Sichuan");
    mvaddstr(iLine+7, iCol-21,"type  :q<Enter>                     to exit");
    //idlok(stdscr, TRUE);
    //keypad(stdscr, TRUE);
    //refresh();
    //usleep(10);
    isWelcome=true;
}
/*编辑模式*/
edit()  //编辑模式
{
    int c;
    file_row=file_col=1;
    while(1)        //一直循环
    {
        statusbar(NULL);
        move(row, col);
        refresh();
        c = getch();
        /* Editor commands */
        switch(c)          //可用命令
        {
            /* hjkl and arrow keys: move cursor
             * in direction indicated */
            case 'h':
            case KEY_LEFT:
            case KEY_BACKSPACE:
                move_back();
                break;

            case 'j':
            case KEY_DOWN:
                move_down();
                break;

            case 'k':
            case KEY_UP:
                move_up();
                break;

            case 'l':
            case KEY_RIGHT:
                move_forward();
                break;
                /* i: enter insert mode */
            case KEY_IC:
            case 'i':
                insert();
                break;
                /* a: append insert mode */
            case 'a':
                if (col < COLS - 1)
                    col++;
                insert();
                break;

                /* x: delete current character */
            case KEY_DC:
            case 'x':
                delete_character();
                break;

                /* o: open up a new line and enter insert mode */
            case KEY_IL:
            case 'o':
                //move_down();
                //move(--row, col = SYSCOL);   //前四列显示行号用
                insertln();
                file_row++;
                row++;
                setline(file_row,row);
                insert();
                break;

                /* d: delete current line */
            case KEY_DL:
            case 'd':
                delete_line();

                break;

                /* ^L: redraw screen */
            case KEY_CLEAR:
            case CTRL('L'):
                wrefresh(curscr);
                break;
            case ':':           //进入命令模式
                command(':');
                break;
            case '/':
                command('/');       //进入检索模式
                break;
            case '?':
                command('?');       //进入逆向检索模式
                break;
            case 'n':
                find_next();
                break;
            default:
                statusbar("unextended operator");
                break;
        }
    }
}
delete_character()
{
    delch();
    int i;
    for(i=1;mvinch(row+i,SYSCOL-2) == ' ';i++)
    {
        chtype c = mvinch(row+i,SYSCOL);
        mvaddch(row+i-1,COLS-1,c);
        mvdelch(row+i,SYSCOL);
    }
}
delete_line()
{
    if(has_colors())
    {
        start_color();
        init_pair(1, FONTCOLOR, COLOR_BLACK);
        init_pair(2, FONTCOLOR1, COLOR_BLACK);
        init_pair(3,FONTCOLOR2,COLOR_BLACK);
        //attron(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
    }
    deleteln();//删除当前行
    int i;
    /*防止状态栏上移*/
    move(LINES-SYSROW-1,0);
    clrtoeol();
    mvprintw(LINES-SYSROW-1,2,"~");

    int a;
    //chtype finger;
    int finger;
    bool first=1;
    for (a=row;a<LINES-SYSROW;a++)
    {
        for(i=SYSCOL-2;i>=0;i--)
        {
            finger=mvinch(a,i) & A_CHARTEXT;
            if(finger == '~')//到行尾了
                break;
            if(finger == ' ')
            {
                if(first)
                {
                    deleteln();
                    a--;
                }
                break;
            }
            else
            {
                first=0;
                if(finger==48) //如果是零
                {
                    addch(57);//输出9
                }
                else
                {
                    addch(finger-1);
                    break;
                }
            }
        }
        if((mvinch(row,SYSCOL-2) & A_CHARTEXT)== '~')//到行尾了
        {
            move_up();
            break;
        }
    }
    if(has_colors())
    {
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);
        //standout();
    }
}
move_back()
{
    if (col > SYSCOL)
    {
        col--;
        file_col--;
    }
    else if(mvinch(row, SYSCOL-2) == ' ')
    {
        row--;
        col=COLS-1;
        file_col--;
    }
}
move_forward()
{
    if (col < COLS - 1)
    {
        col++;
        file_col++;
    }
    else if(mvinch(row+1, SYSCOL-2) == ' ')
    {
        row++;
        col=SYSCOL;
        file_col++;
    }
}
move_down()
{
    int i=1;
    while((mvinch(row+i, SYSCOL-2) & A_CHARTEXT) == ' ')
        i++;
    if((mvinch(row+i, SYSCOL-2) & A_CHARTEXT) == '~'){}
    else if(row+i < LINES - 2)
    {
        row=row+i;
        file_row++;
        file_col=col-3; //让每行都在控制范围内
    }
}
move_up()
{
    if (row > 0)
    {
        int i;
        for(i=1;mvinch(row-i, SYSCOL-2) == ' ';i++){}
        row=row-i;
        file_row--;
        file_col = col-3;
    }
    else
    {
        //setstatusbar();
    }
}
/*命令模式*/
command(int type)           //
{
    move(LINES-1,0);       
    clrtoeol();
    addch(type);
    int posi=1;

    int c;
    int i=0;
    for(;;)
    {
        c=getch();
        if (c == 27) //ESC键返回编辑模式edit
        {
            //move(LINES - 1, 0);
            clrtoeol();
            break;
        }
        if (c == 13) //回车键执行命令
        {
            char command_str[COLS];
            int n = len(LINES-1);
            int i;
            for(i=0;i<n;i++)
                command_str[i] = mvinch(LINES-1,i+1) & A_CHARTEXT;
            command_str[n]='\0';    //很重要！否则会有乱码

            switch(type)
            {
                case ':':
                    execute(command_str);
                    break;
                case '/':
                    search(command_str);
                    break;
                case '?':
                    reverse_search(command_str);
                    break;
                default:
                    break;
            }
            break;
        }
        else if(c == KEY_BACKSPACE)
        {
            if(posi>1)//防越界
            {
                posi--;
                move(LINES-1,posi);
                delch();
            }
            else
                break;
        }
        else
        {
            if(posi<COLS-1)
            {
                addch(c);
                posi++;
            }
        }
    }
}
execute(char *command_str)
{
    char *pch = strtok(command_str," ");
    if(pch != NULL)
    {
        if(!strcmp(pch,"q"))    //退出
            quit();
        else if(!strcmp(pch,"w"))
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
                strcpy(file_name, pch);
            write_file(file_name);
        }
        else if(!strcmp(pch,"wq"))  //保存并退出
        {
            pch = strtok (NULL, " ");
            if(pch==NULL)
                write_file(file_name);
            else
                write_file(pch);
            quit();
        }
        else if(!strcmp(pch,"q!"))  //强制退出
        {
            extern void perror(), exit();
            endwin();
            exit(2);
        }
        else if(!strcmp(pch,"r"))
        {
            pch = strtok(NULL," ");
            if(pch!=NULL)
            {
                move(row,col);
                read_file(pch);
            }
            else
                statusbar("please input a file name");
        }
    }
}
write_file(char *file_nm)
{
    if(file_nm==NULL)
    {
        mvprintw(LINES-1,0,"%s","E32: No File Name");
    }
    else
    {
        int i, n, l;
        FILE *food;
        food = fopen(file_nm, "w");
        for (l = 0; l < LINES - SYSCOL; l++)
        {
            n = len(l);
            for (i = SYSCOL; i < n; i++)
                putc(mvinch(l, i) & A_CHARTEXT, food);
            if((mvinch(l+1,2) & A_CHARTEXT) != ' ')
                putc('\n', food);
        }
        fclose(food);
        saved=1;
        mvprintw(LINES-SYSROW,20,"%s","   ");
    }
}
quit()
{
    extern void perror(), exit();
    if(saved)
    {
        endwin();
        exit(2);
    }
    else
    {
        mvprintw(LINES-1,0,"%s","No Saved!Please save and quit(wq) or Not save and quit!(q!)");
        //提示：未保存，请先保存再退出(wq)或不保存退出(q!)
    }
}
/*插入模式*/
insert() //插入模式
{
    int c;
    //standout();
    statusbar(NULL);
    set_command_line("-- INSERT --");
    //standend();
    move(row, col);
    refresh();
    for (;;)
    {
        c = getch();
        if (c == 27) //ESC键返回编辑模式edit
        {
            if(col>SYSCOL)//防越界
                col--;
            set_command_line("            ");
            break;
        }
        else
        {
            saved=0;
            if(c == KEY_BACKSPACE)
            {
                if(col>SYSCOL)//防越界
                {
                    col--;
                    move(row,col);
                    delch();
                }
            }
            else
            {
                if(isWelcome)   //清除欢迎画面
                {
                    clear();
                    statusbar(NULL);
                    set_command_line("-- INSERT --");
                    setline(1,0);
                    col=4;
                    move(row,col);
                    isWelcome=0;
                }
                insch(c);
                //statusbar(NULL);
                mvprintw(LINES-SYSROW,20,"[+]");
                move(row, ++col);
            }
        }
    }
    move(LINES - 1, COLS - 20);
    clrtoeol();
    move(row, col);
    refresh();
}
/*检索模式*/
search(char *exgre)
{
}
reverse_search(char *exgre)
{
}
find_next(char *sub_str)    //
{
}
/*主函数*/
int main(int argc,char **argv)      //主函数
{
    char file_name[10];
    //strcpy(file_name,argv[1]);
    init(argc, argv);       //初始化
    edit();                 //进入编辑模式
    return 0;
}
