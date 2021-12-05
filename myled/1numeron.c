#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Eranp[3], Branp[3];
int n = 0, y = 0;
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
            n++;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int w = 0; w < 3; w++)
        {
            if (rdom[i] == user[w])
                y++;
        }
    }
}
void set(int var)
{
    if (var == 1)
        system("echo 1> /dev/myled0");
    else if (var == 2)
        system("echo 2> /dev/myled0");
    else if (var == 3)
    {
        system("echo 3> /dev/myled0");
        printf("var%d\n", var);
    }
}
int main()
{
    system("sudo insmod 1myled.ko");
    system("sudo chmod 666 /dev/myled0");
    char array[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char user[3];
    shuffle(array, 9);
    for (int i = 0; i < 3; i++)
    {
        printf("%c", array[i]);
    }
    scanf("%c%c%c", &user[0], &user[1], &user[2]);
    game(array, user);
    printf("%d,%d\n", n, y);
    if (n != 0)
    {
        system("echo E> /dev/myled0");
        if (n == 1)
            system("echo 1> /dev/myled0");
        else if (n == 2)
            system("echo 2> /dev/myled0");
        else if (n == 3)
            system("echo 3> /dev/myled0");
    }
    if (y != 0)
    {
        system("echo B> /dev/myled0");
        if (y == 1)
            system("echo 1> /dev/myled0");
        else if (y == 2)
            system("echo 2> /dev/myled0");
        else if (y == 3)
            system("echo 3> /dev/myled0");
    }
    else
        system("echo Q> /dev/myled0");

    return 0;
}
