#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct studentScore
{
    unsigned long number;
    char name[64];
    char gender;
    int stage;
    float sumScore;
    float score[8];
};
struct studentScore * databaseP;

int databaseVolume = 1;
int dataVolume = 0;
int advanceStaticFlag = 0;
int deleteWarningFlag = 1;
int operateTarget = -1;
int printDataList(struct studentScore * databasePointer, int dataLength);
int readInDatabase(void);
int saveDatabase(void);
int scoreRecordAdd(struct studentScore *newRecordData);
int recordAddInterface(struct studentScore * databasePointer);
int eraseDatabase(struct studentScore * databasePointer);
void databaseTerminal(void);
void terminalBranch(int branchCode);
void helpInfo(int density);
int extendMemBuffer(int increment);
void stats(void);
void advancedStaticGuide(void);
void advanceInsight(struct studentScore * databasePointer, int dataLength);
void advanceOrder(struct studentScore * databasePointer, int dataLength, int subjectCode, char order);
void advanceOrderInterface(struct studentScore * databasePointer, int dataLength);
int advanceListing(struct studentScore * databasePointer, int dataCode, int format);
int recordFinder(struct studentScore * databasePointer, int dataLength);
int recordRemove(struct studentScore * databasePointer, int dataLength, int dataCode);
int recordEdit(struct studentScore * databasePointer, int dataLength, int dataCode);

FILE *databaseFileP;
size_t dataBlockSize = sizeof(struct studentScore);// + 8 * sizeof(float);

int main(void)
{
    printf("\t╔════════════════════════╗\n");
    printf("\t║    欢迎使用数据库系统！    ║\n");
    printf("\t╚════════════════════════╝\n");
    helpInfo(0);
    puts("本 databaseTerminal 不具有超级牛力.");
    readInDatabase();
    databaseTerminal();
    return 0;
}
int readInDatabase(void)
//返回值：1读入成功 0读入失败 2读入成功，但是空数据库
{
    if ((databaseFileP = fopen("database.bin", "a+b")) == NULL)
    {
        puts("[E] 读入数据库失败.");
        return 0;
    } else
    {
        fseek(databaseFileP, 0, SEEK_END);
        rewind(databaseFileP);
        fread(&dataVolume, sizeof(int), 1, databaseFileP);
        fread(&databaseVolume, sizeof(int), 1, databaseFileP);
        if (databaseVolume == 0)
            databaseVolume = 1;
        databaseP = (struct studentScore *)malloc(databaseVolume * dataBlockSize);
        fread(databaseP, dataBlockSize, dataVolume, databaseFileP);

        if (ftell(databaseFileP) == 0)
        {
            puts("[M] 数据库不存在.将创建新数据库.");
            fclose(databaseFileP);
            return 2;
        }else
        {
            fclose(databaseFileP);
            return 1;
        }
    }
}
void databaseTerminal(void)
{
    char ch;
    int branch = 0;
    if (advanceStaticFlag)
    {
        printf("%s@localhost:~$ aS+", getlogin());
        char command[99];
        scanf("%s", command);
        if (!strcmp(command, "quit"))
            branch = 11;
        else if (!strcmp(command, "insight"))
            branch = 12;
        else if (!strcmp(command, "order"))
            branch = 13;
        else
        {
            branch = -1;
        }
    }else
    {
        printf("%s@localhost:~$ ", getlogin());
        char command[99];
        scanf("%s", command);
        if (!strcmp(command, "list"))
            branch = 0;
        else if (!strcmp(command, "add"))
            branch = 1;
        else if (!strcmp(command, "erase"))
            branch = 2;
        else if (!strcmp(command, "reload"))
            branch = 3;
        else if (!strcmp(command, "wquit"))
            branch = 4;
        else if (!strcmp(command, "quit"))
            branch = 5;
        else if (!strcmp(command, "help"))
            branch = 6;
        else if (!strcmp(command, "write"))
            branch = 7;
        else if (!strcmp(command, "extend"))
            branch = 8;
        else if (!strcmp(command, "stats"))
            branch = 9;
        else if (!strcmp(command, "statistic"))
            branch = 10;
        else if (!strcmp(command, "find"))
            branch = 14;
        else if (!strcmp(command, "delete"))
        {
            scanf("%d", &operateTarget);
            branch = 15;
        }
        else if (!strcmp(command, "edit"))
        {
            scanf("%d", &operateTarget);
            branch = 16;
        }
        else
        {
            branch = -1;
        }
    }
    while ((ch = getchar()) != '\n' && ch != EOF);
    terminalBranch(branch);
}
void helpInfo(int density)
{
    switch (density)
    {
    case 0:
        printf("常用命令:\n");
        printf("(输入\"help\"获取完整命令列表和帮助信息)\n");
    case 1:
        printf(
        "\tlist\t\t展示所有在库记录\n"
        "\tadd\t\t\t新建记录\n"
        "\tfind\t\t查找在库记录\n"
        "\tstatistic\t高级统计\n"
        "\twquit\t\t保存并退出\n"
        "\thelp\t\t获取帮助\n"
        );
        break;
    case 2:
        printf(
        "\twrite\t\t保存\n"
        "\tquit\t\t退出而不保存修改\n"
        "\tdelete\t\t删除在库记录 [targetCode] 待删除的记录编号"
        "\n\t\t\t\t      具有一个参数."
        "\n\t\t\t\t      使用时需要指定 targetCode.例如: delete 0.这将删除列表中第1个项目.\n"
        "\tedit\t\t修改在库记录 [targetCode] 待修改的记录编号"
        "\n\t\t\t\t      具有一个参数."
        "\n\t\t\t\t      使用时需要指定 targetCode.例如: edit 0.这将修改列表中第1个项目.\n"
        "\terase\t\t清空数据库\n"
        "\treload\t\t重装载数据库\n"
        "\textend\t\t数据库缓冲区扩容\n\t\t\t\t      程序默认分配内存大小最多装填10条记录,该操作用于手动扩大 malloc() 分配的内存."
        "\n\t\t\t\t      当在库记录数量已经达到缓冲区最大限制,这一过程通常会自动进行,不需要使用该操作."
        "\n\t\t\t\t      进行该操作后必须立刻重新读取数据库,否则您将无法进行任何操作.如有计划回滚到过去"
        "\n\t\t\t\t      的数据库版本并舍弃所有未完成的工作,请先手动备份.\n"
        "\tstats\t\t显示状态信息\n"
        );
        printf("可能见到的标识:\n");
        printf(
        "\t[M]\t\t完成某一操作后,您将收到消息.\n"
        "\t[W]\t\t进行破坏性操作前,您将收到警告.\n"
        "\t[E]\t\t操作无法顺利完成时,您将收到错误.\n"
        "\t[EOF]\t列表完全显示后,您将看到 End Of File 标志.\n"
        );
    }
}

void terminalBranch(int branchCode)
{
    switch (branchCode)
    {
    case 0:
        printDataList(databaseP, dataVolume);
        break;
    case 1:
        recordAddInterface(databaseP);
        break;
    case 2:
        eraseDatabase(databaseP);
        break;
    case 3:
        readInDatabase();
        break;
    case 4:
        saveDatabase();
        exit(0);
    case 5:
        fclose(databaseFileP);
        exit(0);
    case 6:
        printf("\t╔══════════════════════════╗\n");
        printf("\t║ 学生成绩数据管理系统         ║\n");
        printf("\t╚══════════════════════════╝\n");
        printf("以下是完整的命令列表及说明：\n");
        helpInfo(1);
        helpInfo(2);
        break;
    case 7:
        saveDatabase();
        break;
    case 8:
        extendMemBuffer(5);
        break;
    case 9:
        stats();
        break;
    case 10:
        advancedStaticGuide();
        break;
    case 11:
        advanceStaticFlag = 0;
        puts("[M] 已离开高级统计模式.高级统计功能不再可用.");
        break;
    case 12:
        advanceInsight(databaseP, dataVolume);
        break;
    case 13:
        advanceOrderInterface(databaseP, dataVolume);
        break;
    case 14:
        recordFinder(databaseP, dataVolume);
        break;
    case 15:
        recordRemove(databaseP, dataVolume, operateTarget);
        operateTarget = -1;
        break;
    case 16:
        recordEdit(databaseP, dataVolume, operateTarget);
        operateTarget = -1;
        break;
    default:
        puts("[E] databaseTerminal: 没有此命令.");
    }
    databaseTerminal();
}
int printDataList(struct studentScore * databasePointer, int dataLength)
{
    printf("\t+++++++++++++++++++++++{完整列表}+++++++++++++++++++++++\n");
    printf("\t= 学号        姓名\t性别\t学段\t总分\t英语  数学  语文  地理  历史  英语  程序设计   高等数学\n");
    for (int i = 0; i < dataLength; i++)
    {
        int classLimit = 3;
        printf("\t- %10lu %s\t%s\t",
            databasePointer[i].number,
            databasePointer[i].name,
            (databasePointer[i].gender == '0' ? "男":"女")
            );
        switch (databasePointer[i].stage)
        {
        case 0:
            printf("小学 ");
            classLimit = 3;
            break;
        case 1:
            printf("中学 ");
            classLimit = 5;
            break;
        case 2:
            printf("大学 ");
            classLimit = 8;
            break;
        default:
            printf("大学 ");
            classLimit = 8;
            break;
        }
        printf("%.1f\t", databasePointer[i].sumScore);
        for (int j = 0; j < classLimit; j++)
            if (databasePointer[i].score[j] < 0)
                printf("--  ");
            else
                printf("%.1f ", databasePointer[i].score[j]);
        printf("\n");
    }
    puts("[EOF] 已输出完整列表.");
    return 0;
}
int saveDatabase(void)
//返回值 0失败 1成功
{
    databaseFileP = fopen("database.bin", "wb");
    rewind(databaseFileP);
    fwrite(&dataVolume, sizeof(int), 1, databaseFileP);
    fwrite(&databaseVolume, sizeof(int), 1, databaseFileP);
    fwrite(databaseP, dataBlockSize, dataVolume, databaseFileP);
    if (fclose(databaseFileP) == EOF)
    {
        fclose(databaseFileP);
        puts("[E] 写入数据库失败.");
        return 0;
    }else
    {
        puts("[M] 写入数据库成功.");
        return 1;
    }
}
int scoreRecordAdd(struct studentScore *newRecordData)
//返回值 0失败 1成功
{
    float sumScore;
    if (dataVolume >= databaseVolume)
    {
        puts("[E] 写入失败1条数据.");
        puts("[M] 数据库缓冲区已满,将尝试自动扩容.所有未保存的更改将于稍后自动完成.");
        extendMemBuffer(1);
    }
    int mark = dataVolume;
    databaseP[mark].number = newRecordData->number;
    strcpy(databaseP[mark].name, newRecordData->name);
    databaseP[mark].gender = newRecordData->gender;
    databaseP[mark].stage = newRecordData->stage;
    for (int i = 0; i < 8; i++)
    {
        databaseP[mark].score[i] = newRecordData->score[i];
        sumScore += newRecordData->score[i];
    }
    databaseP[mark].sumScore = sumScore;
    dataVolume++;
    puts("[M] 成功写入1条数据.");
    return 1;
}
int recordAddInterface(struct studentScore * databasePointer)
{
    char ch;
    struct studentScore *stuToAdd;
    if ((stuToAdd = (struct studentScore *)malloc(dataBlockSize)) == NULL)
    {
        puts("[E] 写入1条数据失败.内存分配失败.");
        return 0;
    }
    puts("[M] 正在追加新纪录.");
    char uniqueNumberFlag;
    while (!uniqueNumberFlag)
    {
        printf("\t学号: ");
        scanf("%lu", &stuToAdd->number);
        uniqueNumberFlag = 1;
        for (int j = 0; j < dataVolume; j++)
            if (databasePointer[j].number == stuToAdd->number)
            {
                puts("[W] 该学生成绩已存在.无法创建具有相同学号的项目.指定不同学号以追加新项目.");
                uniqueNumberFlag = 0;
                break;
            }
    }

    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\t姓名: ");
    scanf("%s", stuToAdd->name);

    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\t性别(0为男/1为女): ");
    scanf("%c", &stuToAdd->gender);

    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\t学段(0为小学生/1为中学生/2为大学生): ");
    scanf("%d", &stuToAdd->stage);

    printf("\t成绩(顺序：英语 数学 语文 地理 历史 英语 程序设计 高等数学，没有的科目填-1): ");
    while ((ch = getchar()) != '\n' && ch != EOF);
    for (int i = 0; i < 8; i++)
        scanf("%f", &stuToAdd->score[i]);

    scoreRecordAdd(stuToAdd);
    free(stuToAdd);
    stuToAdd = NULL;
    return 1;
}
int eraseDatabase(struct studentScore * databasePointer)
{
    databaseP = (struct studentScore *)malloc(databaseVolume * dataBlockSize);
    dataVolume = 0;
    databaseVolume = 0;
    saveDatabase();
    if (readInDatabase() != 0)
    {
        puts("数据库空覆写成功！");
        return 1;
    } else
    {
        puts("数据库空覆写失败！");
        return 0;
    }
}
int extendMemBuffer(int increment)
{
    databaseVolume += increment;
    if (saveDatabase())
    {
        puts("[M] 数据库扩容成功.");
        readInDatabase();
        return 1;
    }else{
        fclose(databaseFileP);
        puts("[E] 改动未能成功写入.数据库扩容失败.");
        return 0;
    }
}
void stats(void)
{
    puts("[M] 状态信息统计已完成.");
    printf("\t已登录的用户: \"%s\"\n", getlogin());
    printf("\t数据容量: %d\n", dataVolume);
    printf("\t数据库容量: %d\n", databaseVolume);
    printf("\t数据库占用的内存大小: %zd Byte\n", databaseVolume * dataBlockSize);
    printf("\t数据库文件占用的内存大小: %zd Byte\n", databaseVolume * dataBlockSize + 2 * sizeof(int));
    printf("\t数据块的大小: %zd Byte\n", dataBlockSize);
}
void advancedStaticGuide(void)
{
    advanceStaticFlag = 1;
    puts("[M] 已进入高级统计模式.要使用非高级统计功能,须先行退出高级统计模式.");
    puts("[W] 高级统计模式下对列表的操作会直接作用于已读入数据库.如有计划回滚到过去");
    puts("    的数据库版本并舍弃在高级统计模式下的工作,请重装载数据库.");
    printf("高级统计参数列表:\n");
    printf(
        "\taS+insight\t\t参数洞察\n\t\t\t\t\t      查看学生总数和各学科的总体情况.\n"
        "\taS+order\t\t按需要排序\n"
        "\taS+quit\t\t\t离开高级统计模式\n"
        );
}
void advanceInsight(struct studentScore * databasePointer, int dataLength)
{
    puts("[M] 已生成参数洞察.");
    printf("\t学生总数:%d\n", dataVolume);
    printf("\t各科目平均分:英语    数学    语文    地理   历史    英语    程序设计 高等数学\n");
    printf("\t\t\t ");
    for (int i = 0; i < 8; i++)
    {
        int count = 0;
        float avr = 0;
        for (int j = 0; j < dataVolume; j++)
        {
            if (databasePointer[j].score[i] != -1)
            {
                avr += databasePointer[j].score[i];
                count++;
            }
        }
        avr /= count;
        printf("%6.1f ", avr);
    }
    printf("\n");
}
void advanceOrder(struct studentScore * databasePointer, int dataLength, int subjectCode, char order)
{
    if (subjectCode == 8)
    {
        for (int i = 0; i < dataLength - 1; i++)
        {
            for (int j = 0; j < dataLength - i - 1; j ++)
            {
                if (order == 'a')
                {
                    if (databasePointer[j].sumScore > databasePointer[j + 1].sumScore)
                    {
                        struct studentScore temp = databasePointer[j];
                        databasePointer[j] = databasePointer[j + 1];
                        databasePointer[j + 1] = temp;
                    }
                }else if (order == 'd')
                {
                    if (databasePointer[j].sumScore < databasePointer[j + 1].sumScore)
                    {
                        struct studentScore temp = databasePointer[j];
                        databasePointer[j] = databasePointer[j + 1];
                        databasePointer[j + 1] = temp;
                    }
                }
            }
        }
    }else
    {
        for (int i = 0; i < dataLength - 1; i++)
        {
            for (int j = 0; j < dataLength - i - 1; j ++)
            {
                if (order == 'a')
                {
                    if (databasePointer[j].score[subjectCode] > databasePointer[j + 1].score[subjectCode])
                    {
                        struct studentScore temp = databasePointer[j];
                        databasePointer[j] = databasePointer[j + 1];
                        databasePointer[j + 1] = temp;
                    }
                }else if (order == 'd')
                {
                    if (databasePointer[j].score[subjectCode] < databasePointer[j + 1].score[subjectCode])
                    {
                        struct studentScore temp = databasePointer[j];
                        databasePointer[j] = databasePointer[j + 1];
                        databasePointer[j + 1] = temp;
                    }
                }
            }
        }
    }
}
void advanceOrderInterface(struct studentScore * databasePointer, int dataLength)
{
    int subjectCode;
    char orderMode, ch;
    puts("[M] 进入按需要排序模式.该模式可以针对某一特定科目做升序或降序排列.");
    puts("    需要提供排序信息.排序完成后,将显示最新列表.");
    printf("\t对象科目代号(0英语 1数学 2语文 3地理 4历史 5英语 6程序设计 7高等数学 8总分): ");
    scanf("%d", &subjectCode);
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\t排序方式(a升序 d 降序): ");
    scanf("%c", &orderMode);
    advanceOrder(databasePointer, dataLength, subjectCode, orderMode);
    puts("[M] 高级排序已完成.");
    printDataList(databasePointer, dataLength);
}
int advanceListing(struct studentScore * databasePointer, int dataCode, int format)
{
    printf("\t++++++++++++++++++++++{自定义列表}++++++++++++++++++++++\n");
    printf("\t= 学号        姓名\t性别\t学段\t总分\t英语  数学  语文  地理  历史  英语  程序设计   高等数学\n");

    int classLimit = 3;
    printf("\t- %10lu %s\t%s\t",
        databasePointer[dataCode].number,
        databasePointer[dataCode].name,
        (databasePointer[dataCode].gender == '0' ? "男":"女")
        );
    switch (databasePointer[dataCode].stage)
    {
    case 0:
        printf("小学 ");
        classLimit = 3;
        break;
    case 1:
        printf("中学 ");
        classLimit = 5;
        break;
    case 2:
        printf("大学 ");
        classLimit = 8;
        break;
    default:
        printf("大学 ");
        classLimit = 8;
        break;
    }
    printf("%.1f\t", databasePointer[dataCode].sumScore);
    for (int j = 0; j < classLimit; j++)
        if (databasePointer[dataCode].score[j] < 0)
            printf("--  ");
        else
            printf("%.1f ", databasePointer[dataCode].score[j]);
    printf("\n");
    return 1;
}
int recordFinder(struct studentScore * databasePointer, int dataLength)
{
    int keywordType;
    char ch, foundFlag, keywordString[64];
    unsigned long keywordNumber;
    printf("\t欲查询数据库中的内容,请选择关键字类型(0学号 1姓名):");
    scanf("%d", &keywordType);
    while ((ch = getchar()) != '\n' && ch != EOF);
    foundFlag = 0;
    printf("\t请输入关键字:");
    if (keywordType == 0)
    {
        scanf("%lu", &keywordNumber);
        for (int i = 0; i < dataLength; i++)
        {
            if (databasePointer[i].number == keywordNumber)
            {
                puts("[M] 找到一条记录.");
                foundFlag = 1;
                advanceListing(databasePointer, i, 0);
                break;
            }
        }
    } else if (keywordType == 1)
    {
        scanf("%s", keywordString);
        for (int i = 0; i < dataLength; i++)
        {
            if (!strcmp(databasePointer[i].name, keywordString))
            {
                puts("[M] 找到一条记录.");
                foundFlag = 1;
                advanceListing(databasePointer, i, 0);
                break;
            }
        }
    }
    if (!foundFlag)
    {
        puts("[M] 没有找到相关内容.");
        return 0;
    }
    puts("[EOF] 已输出完整结果.");
    return 1;
}
int recordRemove(struct studentScore * databasePointer, int dataLength, int dataCode)
{
    if (operateTarget > dataLength - 1)
    {
        puts("[E] 删除1条记录失败.不存在的记录.");
        return 0;
    }
    char ch = '1';
    if (deleteWarningFlag)
    {
        printf("\t确定移除这条数据吗(%lu, %s)?\n", databasePointer[dataCode].number, databasePointer[dataCode].name);
        printf("\t0否 1是 2本次会话均选\"是\":");
        scanf("%c", &ch);
    }
    switch (ch)
    {
    case '2':
        deleteWarningFlag = 0;
    case '1':
        for (int i = dataCode; i < dataLength; i++)
        {
            databasePointer[i] = databasePointer[i + 1];
        }
        memset(&databasePointer[dataLength], 0, dataBlockSize);
        dataVolume--;
        if (databasePointer[dataLength].number == 0 && databasePointer[dataLength].name[0] == '\0')
        {
            puts("[M] 删除了1条记录.");
            return 1;
        }
        puts("[E] 删除1条记录失败.清空内存失败.");
    case '0':
        puts("[E] 删除1条记录失败.用户已放弃.");
        break;
    }
    return 0;
}
int recordEdit(struct studentScore * databasePointer, int dataLength, int dataCode)
{
    char ch;
    int changedCount = 0;
    struct studentScore changedInfo;
    puts("[M] 正在修改1条记录.按下 Enter 将保持该项不变.");
    advanceListing(databasePointer, dataCode, 0);
    char uniqueNumberFlag = 0;
    while (!uniqueNumberFlag)
    {
        printf("\t学号(未更改 %lu): ", databasePointer[dataCode].number);
        if ((ch = getchar()) != '\n')
        {
            ungetc(ch, stdin);
            scanf("%lu", &changedInfo.number);
            uniqueNumberFlag = 1;
            for (int j = 0; j < dataVolume; j++)
            {
                if (j == dataCode)
                {
                    if (databasePointer[dataCode].number == changedInfo.number)
                    {
                        uniqueNumberFlag = 0;
                    }else
                        continue;
                }
                if (databasePointer[j].number == changedInfo.number)
                {
                    puts("[W] 该学生成绩已存在.无法创建具有相同学号的项目.指定不同学号以更改此项目.");
                    uniqueNumberFlag = 0;
                    break;
                }
            }
            if (uniqueNumberFlag)
            {
                printf("\t学号将更改为 %lu.\n", changedInfo.number);
                changedCount++;
            }
            while ((ch = getchar()) != '\n' && ch != EOF);
        } else
        {
            changedInfo.number = databasePointer[dataCode].number;
            uniqueNumberFlag = 1;
        }
    }
    printf("\t姓名(未更改 %s): ", databasePointer[dataCode].name);
    if ((ch = getchar()) != '\n')
    {
        ungetc(ch, stdin);
        scanf("%s", changedInfo.name);
        printf("\t姓名将更改为 %s.\n", changedInfo.name);
        changedCount++;
        while ((ch = getchar()) != '\n' && ch != EOF);
    } else
    {
        strcpy(changedInfo.name, databasePointer[dataCode].name);
    }

    printf("\t性别(未更改 %s 0为男/1为女): ", (databasePointer[dataCode].gender == '0' ? "男":"女"));
    if ((ch = getchar()) != '\n')
    {
        ungetc(ch, stdin);
        scanf("%c", &changedInfo.gender);
        printf("\t性别将更改为 %s.\n", (changedInfo.gender == '0' ? "男":"女"));
        changedCount++;
        while ((ch = getchar()) != '\n' && ch != EOF);
    } else
    {
        changedInfo.gender = databasePointer[dataCode].gender;
    }

    printf("\t学段(未更改 ");
    switch (databasePointer[dataCode].stage)
    {
    case 0:
        printf("小学");
        break;
    case 1:
        printf("中学");
        break;
    case 2:
        printf("大学");
        break;
    default:
        printf("大学");
        break;
    }
    printf(" 0为小学生/1为中学生/2为大学生): ");
    if ((ch = getchar()) != '\n')
    {
        ungetc(ch, stdin);
        scanf("%d", &changedInfo.stage);
        printf("\t学段将更改为 ");
        switch (changedInfo.stage)
        {
        case 0:
            printf("小学.\n");
            break;
        case 1:
            printf("中学.\n");
            break;
        case 2:
            printf("大学.\n");
            break;
        default:
            printf("大学.\n");
            break;
        }
        changedCount++;
        while ((ch = getchar()) != '\n' && ch != EOF);
    } else
    {
        changedInfo.stage = databasePointer[dataCode].stage;
    }


    printf("\t成绩(顺序：英语 数学 语文 地理 历史 英语 程序设计 高等数学，没有的科目填-1): ");
    if ((ch = getchar()) != '\n')
    {
        float sumScore = 0;
        ungetc(ch, stdin);
        for (int i = 0; i < 8; i++)
            scanf("%f", &changedInfo.score[i]);
        printf("\t成绩将更改为 ");
        for (int j = 0; j < 8; j++)
        {
            printf("%.1f ", changedInfo.score[j]);
            if (changedInfo.score[j] > 0)
                sumScore += changedInfo.score[j];
        }
        changedInfo.sumScore = sumScore;
        printf(".新的总成绩为 %.1f.\n", changedInfo.sumScore);
        changedCount++;
        while ((ch = getchar()) != '\n' && ch != EOF);
    } else
    {
        for (int k = 0; k < 8; k++) {
            changedInfo.score[k] = databasePointer[dataCode].score[k];
        }
        changedInfo.sumScore = databasePointer[dataCode].sumScore;
    }
    if (changedCount)
    {
        databasePointer[dataCode] = changedInfo;
        printf("[M] 已修改1条记录.%d项发生了变化.\n", changedCount);
        advanceListing(databasePointer, dataCode, 0);
    } else
    {
        printf("[W] 未修改1条记录.与原内容相比没有任何变化.\n");
    }
}