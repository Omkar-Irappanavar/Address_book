/*
    Name: Omkar Irappanavar
    Date: 28/08/2024
    Description: Address book project (file i/o)
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio_ext.h>

void add_details();
int samedata();
void search_details();
void print_details();
void edit_details();
void delete_details();
void copying(FILE *, FILE *);

struct address
{
    char name[20];
    char mob[15];
    char email[30];
    char address[30];
} details;

int count = 0;

#define RESET     "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

int main()
{
    char ch = 'y';
    int choice;

    while (ch == 'y' || ch == 'Y')
    {
        printf(YELLOW"1. Add Details\n2. Search Details\n3. Print Details\n4. Edit Details\n5. Delete Details\n6. EXIT\n"RESET);
        printf(MAGENTA"Enter your choice : "RESET);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_details();
            break;
        case 2:
            search_details();
            break;
        case 3:
            print_details();
            break;
        case 4:
            edit_details();
            break;
        case 5:
            delete_details();
            break;
        case 6:
            return 0;

        default:
            printf(RED"Error : Invalid choice..!!\n"RESET);
        }

        printf(MAGENTA"\nDo you want to continue(y/n) : "RESET);
        __fpurge(stdin);
        scanf(" %c", &ch);
    }
}

void add_details()
{
    FILE *fptr = fopen("data.csv", "r");
    FILE *fptr1 = fopen("temp.csv", "w");

    fscanf(fptr, "%d\n", &count);
    count++;
    fprintf(fptr1, "%d\n", count);

    for (int i = 1; i < count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        fprintf(fptr1, "%s,%s,%s,%s\n", details.name, details.mob, details.email, details.address);
    }

    printf(CYAN"--------- Enter user details : -----------\n"RESET);
    printf(MAGENTA"Name: "RESET);
    scanf(" %[^\n]", details.name);

    int repeat = 1, len, flag;
    while (repeat)
    {
        printf(MAGENTA"Mobile No.: "RESET);
        scanf(" %[^\n]", details.mob);
        len = strlen(details.mob);

        if (len != 10)
        {
            printf(RED"Error : --- Invalid. Enter 10 digit mobile number(0-9) ---\n"RESET);
            continue;
        }

        flag = 0; // to check mobile number has digits only
        for (int i = 0; i < len; i++)
        {
            if (!(isdigit(details.mob[i])))
            {
                printf(RED"Error : --- Invalid. Enter 10 digit mobile number(0-9) ---\n"RESET);
                flag = 1;
                break;
            }
        }

        if (flag == 1)
            continue;
        else
        {
            int ret = samedata();
            if (ret == 1)
            {
                printf(RED"Error : --- Mobile No. already exists..!! ---\n"RESET);
                continue;
            }
        }

        repeat = 0;
    }

    __fpurge(stdin);

    repeat = 1; // to check email is valid or not
    while (repeat)
    {
        printf(MAGENTA"Email: "RESET);
        scanf(" %[^\n]", details.email);

        len = strlen(details.email);
        if (len < 10)
        {
            printf(RED"Error : --- Invalid. Email shoud be more than 10 characters ---\n"RESET);
            continue;
        }

        flag = 0;
        int i = 0;
        char gmail[5] = ".com";
        char str[15];
        for (i = 0; i < 4; i++)
        {
            str[i] = details.email[len - 4 + i]; // fetching last 10 characters of email
        }
        str[i] = '\0';

        if (strcmp(gmail, str) != 0)
        {
            printf(RED"Error : --- Invalid. Missing '.com' at end ---\n"RESET);
            continue;
        }

        int alpha = 0, digit = 0, spchar=0, space = 0;
        for (i = 0; i < len - 4; i++)
        {
            if (details.email[i] == ' ' || details.email[i] == '\t')
            {
                space = 1;
                break;
            }
            if (isalpha(details.email[i]))
                alpha = 1;
            else if (isdigit(details.email[i]))
                digit = 1;
            else if(details.email[i] == '@')
                spchar = 1;
        }

        if (space == 1)
        {
            printf(RED"Error : --- Space not allowed ---\n"RESET);
            continue;
        }

        if (!((alpha == 1) && (digit == 1) && (spchar == 1)))
        {
            printf(RED"Error : --- Invalid. Missing character or digit or special character ---\n"RESET);
            continue;
        }

        int ret = samedata();
        if (ret == 2)
        {
            printf(RED"Error : --- Email already exists..!! ---\n"RESET);
            continue;
        }
        repeat = 0;
    }

    __fpurge(stdin);

    printf(MAGENTA"Address: "RESET);
    scanf(" %[^\n]", details.address);

    fprintf(fptr1, "%s,%s,%s,%s\n", details.name, details.mob, details.email, details.address);

    fclose(fptr);
    fclose(fptr1);

    fptr = fopen("data.csv", "w");
    fptr1 = fopen("temp.csv", "r");

    copying(fptr, fptr1);
    fclose(fptr);
    fclose(fptr1);
    printf(GREEN"----- %s's data added successfully-----\n"RESET, details.name);

    return;
}

void search_details()
{
    int choice;
    char chname[20] = "\0", chmob[11] = "\0", chemail[20] = "\0", chaddress[30] = "\0";

    printf(CYAN"\n---------Choose from list to search---------\n"RESET);
    printf(YELLOW"1. Name\n2. Mobile No.\n3. Email\n4. Address\n"RESET);
    printf(MAGENTA"Enter your choice : "RESET);
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf(MAGENTA"Enter Name: "RESET);
        scanf(" %[^\n]", chname);
        break;
    case 2:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 3:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    case 4:
        printf(MAGENTA"Enter Address: "RESET);
        scanf(" %[^\n]", chaddress);
        break;
    default:
        printf(RED"Error : --- Invalid choice ---\n"RESET);
        return;
    }

    FILE *fptr = fopen("data.csv", "r");
    fscanf(fptr, "%d\n", &count);
    int found = 0;

    if (choice == 1 || choice == 4)
    {
        int samename = 0, sameadd = 0;
        for (int i = 0; i < count; i++)
        {
            fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
            if ((strcmp(chname, details.name) == 0))
            {
                samename++;
                if (samename > 1)
                {
                    printf(RED"--- Multiple users found with same name ---\n\n"RESET);
                    goto skip;
                }
            }
            if ((strcmp(chaddress, details.address) == 0))
            {
                sameadd++;
                if (sameadd > 1)
                {
                    printf(RED"--- Multiple users found with same address ---\n\n"RESET);
                    goto skip;
                }
            }
        }

        if (samename == 1 || sameadd == 1)
        {
            goto search;
        }
        else if (samename == 0 && sameadd == 0)
        {
            goto notfound;
        }
    }
    else
        goto search;

skip:

    rewind(fptr);
    fscanf(fptr, "%d\n", &count);
    for (int i = 0; i < count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
        if ((strcmp(chaddress, details.address) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
    }
    chname[0] = '\0';
    chaddress[0] = '\0';

    int select;
    printf(CYAN"\n--- choose to search ---\n"RESET);
    printf(YELLOW"1. Mobile No\n2. Email\n"RESET);
    printf(MAGENTA"Enter option: "RESET);
    scanf("%d", &select);
    __fpurge(stdin);

    switch (select)
    {
    case 1:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 2:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    default:
        printf(RED"Error : --- Invalid choice --\n"RESET);
        goto skip;
    }

search:

    rewind(fptr);
    fscanf(fptr, "%d\n", &count);

    for (int i = 1; i <= count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) == 0) || (strcmp(chmob, details.mob) == 0) || (strcmp(chemail, details.email) == 0) || (strcmp(chaddress, details.address) == 0))
        {
            found = 1;
            printf(CYAN"\n--------- user %d details ---------\n"RESET, i);
            printf("Name: %s\n", details.name);
            printf("Mobile No.: %s\n", details.mob);
            printf("Email: %s\n", details.email);
            printf("Address: %s\n", details.address);
            break;
        }
    }

notfound:

    if (found == 0)
        printf(RED"--- Searched details not found ---\n"RESET);

    fclose(fptr);
}


void print_details()
{
    struct data
    {
        char name[20];
        char mob[15];
        char email[30];
        char address[30];
    };

    FILE *fptr = fopen("data.csv", "r");
    fscanf(fptr, "%d\n", &count);

    struct data user[count];
    struct data ch;
    for(int i=0; i<count; i++)
    {
       fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", user[i].name, user[i].mob, user[i].email, user[i].address);
    }

    int order;
    printf(YELLOW"\n1. A to Z\n2. Z to A\n"RESET);
    printf(MAGENTA"Enter the order of printing : "RESET);
    scanf("%d", &order);
    __fpurge(stdin);

    if((order != 1) && (order != 2))
    {
        printf(RED"Error : --- Invalid choice ---\n"RESET);
        return;
    }

    if(order == 1)
    {
        for(int i=0; i<count-1; i++)
        {
            for(int j=0; j<count-i-1; j++)
            {
                if(strcmp(user[j].name, user[j+1].name) == 0)
                {
                    if(strcmp(user[j].email, user[j+1].email) > 0)
                    {
                        ch = user[j];
                        user[j] = user[j+1];
                        user[j+1] = ch;
                    }
                }
                else if(strcmp(user[j].name, user[j+1].name) > 0)
                {
                    ch = user[j];
                    user[j] = user[j+1];
                    user[j+1] = ch;
                }
            }
        }
    }
    else
    {
        for(int i=0; i<count-1; i++)
        {
            for(int j=0; j<count-i-1; j++)
            {
                if(strcmp(user[j].name, user[j+1].name) == 0)
                {
                    if(strcmp(user[j].email, user[j+1].email) < 0)
                    {
                        ch = user[j];
                        user[j] = user[j+1];
                        user[j+1] = ch;
                    }
                }
                else if(strcmp(user[j].name, user[j+1].name) < 0)
                {
                    ch = user[j];
                    user[j] = user[j+1];
                    user[j+1] = ch;
                }
            }
        }
    }


    for(int k=0; k<count; k++)
    {
        printf(CYAN"\n----- user %d details -----\n"RESET,k+1);
        printf("Name : %s\nMobile No.: %s\nEmail : %s\nAddress : %s\n",user[k].name, user[k].mob, user[k].email, user[k].address);
    }
    
}


void edit_details()
{

    // int count;
    int choice;
    char chname[20] = "\0", chmob[11] = "\0", chemail[20] = "\0", chaddress[30] = "\0";

    printf(CYAN"\n--------- Choose to check data available in file ---------\n"RESET);
    printf(YELLOW"1. Name\n2. Mobile No.\n3. Email\n4. Address\n"RESET);
    printf(MAGENTA"Enter your choice : "RESET);
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf(MAGENTA"Enter Name: "RESET);
        scanf(" %[^\n]", chname);
        break;
    case 2:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 3:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    case 4:
        printf(MAGENTA"Enter Address: "RESET);
        scanf(" %[^\n]", chaddress);
        break;
    default:
        printf(RED"Error : --- Invalid choice ---\n"RESET);
        return;
    }

    int found = 0;

    FILE *fptr = fopen("data.csv", "r");
    FILE *fptr1 = fopen("temp.csv", "w");

    fscanf(fptr, "%d\n", &count);
    fprintf(fptr1, "%d\n", count);

    if (choice == 1 || choice == 4)
    {
        int samename = 0, sameadd = 0;
        for (int i = 0; i < count; i++)
        {
            fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
            if ((strcmp(chname, details.name) == 0))
            {
                samename++;
                if (samename > 1)
                {
                    printf(RED"--- Multiple users found with same name ---\n\n"RESET);
                    goto skip;
                }
            }
            if ((strcmp(chaddress, details.address) == 0))
            {
                sameadd++;
                if (sameadd > 1)
                {
                    printf(RED"--- Multiple users found with same address ---\n\n"RESET);
                    goto skip;
                }
            }
        }

        if (samename == 1 || sameadd == 1)
        {
            goto edit;
        }
        else if (samename == 0 && sameadd == 0)
        {
            goto notfound;
        }
    }
    else
        goto edit;

skip:

    rewind(fptr);
    fscanf(fptr, "%d\n", &count);
    for (int i = 0; i < count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
        if ((strcmp(chaddress, details.address) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
    }
    chname[0] = '\0';
    chaddress[0] = '\0';

    int select;
    printf(CYAN"--- Choose from list ---\n"RESET);
    printf(YELLOW"1. Mobile No\n2. Email\n"RESET);
    printf(MAGENTA"Enter option: "RESET);
    scanf("%d", &select);
    __fpurge(stdin);

    switch (select)
    {
    case 1:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 2:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    default:
        printf(RED"Error : --- Invalid choice ---\n"RESET);
        goto skip;
    }

edit:

    rewind(fptr);
    fscanf(fptr, "%d\n", &count);

    for (int i = 1; i <= count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) == 0) || (strcmp(chmob, details.mob) == 0) || (strcmp(chemail, details.email) == 0) || (strcmp(chaddress, details.address) == 0))
        {
            found = 1;
            int choice;
            char ch;
            printf(MAGENTA"--- Confirm to edit %s's data(y/n) ? : "RESET, details.name);
            scanf(" %c", &ch);

            if (ch == 'y' || ch == 'Y')
            {

                printf(CYAN"\n---- Choose to edit ----\n"RESET);
                printf(YELLOW"1. Name\n2. Mobile No.\n3. Email\n4. Address\n"RESET);
                printf(MAGENTA"Enter your choice: "RESET);
                scanf("%d", &choice);

                if (choice == 1)
                {
                    printf(MAGENTA"Name: "RESET);
                    scanf(" %[^\n]", details.name);
                    printf(GREEN"--- Name edited successfully ---\n"RESET);
                }
                else if (choice == 2)
                {
                    int repeat = 1, len, flag;
                    while (repeat)
                    {
                        printf(MAGENTA"Mobile No.: "RESET);
                        scanf(" %[^\n]", details.mob);
                        len = strlen(details.mob);

                        if (len != 10)
                        {
                            printf(RED"Error : --- Invalid. Enter 10 digit mobile number(0-9) ---\n"RESET);
                            continue;
                        }

                        flag = 0; // to check mobile number has digits only
                        for (int i = 0; i < len; i++)
                        {
                            if (!(isdigit(details.mob[i])))
                            {
                                printf(RED"--- Invalid. Enter 10 digit mobile number(0-9) ---\n"RESET);
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0)
                            repeat = 0;
                    }

                    printf(GREEN"--- Mobile no. edited successfully ---\n"RESET);
                }
                else if (choice == 3)
                {
                    int repeat = 1, len, flag = 0; // to check email is valid or not
                    while (repeat)
                    {
                        printf(MAGENTA"Email: "RESET);
                        scanf(" %[^\n]", details.email);

                        len = strlen(details.email);
                        if (len < 11)
                        {
                            printf(RED"Error : --- Invalid. Email shoud be more than 10 characters ---//\n"RESET);
                            continue;
                        }

                        flag = 0;
                        int i = 0;
                        char gmail[5] = ".com";
                        char str[15];
                        for (i = 0; i < 4; i++)
                        {
                            str[i] = details.email[len - 4 + i]; // fetching last 10 characters of email
                        }
                        str[i] = '\0';

                        if (strcmp(gmail, str) != 0)
                        {
                            printf(RED"Error : --- Invalid. Missing '.com' at end ---\n"RESET);
                            continue;
                        }

                        int alpha = 0, digit = 0, spchar=0, space = 0;
                        for (i = 0; i < len - 4; i++)
                        {
                            if (details.email[i] == ' ' || details.email[i] == '\t')
                            {
                                space = 1;
                                break;
                            }
                            if (isalpha(details.email[i]))
                                alpha = 1;
                            else if (isdigit(details.email[i]))
                                digit = 1;
                            else if(details.email[i] == '@')
                                spchar = 1;
                        }

                        if (space == 1)
                        {
                            printf(RED"Error : --- Space not allowed ---\n"RESET);
                            continue;
                        }
                        if (!((alpha == 1) && (digit == 1) && (spchar == 1)))
                        {
                            printf(RED"Error : --- Invalid. Missing character or digit or special character ---\n"RESET);
                            continue;
                        }

                        repeat = 0;
                    }

                    printf(GREEN"--- Email edited successfully ---\n"RESET);
                }
                else if (choice == 4)
                {
                    printf(MAGENTA"Address: "RESET);
                    scanf(" %[^\n]", details.address);
                    printf(GREEN"--- Address edited successfully ---\n"RESET);
                }
                else
                {
                    printf(RED"Error : --- Invalid choice ---\n"RESET);
                    return;
                }
            }
        }
        fprintf(fptr1, "%s,%s,%s,%s\n", details.name, details.mob, details.email, details.address);
    }

notfound:

    if (found == 0)
    {
        printf(RED"----- User data not found -----\n"RESET);
        return;
    }

    fclose(fptr);
    fclose(fptr1);

    fptr = fopen("data.csv", "w");
    fptr1 = fopen("temp.csv", "r");

    copying(fptr, fptr1);
    fclose(fptr);
    fclose(fptr1);
}

void delete_details()
{
    // int count;
    char chname[20] = "\0", chmob[11] = "\0", chemail[20] = "\0", chaddress[20] = "\0";
    int choice;

    printf(CYAN"\n---------Choose to delete---------\n"RESET);
    printf(YELLOW"1. Name\n2. Mobile No.\n3. Email\n4. Address\n"RESET);
    printf(MAGENTA"Enter your choice : "RESET);
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf(MAGENTA"Enter Name: "RESET);
        scanf(" %[^\n]", chname);
        break;
    case 2:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 3:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    case 4:
        printf(MAGENTA"Enter Address: "RESET);
        scanf(" %[^\n]", chaddress);
        break;
    default:
        printf(RED"Error : --- Invalid choice ---\n"RESET);
        return;
    }

    int found = 0;

    FILE *fptr = fopen("data.csv", "r");
    fscanf(fptr, "%d\n", &count);

    if (choice == 1 || choice == 4)
    {
        int samename = 0, sameadd = 0;
        for (int i = 0; i < count; i++)
        {
            fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
            if ((strcmp(chname, details.name) == 0))
            {
                samename++;
                if (samename > 1)
                {
                    printf(RED"--- Multiple users found with same name ---\n\n"RESET);
                    goto skip;
                }
            }
            if ((strcmp(chaddress, details.address) == 0))
            {
                sameadd++;
                if (sameadd > 1)
                {
                    printf(RED"--- Multiple users found with same address ---\n\n"RESET);
                    goto skip;
                }
            }
        }

        if (samename == 1 || sameadd == 1)
        {
            goto delete;
        }
        else if (samename == 0 && sameadd == 0)
        {
            goto notfound;
        }
    }
    else
        goto delete;

skip:

    rewind(fptr);
    fscanf(fptr, "%d\n", &count);
    for (int i = 0; i < count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
        if ((strcmp(chaddress, details.address) == 0))
        {
            printf("%d. %s - %s - %s - %s\n", i + 1, details.name, details.mob, details.email, details.address);
        }
    }
    chname[0] = '\0';
    chaddress[0] = '\0';

    int select;
    printf(CYAN"--- choose to delete ---\n"RESET);
    printf(YELLOW"1. Mobile No\n2. Email\n"RESET);
    printf(MAGENTA"Enter option: "RESET);
    scanf("%d", &select);
    __fpurge(stdin);

    switch (select)
    {
    case 1:
        printf(MAGENTA"Enter Mobile No.: "RESET);
        scanf(" %[^\n]", chmob);
        break;
    case 2:
        printf(MAGENTA"Enter Email: "RESET);
        scanf(" %[^\n]", chemail);
        break;
    default:
        printf(MAGENTA"--- Invalid choice ---\n"RESET);
        goto skip;
    }

    delete :

    rewind(fptr);
    FILE *fptr1 = fopen("temp.csv", "w");
    fscanf(fptr, "%d\n", &count);
    fprintf(fptr1, "%d\n", count - 1);

    for (int i = 1; i <= count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", details.name, details.mob, details.email, details.address);
        if ((strcmp(chname, details.name) != 0) && (strcmp(chmob, details.mob) != 0) && (strcmp(chemail, details.email) != 0) && (strcmp(chaddress, details.address) != 0))
        {
            fprintf(fptr1, "%s,%s,%s,%s\n", details.name, details.mob, details.email, details.address);
        }
        else
        {
            found = 1;
            char chdel;
            printf(MAGENTA"Confirm to delete %s's data(y/n) ? : "RESET, details.name);
            scanf(" %c", &chdel);
            if ((chdel == 'y') || (chdel == 'Y'))
            {
                printf(GREEN"---** %s's data deleted successfully **---\n"RESET, details.name);
            }
            else
                return;
        }
    }

notfound:

    if (found == 0)
    {
        printf(RED"--- User data not found ---\n"RESET);
        return;
    }

    fclose(fptr);
    fclose(fptr1);

    fptr = fopen("data.csv", "w");
    fptr1 = fopen("temp.csv", "r");

    copying(fptr, fptr1);
    fclose(fptr);
    fclose(fptr1);
}

int samedata()
{
    char chname[20] = "\0", chmob[11] = "\0", chemail[20] = "\0", chaddress[20] = "\0";

    FILE *fptr = fopen("data.csv", "r");

    int count;
    fscanf(fptr, "%d\n", &count);

    for (int i = 0; i < count; i++)
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", chname, chmob, chemail, chaddress);
        if (strcmp(chmob, details.mob) == 0)
        {
            return 1;
        }
        else if (strcmp(chemail, details.email) == 0)
        {
            return 2;
        }
    }

    return 0;
}


// copying all data from temporary file(temp.csv) to original file(data.csv)
void copying(FILE *fptr, FILE *fptr1)
{
    char ch;
    while ((ch = fgetc(fptr1)) != EOF)
    {
        fputc(ch, fptr);
    }

    return;
}