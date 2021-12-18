#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // header file for directory
#include <windows.h>
#include <MMsystem.h> //headers file for music

#define MAX 20
struct node
{
    int index;
    struct node *prev;
    char data[MAX];
    struct node *next;
};

struct node *front = NULL, *temp, *new;

// for checking substring
int substring(char string[], char substring[])
{
    int stringLength = strlen(string), substringLength = strlen(substring);
    int i = 0, flag = 0;

    while (string[i] != '\0')
    {
        if (string[i] == substring[0])
        {
            if (i + substringLength > stringLength)
            {
                break;
            }
            if (strncmp(string + i, substring, substringLength) == 0)
            {
                flag = 1;
                break;
            }
        }
        i++;
    }
    return flag;
}

void insert(char data[], int count)
{
    new = (struct node *)malloc(sizeof(struct node));
    strcpy(new->data, data);
    new->index = count;
    new->next = NULL;

    if (front == NULL) // if it's first node
    {
        new->prev = NULL;
        front = new;
        return; // exit the function
    }
    temp = front;
    while (temp->next != NULL)
        temp = temp->next;
    new->prev = temp;
    temp->next = new;
}

void initialize()
{
    int indexCount = 1;
    DIR *dir;
    struct dirent *file;
    int sizefun = 0;
    dir = opendir("."); // open current directory
    if (dir)
    {
        while ((file = readdir(dir)) != NULL)
        {
            if (substring(file->d_name, ".wav"))
            {
                insert(file->d_name, indexCount);
                indexCount++;
            }
        }
        closedir(dir);
    }
}

void play(int index, struct node *temp)
{
    if (index != NULL)
    {
        // for playing a specific song by index
        int i;
        front = temp;
        for (i = 1; i < index; i++)
        {
            front = front->next;
        }
    }
    // code for playing

    puts(front->data);
    PlaySound(TEXT(front->data), NULL, SND_FILENAME | SND_ASYNC);
};

void next()
{
    if (front != NULL)
    {
        play(NULL, front);
        front = front->next;
    }
    else
    {
        puts("End of playlist!");
    }
}

void previous()
{
    if (front->prev != NULL)
    {
        front = front->prev;
        play(NULL, front);
    }
    else
    {
        puts("No file before");
    }
}

void stop()
{
    PlaySound(NULL, 0, 0);
    puts("Stopped!!!");
}

void queue()
{
    int indexCount = 1;
    temp = front;
    while (temp != NULL)
    {
        printf("%d. %s\n", indexCount++, temp->data);
        temp = temp->next;
    }
    // char q = getchar();
    // for the freezing and good user experince
}

int main()
{
    // intializers
    initialize();
    puts("\t\t\tWelcome to iMusic");
    system("color 4f");

    // for our refernce for deallocating memorr
    struct node *reference = front;

    int choice;
    while (1)
    {
        printf("\n \n");
        puts("1. Play");
        puts("2. Next");
        puts("3. Previous");
        puts("4. Stop");
        puts("5. Current Queue");
        puts("6. Exit");
        scanf("%d", &choice);
        // fflush(stdin);
        system("cls");
        switch (choice)
        {
        case 1:
            queue();
            printf("Which song :");
            scanf("%d", &choice);
            system("cls");
            play(choice, reference);
            break;

        case 2:
            next();
            break;

        case 3:
            previous();
            break;

        case 4:
            stop();
            break;

        case 5:
            queue();
            break;

        case 6:
            // free all allocated memory.
            printf("BYE!!!");
            temp = reference;
            while (temp != NULL)
            {
                front = temp;
                temp = temp->next;
                free(front);
            }
            exit(1);

        default:
            puts("Enter a valid option!");
            break;
        }
    }
    return 0;
}