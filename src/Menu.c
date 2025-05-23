#include <termios.h>
#include "header.h"

void checkAcount(struct User *u)
{
    FILE *pf = fopen("../data/records.txt", "r");
    struct Record r;
    // system("clear");
    int nbr;
    char Tname[1024];
    int len;
    int stop = 0;
    int i = 0;
    bool found = 0;
    //
    int clear = 0;
    system("clear");
    while (!stop)
    {
        if (found == 1)
        {
            system("clear");
            printf("\n\n\t\t\t======= Not Found This Account =======\n");
        }
        printf("\n\n\t\t\t======= ATM =======\n");
        printf("\t\t\t===== Check Account =====\n");
        printf("\n\n\t\t\t\"Entre -1 At first Paramter if u want skipp and return to Menu !!\"");
        printf("\n\n\n\t\t\tMr. %s Please Chose In Wich Compte u Want: ", u->name);
        // I GET a PROBLEM TO NOT READ FROM STDIN AND I USE GETCHAR TO CLEAR THE BUFFER

        fgets(Tname, 1024, stdin);
        Tname[strcspn(Tname, "\n")] = '\0';
        int skipp = atoi(Tname);
        if (skipp == -1)
        {
            mainMenu(u);
        }
        while (1)
        {
            len = strlen(Tname);
            if (!ISNAME(Tname) || len == 0 || len > 9)
            {
                printf("U are put limit or empty please try again with valid character : ");
                if (fgets(Tname, 1024, stdin) != NULL)
                {
                    Tname[strcspn(Tname, "\n")] = '\0';
                }
                continue;
            }
            else
            {
                break;
            }
        }

        nbr = atoi(Tname);
        fseek(pf, 0, SEEK_SET);
        while (getAccountFromFile(pf, &r))
        {
            if (nbr == r.accountNbr && strcmp(r.name, u->name) == 0)
            {
                yearandrate Rate = calculaterate(r.amount, r.accountType, r.deposit.year);

                system("clear");
                printf("Mr. %s this is your account id : %d\n", u->name, r.accountNbr);
                printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                       r.accountNbr,
                       r.deposit.month,
                       r.deposit.day,
                       r.deposit.year,
                       r.country,
                       r.phone,
                       r.amount,
                       r.accountType);
                if (strcmp(r.accountType, "current") == 0)
                {
                    printf("\n\n\t====> You will not get interests because the account is of type current . <====\n\n");
                    i = 1;
                    stop = 1;
                    break;
                }
                if (strcmp(r.accountType, "saving") == 0)
                {
                    printf("\n\n\t<== You will get %.2f as interest on day %d of every month ==>\n\n", Rate.rate, r.deposit.day);
                    i = 1;
                    stop = 1;
                    break;
                }
                printf("\n\n\t<== You will get %.2f of interest on %d/%d/%d ==>\n\n", Rate.rate, r.deposit.month, r.deposit.day, Rate.year);
                i = 1;
                stop = 1;
                break;
            }
            found = 1;
        }
    }
    fclose(pf);
    stayOrReturn(i, checkAcount, u);
}


void updateacount(struct User *u)
{
    struct Record r;
    char option[1024];
    char accountNbr[1024];
    int found = 0;
    char country[1024];
    char phone[1024];
    char op[1024];
    int clear = 0;
    system("clear");
    printf("\n\n\t\t   ======= ATM =======\n");
    printf("\n\t\t\"Entre -1 At first Parameter if u want skipp and return to Menu !!\"");
    printf("\n\n\t\t   ======= %s =======\n", u->name);
    printf("\n\n\t\t=======  Update account =======\n");
    printf("\n\t\tEnter the account number: ");
    fgets(accountNbr, sizeof(accountNbr), stdin);
    accountNbr[strcspn(accountNbr, "\n")] = '\0';
    int skipp = atoi(accountNbr);
    if (skipp == -1)
    {
        mainMenu(u);
    }
    validprompt(accountNbr, "N");
    int nbr = atoi(accountNbr);
    int Ex = Checkifexist(nbr, u->name);
    if (Ex == 0)
    {
        stayOrReturn(0, updateacount, u);
    }
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Country\n");
    printf("\n\t\t[2]- Phone\n");
    printf("\nEnter your choice: ");
    fgets(option, sizeof(option), stdin);
    option[strcspn(option, "\n")] = '\0';
    int tt;

    while (1)
    {
        int i = strlen(option);
        tt = atoi(option);

        if ((tt != 1 && tt != 2) || i > 1)
        {
            printf("Try again, input must be 1 or 2: ");
            if (fgets(option, sizeof(option), stdin) != NULL)
            {
                option[strcspn(option, "\n")] = '\0';
            }
            i = strlen(option);
            continue;
        }
        else
        {
            break;
        }
    }
    FILE *pf = fopen("../data/records.txt", "r");
    FILE *temp = fopen("../data/temp.txt", "w");
    while (getAccountFromFile(pf, &r))
    {
        if (r.accountNbr == nbr && strcmp(r.name, u->name) == 0)
        {
            found = 1;
            switch (tt)
            {
            case 1:
                printf("Enter new country: ");
                fgets(country, sizeof(country), stdin);
                country[strcspn(country, "\n")] = '\0';
                validprompt(country, "C");
                int len = strlen(country);
                // Then copy the new data including the null terminator
                strcpy(r.country, country);
                break;
            case 2:
                printf("Enter new phone number: ");
                getchar();
                if (fgets(phone, sizeof(phone), stdin) != NULL)
                {
                    phone[strcspn(phone, "\n")] = '\0';
                }
                validprompt(phone, "P");
                r.phone = atoi(phone);
                break;
            default:
                printf("Invalid option.\n");
                fclose(pf);
                fclose(temp);
                return;
            }
        }

        // her i write to the tempo file !!
        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(pf);
    fclose(temp);

    if (found)
    {
        remove("../data/records.txt");
        rename("../data/temp.txt", "../data/records.txt");
        printf("Account updated successfully!\n");
    }
    else
    {
        printf("Account number not found.\n");
        remove("../data/temp.txt");
    }
    stayOrReturn(found, updateacount, u);
}

void removeacount(struct User *u)
{
    struct Record r;
    char accountNbr[1024];
    int found = 0;
    FILE *pf = fopen("../data/records.txt", "r");
    FILE *temp = fopen("../data/temp.txt", "w");

    system("clear");
    printf("\t\t\t===== Remove account =====\n");
    printf("\t\t\t===== Mr.%s =====\n", u->name);
    printf("\nEnter the account number:");
    fgets(accountNbr, sizeof(accountNbr), stdin);
    accountNbr[strcspn(accountNbr, "\n")] = '\0';
    validprompt(accountNbr, "N");
    int Nbr = atoi(accountNbr);
    while (getAccountFromFile(pf, &r))
    {
        if (strcmp(r.name, u->name) == 0 && r.accountNbr == Nbr)
        {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }
    if (found)
    {
        fclose(pf);
        fclose(temp);
        remove("../data/records.txt");
        rename("../data/temp.txt", "../data/records.txt");
        printf("Account removed successfully!\n");
    }
    else
    {
        fclose(pf);
        fclose(temp);
        remove("../data/temp.txt");
        printf("Account number not found.\n");
    }
    stayOrReturn(found, removeacount, u);
}


void transaction(struct User *u)
{
    char accnbr[1024];
    char option[1024];
    struct Record r;
    int found = 0;
    char take[1024];
    char add[1024];
    system("clear");
    printf("\n\n\t\t   ======= ATM =======\n");
    printf("\n\n\t\t======= Make Transaction .=======\n");
    printf("\n\n\t\t======= Mr. %s .=======\n", u->name);
    printf("\n\t\tEnter the account number: ");
    fgets(accnbr, 1024, stdin);
    accnbr[strcspn(accnbr, "\n")] = '\0';
    validprompt(accnbr, "N");
    int nbr = atoi(accnbr);
    ///  - - - - - - -  > > > > !
    FILE *pf = fopen("../data/records.txt", "r");
    FILE *temp = fopen("../data/temp.txt", "w");
    fseek(pf, 0, SEEK_SET);
    bool found1 = false;

    while (getAccountFromFile(pf, &r))
    {
        if (r.accountNbr == nbr && strcmp(r.name, u->name) == 0)
        {

            if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
            {
                system("clear");
                printf("\n\n\t <===== YOU CAN'T DO THIS TRANSACTION ON THIS ACCOUNT =====>!\n\n");
                printf("\t<== After 5 seconde u will see message of not found and choose in option ==> ...!\n");
                printf("\n\n\t\t\t\t\t\t Thank you Mr. %s !!\n", u->name);
                sleep(5);
                found1 = true;
                found = 0;
                break;
            }
            found = 1;
            printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
            printf("\n\t\t[1]- deposit\n");
            printf("\n\t\t[2]- withdraw\n");
            printf("Enter your choice: ");
            fgets(option, sizeof(option), stdin);
            option[strcspn(option, "\n")] = '\0';
            int tt;
            while (1)
            {
                int i = strlen(option);
                tt = atoi(option);

                if ((tt != 1 && tt != 2) || i > 1)
                {
                    printf("Try again, input must be 1 or 2: ");
                    if (fgets(option, sizeof(option), stdin) != NULL)
                    {
                        option[strcspn(option, "\n")] = '\0';
                    }
                    i = strlen(option);
                    continue;
                }
                else
                {
                    break;
                }
            }
            switch (tt)
            {
            case 1:
                printf("Enter amount u want to add to account : ");
                fgets(add, sizeof(add), stdin);
                add[strcspn(add, "\n")] = '\0';
                validprompt(add, "D");
                // Then copy the new data including the null terminator
                double a = strtod(add, NULL);
                r.amount = a + r.amount;
                printf("U add %lf $ to ur account successfully !\n", a);
                break;
            case 2:
                printf("Entre amount u want to take from acount $$ :");
                fgets(take, sizeof(take), stdin);
                take[strcspn(take, "\n")] = '\0';
                validprompt(take, "D");
                double t = strtod(take, NULL);
                if (t > r.amount)
                {
                    system("clear");
                    printf("\n\n\t\t\t<<== YOU DON'T HAVE THIS AMOUNT in YOUR ACCOUNT ! << \n");
                    printf("\t<== After 5 seconde u will see message of not found and choose in option ==> ...!\n");
                    printf("\n\n\t\t\t\t\t\t Thank you Mr. %s !!\n", u->name);
                    found = 0;

                    sleep(5);
                }
                else
                {
                    r.amount = r.amount - t;
                    printf("U take %lf $ from account successfully !\n", t);
                }
                break;
            default:
                printf("Invalid option.\n");
                fclose(pf);
                fclose(temp);
                return;
            }
        }

        // her i write to the tempo file !!
        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(pf);
    fclose(temp);

    if (found)
    {
        remove("../data/records.txt");
        rename("../data/temp.txt", "../data/records.txt");
        printf("Account updated successfully!\n");
    }
    else
    {

        remove("../data/temp.txt");

        stayOrReturn(found, transaction, u);
    }
    success(u);
}