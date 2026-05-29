#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NAME 30
#define MAX_PWD 20
#define CATEGORY_NUM 5
#define Vip_LEVEL 4

typedef enum {
    COMMON = 0,
    SILVER = 1,
    GOLD = 2,
    DIAMOND = 3
} VipLevel;

typedef enum {
    STUDY = 0,
    FOOD = 1,
    DAILY = 2,
    DRINK = 3,
    OTHER = 4
} Category;

typedef struct Goods {
    int id;
    char name[MAX_NAME];
    Category cate;
    float price;
    int stock;
    int total_sell;
    int expire;
    float discount;
    int discount_deadline;
    struct Goods* next;
} Goods;

typedef struct Member {
    int id;
    char name[MAX_NAME];
    char pwd[MAX_PWD];
    VipLevel level;
    float total_cost;
    int point;
    float balance;
    struct Member* next;
} Member;

typedef struct Record {
    int member_id;
    int goods_id;
    int num;
    float cost;
    char time[30];
    struct Record* next;
} Record;

Goods* goods_head = NULL;
Member* member_head = NULL;
Record* record_head = NULL;

int goods_count = 0;
int member_count = 0;
int record_count = 0;

float today_income = 0.0f;

const char* cate_name[CATEGORY_NUM] = {
    "Study","Food","Daily","Drink","Other"
};

const char* vip_name[Vip_LEVEL] = {
    "Common","Silver","Gold","Diamond"
};

const float vip_discount[Vip_LEVEL] = {
    1.0f,0.95f,0.9f,0.85f
};

void getCurrentTime(char* str) {
    time_t t = time(NULL);
    struct tm* tt = localtime(&t);

    sprintf(str,
        "%04d-%02d-%02d %02d:%02d",
        tt->tm_year + 1900,
        tt->tm_mon + 1,
        tt->tm_mday,
        tt->tm_hour,
        tt->tm_min);
}

void updateVipLevel(Member* m) {
    if (m->total_cost >= 5000) m->level = DIAMOND;
    else if (m->total_cost >= 2000) m->level = GOLD;
    else if (m->total_cost >= 500) m->level = SILVER;
    else m->level = COMMON;
}

void showAllGoods() {
    Goods* p = goods_head;

    printf("====================================\n");
    printf("ID   Name      Cate    Price Stock\n");
    printf("====================================\n");

    while (p != NULL) {
        printf("%-4d %-9s %-8s %-5.2f %d\n",
            p->id,
            p->name,
            cate_name[p->cate],
            p->price * p->discount,
            p->stock);
        p = p->next;
    }
}

void searchGoods() {
    char key[30];
    Goods* g = goods_head;

    system("cls");
    printf("Input Goods Name:");
    scanf("%s", key);

    while (g != NULL) {
        if (strstr(g->name, key) != NULL) {
            printf("ID:%d Name:%s Price:%.2f Stock:%d\n",
                g->id, g->name, g->price, g->stock);
        }
        g = g->next;
    }

    system("pause");
}

void lowStockWarning() {
    Goods* g = goods_head;

    system("cls");
    printf("======= LOW STOCK =======\n");

    while (g != NULL) {
        if (g->stock < 20) {
            printf("ID:%d Name:%s Stock:%d\n",
                g->id, g->name, g->stock);
        }
        g = g->next;
    }

    system("pause");
}

Member* memberLogin() {
    char name[MAX_NAME];
    char pwd[MAX_PWD];

    system("cls");
    printf("Username:");
    scanf("%s", name);

    printf("Password:");
    scanf("%s", pwd);

    Member* m = member_head;

    while (m != NULL) {
        if (!strcmp(m->name, name) && !strcmp(m->pwd, pwd)) {
            printf("Login Success\n");
            system("pause");
            return m;
        }
        m = m->next;
    }

    printf("Login Failed\n");
    system("pause");
    return NULL;
}

void memberRegister() {
    Member* m = (Member*)malloc(sizeof(Member));

    system("cls");

    m->id = ++member_count;

    printf("Username:");
    scanf("%s", m->name);

    printf("Password:");
    scanf("%s", m->pwd);

    m->level = COMMON;
    m->total_cost = 0;
    m->point = 0;
    m->balance = 0;

    m->next = member_head;
    member_head = m;

    printf("Register Success\n");
    system("pause");
}

void recharge(Member* m) {
    float money;

    system("cls");
    printf("Input Money:");
    scanf("%f", &money);

    m->balance += money;

    printf("Success Balance:%.2f\n", m->balance);
    system("pause");
}


void giftRecommend(Member* m) {
    system("cls");

    printf("GIFT RECOMMEND\n");

    if (m->total_cost >= 3000)
        printf("Coffee Cup / Umbrella / Notebook\n");
    else if (m->total_cost >= 1000)
        printf("Notebook / Pen\n");
    else
        printf("Candy\n");

    system("pause");
}

void shopping(Member* m) {
    int id, num;
    float total = 0;

    system("cls");
    showAllGoods();

    printf("Goods ID:");
    scanf("%d", &id);

    printf("Number:");
    scanf("%d", &num);

    Goods* g = goods_head;

    while (g && g->id != id)
        g = g->next;

    if (!g || num > g->stock) return;

    total = g->price * g->discount * num;

    if (m->balance < total) return;

    m->balance -= total;
    m->total_cost += total;
    m->point += (int)total;

    g->stock -= num;
    g->total_sell += num;

    today_income += total;

    updateVipLevel(m);

    printf("Success Total:%.2f\n", total);
    system("pause");
}

bool adminLogin() {
    char pwd[MAX_PWD];

    system("cls");
    printf("Password:");
    scanf("%s", pwd);

    if (!strcmp(pwd, "admin123")) {
        printf("Login Success\n");
        system("pause");
        return true;
    }

    printf("Login Failed\n");
    system("pause");
    return false;
}

void statSales() {
    Goods* p = goods_head;

    system("cls");
    printf("SALES\n");

    while (p) {
        printf("ID:%d Name:%s Sell:%d Stock:%d\n",
            p->id, p->name, p->total_sell, p->stock);
        p = p->next;
    }

    system("pause");
}

void restockGoods() {
    int id, num;

    system("cls");
    printf("ID:");
    scanf("%d", &id);

    Goods* g = goods_head;
    while (g && g->id != id)
        g = g->next;

    if (!g) return;

    printf("Num:");
    scanf("%d", &num);

    g->stock += num;

    system("pause");
}

void adminMenu() {
    if (!adminLogin()) return;

    int c;

    while (1) {
        system("cls");

        printf("ADMIN MENU\n");
        printf("1.Clear Expire\n");
        printf("2.Show Goods\n");
        printf("3.Stat Sales\n");
        printf("4.Today Income\n");
        printf("5.Member List\n");
        printf("6.Low Stock Warning\n");
        printf("7.Set Discount\n");
        printf("8.Restock\n");
        printf("0.Exit\n");

        printf("Select:");
        scanf("%d", &c);

        if (c == 0) break;
        if (c == 2) showAllGoods();
        if (c == 3) statSales();
        if (c == 4) { printf("%.2f\n", today_income); system("pause"); }
        if (c == 6) lowStockWarning();
        if (c == 8) restockGoods();
    }
}

void customerMenu() {
    int c;

    system("cls");
    printf("1.Login\n2.Register\nSelect:");
    scanf("%d", &c);

    Member* m = NULL;

    if (c == 1) m = memberLogin();
    if (c == 2) { memberRegister(); return; }

    if (!m) return;

    while (1) {
        system("cls");

        printf("USER MENU\n");
        printf("1.Shop\n2.Info\n3.Recharge\n4.Search\n0.Exit\n");

        printf("Select:");
        scanf("%d", &c);

        if (c == 0) break;
        if (c == 1) shopping(m);
        if (c == 2) {
            printf("%s %.2f %d\n",
                m->name, m->balance, m->point);
            system("pause");
        }
        if (c == 3) recharge(m);
        if (c == 4) searchGoods();
    }
}

int main() {
    int c;

    while (1) {
        system("cls");

        printf("SYSTEM\n1.Admin\n2.User\n0.Exit\n");
        scanf("%d", &c);

        if (c == 0) break;
        if (c == 1) adminMenu();
        if (c == 2) customerMenu();
    }

    return 0;
}
