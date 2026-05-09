#ifndef  __MENU_H__
#define  __MENU_H__


// 定义菜单标题的最大长度为50个字符
#define MENU_TITLE_MAX_SIZE 50

// 定义菜单标签的最大长度为50个字符
#define MENU_LABEL_MAX_SIZE 50

// 定义菜单级别的数量为10
#define MENU_LEVEL_NUM      10

// 定义测试菜单1的主菜单项数量为8
#define MENU1_MAIN_NUM 8

// 定义测试菜单2的文件菜单项数量为5
#define TEST_MENU2_FILE_NUM 1

//60s无按键按下超时时间，50次等于1s  60*50=3000，60s时间
#define MENU_MAXTIME  3000   

typedef enum {
    MENU_TYPE_FUN,
    MENU_TYPE_LIST
} MenuType;

struct _sMenuList
{
    uint16_t num; // 当前菜单功能项总数
    char title[MENU_TITLE_MAX_SIZE]; // 标题名称
    char label[MENU_LABEL_MAX_SIZE]; // 项目名称
    void (*upfun)(bool);   //按键上功能项函数
    void (*downfun)(bool); //按键下功能项函数
	void (*longaddfun)(bool); //按键长按加功能项函数
	void (*longsubfun)(bool); //按键长按加功能项函数
    struct _sMenuList *next; // 下一级菜单
    struct _sMenuList *prev; // 上一级菜单
};

struct _Menu{
    struct _sMenuList *current; // 当前菜单
    uint16_t index[MENU_LEVEL_NUM]; // 各级索引，最大10级
    uint16_t level; // 当前级数
};

extern struct _Menu TestMenu;//管理菜单的结构体


extern struct _sMenuList Menu1_Main[];//主菜单



extern uint8_t MenuFlag;//菜单标志位
extern uint8_t Menu_KEY0_OK;//确定键
extern uint8_t Menu_KEY1_UP;//向上按键
extern uint8_t Menu_KEY2_DOWN;//向下按键
extern uint8_t Menu_KEY3_RETURN;//返回键
extern uint8_t Key_Long_Flag;//上下键长按标志位  1：上   2：下
extern uint32_t Menu_timeout;//按键最长无按下时间

void ExecuteMenuTask(void) ;//菜单任务
void Menu_Init(void);//菜单初始化
void HandleLongPressAdd(int* time);//长按增加功能
void HandleLongPressSub(int* time);//长按减少功能
void timerMonitorMenu(void);//检测按键在菜单里长时间无操作，返回主页面
void setMenuTimeout(uint8_t *Flag, uint32_t *Menu_timeout) ;// 设置菜单超时。此函数用于初始化菜单超时状态，当菜单处于活动状态时,将超时标志设置为0，并将超时计数器设置为最大值。
#endif



