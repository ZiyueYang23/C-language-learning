#define _CRT_SECURE_NO_WARNINGS 1
#include "contact.h"

void menu(void)
{
    printf("**********************************\n");
    printf("*******   1.Add   2.Del    *******\n");
    printf("*******   3.Fin   4.Edi    *******\n");
    printf("*******   5.Sort  6.Show   *******\n");
    printf("*******   0.exit  9.back   *******\n");
    printf("**********************************\n");
}
// 动态版本
int initContact(contacts *ps) // 改成int需要想想
{
    assert(ps != NULL);


    ps->counter = 0;
    ps->data = (pInformation *)calloc(STA_NUM, sizeof(pInformation));
    // 这是指针承载创建内存的首地址
    // 由于data是指向结构体的指针，所以强转成pInformation*型，让系统知道内存咋划分成块。
    if (ps->data == NULL) // 要保证不是空指针否则不能用
    {
        printf("pInformation::%s", strerror(errno));
        return 1;
    }
    ps->capacity = STA_NUM; // 这个是精髓，创造一个容量，当容量和数量相等的时候就扩容

    LoadContact(ps);
    return 0;
}

void LoadContact(contacts *ps)
{
    FILE *pfRead = fopen("contact.txt", "rb");
    if (pfRead == NULL)
    {
        perror("LoadContact");
        return;
    }
    pInformation tmp = {0};

    while(fread(&tmp,sizeof(pInformation),1,pfRead)==1)
    {
        CheckAddCapacity(ps);

        ps->data[ps->counter] = tmp;
        ps->counter++;
    }
}

//自己尝试做的版本到但是有点小问题
// void LoadContact(contacts *ps)
// {
//     int ret = 0;
//     FILE *pfRead = fopen("contact.txt", "r");
//     if(pfRead==NULL)
//     {
//         perror("LoadContact");
//         return;
//     }
//     pInformation tmp = {0};
//     do
//     {
//         int i = 0;
//         int ret =fscanf(pfRead, "%s %d %s %s %s", tmp.name, &(tmp.age), tmp.sex, tmp.tele, tmp.addr);
//         if(ret!=EOF)
//         {
//             CheckAddCapacity(ps);
//             ps->data[i] = tmp;
//             ps->counter++;
//             i++;
//         }

//     } while (ret != EOF);
// }
// 静态版本
//  void initContact(contacts *ps)
//  {
//      assert(ps != NULL);

//     ps->counter = 0;
//     memset(ps->data, 0, sizeof(ps->data)); // ！细细体会这个地方看鹏哥的
// }

void DestroyContact(contacts *ps) // 有借有还
{
    assert(ps);

    free(ps->data);
    ps->data = NULL;
}

void CheckAddCapacity(contacts*ps)
{
    if (ps->capacity == ps->counter)
    {
        pInformation *ptr = (pInformation *)realloc(ps->data, (ps->capacity + ADD_NUM) * sizeof(pInformation));
        //~细细体会
        // 这个地方其实可以直接用ps->data但是打好基础养好习惯现在是项目小内存小，不需要考虑内存问题，以后就不一定了
        if (ptr == NULL)
        {
            printf("add::%s", strerror(errno));
            return;
        }
        else
        {
            printf("扩容成功\n");
            ps->data = ptr;
            ps->capacity += ADD_NUM;
        }
    }
}

void add(contacts *ps)
{
    assert(ps != NULL);

    CheckAddCapacity(ps);

    printf("请输入姓名:>");
    scanf("%s", ps->data[ps->counter].name); // 我想加一个返回功能就是输错了返回上一级

    if (0 == strcmp(ps->data[ps->counter].name, "9"))
    {
        printf("返回上一级\n");
        return; //~初步做了一个返回的东东
    }

    printf("请输入年龄:>");
    scanf("%d", &(ps->data[ps->counter].age)); // ！ 这个地方一定要注意，&，其他都是数组数组名就是首元素地址

    printf("请输入性别:>");
    scanf("%s", ps->data[ps->counter].sex);

    printf("请输入电话:>");
    scanf("%s", ps->data[ps->counter].tele);

    printf("请输入地址:>");
    scanf("%s", ps->data[ps->counter].addr);
    ps->counter++;
    printf("添加成功\n");
}
// 静态版本
//  void add(contacts *ps)
//  {
//      assert(ps != NULL);

//     if (ps->counter == PEOPLE) // ！没想严密
//     {
//         printf("通讯录已满,添加失败。\n");
//     }
//     else
//     {
//         printf("请输入姓名:>");
//         scanf("%s", ps->data[ps->counter].name); // 我想加一个返回功能就是输错了返回上一级

//         if (0 == strcmp(ps->data[ps->counter].name, "9"))
//         {
//             printf("返回上一级\n");
//             return; //~初步做了一个返回的东东
//         }

//         printf("请输入年龄:>");
//         scanf("%d", &(ps->data[ps->counter].age)); // ！ 这个地方一定要注意，&，其他都是数组数组名就是首元素地址

//         printf("请输入性别:>");
//         scanf("%s", ps->data[ps->counter].sex);

//         printf("请输入电话:>");
//         scanf("%s", ps->data[ps->counter].tele);

//         printf("请输入地址:>");
//         scanf("%s", ps->data[ps->counter].addr);
//         ps->counter++;
//     }
// }

void show(const contacts *ps)
{
    assert(ps != NULL);

    printf("\n*******                                                                *******\n");
    printf("===================================*通讯录*===================================\n");
    printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n", "序号", "姓名", "年龄", "性别", "电话", "地址");

    for (int i = 0; i < ps->counter; i++) // 这个地方对ps->counter 的理解
    {
        printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", i + 1, ps->data[i].name,
               ps->data[i].age, ps->data[i].sex, ps->data[i].tele, ps->data[i].addr);
    }
    // ！这个地方需要注意制表的方法细细体会
    printf("\n");
}
// 删除
void del(contacts *ps)
{
    assert(ps != NULL);

    // 找到
    char ret[20] = {0};
    int flag = 1;
    int a = 0;
    int b = 0;

    printf("请输入想要删除的人的名字:>");
    scanf("%s", ret);

    if (0 == strcmp(ret, "9"))
    {
        printf("返回上一级\n");
        return; //~初步做了一个返回的东东
    }

    int pos = discover(ret, ps);
    // 删除
    if (pos > 0)
    {
        // if(pos>1)
        // {
        //     printf("请输入想要删除的人的序号:>");
        //     scanf("%d", &a);
        // }//这里想做一个就一个的话自动删如果大于1选择
        do
        {
            printf("请输入想要删除的人的序号:>");
            scanf("%d", &a);
            printf("*<   0.back    1.continue  >*");
            scanf("%d", &b);
        } while (b == 0);

        for (int i = a; i < ps->counter; i++)
        {
            ps->data[i - 1] = ps->data[i];
        }
        ps->counter--; // ！有bug就是你输入了不是这里面的数字也会误删
        // 目前想到的就只能做一个回退，还不错
        //删除的时候容量没变
        printf("删除成功\n");
    }
}

void fin(contacts *ps)
{
    assert(ps != NULL);

    char arr1[20] = {0};

    printf("请输入你想查找的人的名字:>");
    scanf("%s", arr1);

    if (0 == strcmp(arr1, "9"))
    {
        printf("返回上一级\n");
        return; //~初步做了一个返回的东东
    }
    int temp = 0;

    temp = discover(arr1, ps);
}

int discover(const char ret[], const contacts *ps)
{
    assert(ps != NULL && ret != NULL);
    int cnt = 0;

    for (int i = 0; i < ps->counter; i++)
    {
        int tmp = 1;

        tmp = strcmp(ret, ps->data[i].name);

        if (tmp == 0)
        {
            if (cnt == 0)
            {
                printf("找到如下结果:\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", i + 1, ps->data[i].name,
                       ps->data[i].age, ps->data[i].sex, ps->data[i].tele, ps->data[i].addr);
            }
            else
            {
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", i + 1, ps->data[i].name,
                       ps->data[i].age, ps->data[i].sex, ps->data[i].tele, ps->data[i].addr);
            }
            cnt++;
        }
    }

    if (cnt == 0)
    {
        int a = 0;

        printf("未找到该人，请您检查输入是否正确。\n");
        printf("是否还需重新输入一次   0.不需要   1.重新输入:>");
        scanf("%d", &a);

        if (a == 1)
        {
            char ret1[20] = {0};
            printf("请重新输入ta的姓名:>");
            scanf("%s", ret1);
            cnt = discover(ret1, ps);
        }
        else if (a != 0 && a != 1)
        {
            do
            {
                printf("请重新输入:>\n");
                scanf("%d", &a);
            } while (a != 0 && a != 1);
        }
    }
    return cnt;
}
// ！！tmd刚刚一不小心把这两个函数的功能给删了tmd头文件声明这里不给定义直接终端打不开，我找问题找半天
void edi(contacts *ps)
{
    assert(ps != NULL);

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    char arr2[20] = {0};

    printf("请输入你需要更改信息的人的名字:>");
    scanf("%s", arr2);

    if (0 == strcmp(arr2, "9"))
    {
        printf("返回上一级\n");
        return; //~初步做了一个返回的东东
    }

    int ret = discover(arr2, ps);
    if (ret > 0)
    {
    // if(ret>1)
    // {
    //     printf("请输入你需要更改信息的人的序号:>");
    //     scanf("%d", &a);
    // } //想做一个如果只有一个就直接自动改
    out:
        // do
        // {
        //     printf("请输入你需要更改信息的人的序号   *< 0.back >*  :>");
        //     scanf("%d", &a); // ！这里也有bug，如果你输错了序号就是输入的是没显示的也会修改
        //     // 目前想到的就只能做一个回退
        // } while (a == 0);

        do
        {
            printf("请输入你需要更改信息的人的序号:>");
            scanf("%d", &a); // ！这里也有bug，如果你输错了序号就是输入的是没显示的也会修改
            // 目前能想到的办法，还凑合
            printf("*<   0.back    1.continue  >*");
            scanf("%d", &d);
        } while (d == 0);
        do
        {
            printf("*******************************\n");
            printf("*******  1.姓名 2.年龄  *******\n");
            printf("*******  3.性别 4.电话  *******\n");
            printf("*******  5.地址 9.back  *******\n");
            printf("*******************************\n");

            printf("请输入你需要更改ta的什么信息:>");
            scanf("%d", &b);
            switch (b)
            {
            case 1:
            {
                char temp[MAX_NAME] = {0, [19] = '\0'};
                printf("请输入ta更改后的姓名:>");
                scanf("%s", temp);

                strcpy(ps->data[a - 1].name, temp);
                printf("更改成功\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", a, ps->data[a - 1].name,
                       ps->data[a - 1].age, ps->data[a - 1].sex, ps->data[a - 1].tele, ps->data[a - 1].addr);
            }
            break;
            case 2:
            {
                int temp = 0;
                printf("请输入ta更改后的年龄:>");
                scanf("%d", &temp);
                ps->data[a - 1].age = temp;

                printf("更改成功\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", a, ps->data[a - 1].name,
                       ps->data[a - 1].age, ps->data[a - 1].sex, ps->data[a - 1].tele, ps->data[a - 1].addr);
            }
            break;
            case 3:
            {
                char temp[MAX_SEX] = {0, [MAX_SEX - 1] = '\0'};
                printf("请输入ta更改后的性别:>");
                scanf("%s", temp);

                strcpy(ps->data[a - 1].sex, temp);
                printf("更改成功\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", a, ps->data[a - 1].name,
                       ps->data[a - 1].age, ps->data[a - 1].sex, ps->data[a - 1].tele, ps->data[a - 1].addr);
            }
            break;
            case 4:
            {
                char temp[MAX_TELE] = {0, [MAX_TELE - 1] = '\0'};
                printf("请输入ta更改后的电话:>");
                scanf("%s", temp);

                strcpy(ps->data[a - 1].tele, temp);
                printf("更改成功\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", a, ps->data[a - 1].name,
                       ps->data[a - 1].age, ps->data[a - 1].sex, ps->data[a - 1].tele, ps->data[a - 1].addr);
            }
            break;
            case 5:
            {
                char temp[MAX_ADDR] = {0, [MAX_ADDR - 1] = '\0'};
                printf("请输入ta更改后的地址:>");
                scanf("%s", temp);

                strcpy(ps->data[a - 1].addr, temp);
                printf("更改成功\n");
                printf("%-3s\t%-20s\t%-5s\t%-5s\t%-12s\t%-30s\n",
                       "序号", "姓名", "年龄", "性别", "电话", "地址");
                printf("%-3d\t%-20s\t%-5d\t%-5s\t%-12s\t%-30s\n", a, ps->data[a - 1].name,
                       ps->data[a - 1].age, ps->data[a - 1].sex, ps->data[a - 1].tele, ps->data[a - 1].addr);
            }
            break;
            case 9:
            {
                printf("返回上一级\n");
                goto out;
            }
            break;
            default:
            {
                printf("请重新输入你需要更改的信息序号\n");
            }
            }
            if (b >= 0 && b <= 5)
            {
                printf("是否还需要更改该人的其他信息  *<  0.不需要，请退出   1.需要请继续   >*  :>");
                scanf("%d", &c);
            }
        } while (c == 1 || b >= 6 || b <= 0);
    }
}

int cmpPinformationByName(const void *e1, const void *e2)
{
    assert(e1 != NULL && e2 != NULL);
    return strcmp(((pInformation *)e1)->name, ((pInformation *)e2)->name);
    // ！细细体会看鹏哥的结构体指针，结构体数组的用法
}
// 排序
void sort(contacts *ps)
{
    assert(ps != NULL);

    qsort(ps->data, ps->counter, sizeof(pInformation), cmpPinformationByName);
    // ！qsort使用的还不够熟练
    // 可以用个switch继续做一个选自排序方式by name
    printf("排序成功\n");

    show(ps);
}
//我自己做的版本
// void SaveContact(const contacts *ps)
// {
//     FILE *pfWrite = fopen("contact.txt", "w");
//     //你在运行程序的时候进行增删查改，都不管打开的时候全都清空，关闭之前把所有修改的全都存进去。
//     if(pfWrite==NULL)
//     {
//         perror("SaveContact");
//         return;
//     }
//     //写文件
//     for (int i = 0; i < ps->counter;i++)
//     {
//         fprintf(pfWrite, "%s %d %s %s %s \n", ps->data[i].name, ps->data[i].age, ps->data[i].sex, ps->data[i].tele, ps->data[i].addr);
//     }
//     fclose(pfWrite);
//     pfWrite == NULL;
// }

void SaveContact(const contacts *ps)
{
    FILE *pfWrite = fopen("contact.txt", "wb");
    // 你在运行程序的时候进行增删查改，都不管打开的时候全都清空，关闭之前把所有修改的全都存进去。
    if (pfWrite == NULL)
    {
        perror("SaveContact");
        return;
    }
    // 写文件
    for (int i = 0; i < ps->counter; i++)
    {
        fwrite(ps->data + i, sizeof(pInformation), 1, pfWrite);
    }

    fclose(pfWrite);
    pfWrite == NULL;
}
