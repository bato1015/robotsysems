#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Eranp[3], Branp[3];
int E = 0, B = 0;
void shuffle(char array[], int size)
{
    srand((int)time(NULL));
    for (int i = 0; i < size; i++)
    {
        int j = rand() % size;
        int t = array[i];
        array[i] = array[j];
        array[j] = t;
    }
}
void game(char user[], char rdom[])
{
    for (int i = 0; i < 3; i++)
    {
        if (user[i] == rdom[i])
            E++;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int w = 0; w < 3; w++)
        {
            if (rdom[i] == user[w])
                B++;
        }
    }
}
void set()
{
    if (E == 0 && B == 0)
        system("echo Q> /dev/myled0");
    if (E == 1)
        system("echo A> /dev/myled0");
    else if (E == 2)
        system("echo B> /dev/myled0");
    else if (E == 3)
        system("echo C> /dev/myled0");

    if (B == 1)
        system("echo a> /dev/myled0");
    else if (B == 2)
        system("echo b> /dev/myled0");
    else if (B == 3)
        system("echo c> /dev/myled0");
}
int main()
{
    char space;
    system("sudo insmod 2myled.ko");
    system("sudo chmod 666 /dev/myled0");
    char array[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char user[3];
    shuffle(array, 9);
    //答え
    /*for (int i = 0; i < 3; i++)
        printf("%c", array[i]);
    printf("\n");*/
    while (1)
    {
        B = 0;
        E = 0;
        scanf("%c%c%c", &user[0], &user[1], &user[2]);
        if (user[0] == 'g' || user[1] == 'i' || user[2] == 'v')
        {
            system("echo s> /dev/myled0");
            printf("answer==");
            for (int i = 0; i < 3; i++)
                printf("%c", array[i]);
            printf("\n");
            break;
        }
        else if (user[0] == 'h' || user[1] == 'l' || user[2] == 'p')
            printf("%c**\n", array[0]);
        game(array, user);
        printf("%d,%d\n", E, B);
        set();
        if (E == 3)
        {
            system("echo g> /dev/myled0");
            printf("gg\n");
            break;
        }
        scanf("%c", &space);
    }
    return 0;
}
