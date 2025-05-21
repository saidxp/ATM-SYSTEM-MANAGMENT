#include "header.h"

char *RECORDS = "../data/records.txt";

int getNameFromFile(FILE *ptr, struct Temp *us)
{
    return fscanf(ptr, "%d %s %s", &us->id, us->name, us->password) != EOF;
}
int getAccountFromFile(FILE *ptr, struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  r->name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User *u, struct Record *r)
{
    int id = getUserId(u->name);
    if (id == -1)
    {
        printf("User id not found!\n");
        return;
    }

    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r->id,
            id,
            u->name,
            r->accountNbr,
            r->deposit.month,
            r->deposit.day,
            r->deposit.year,
            r->country,
            r->phone,
            r->amount,
            r->accountType);
}

void stayOrReturn(int notGood, void f(struct User *u), struct User *u)
{

    int option;
    char Tname[1024];
    int len;
    int r = 0;
    int j = 0;

    if (notGood == 0)
    {
        while (!r)
        {
            system("clear");
            printf("\n✖ Record not found (!! < not exist > !!)\n");
            printf("\nEnter 0 or any special Character to try again, 1 to return to main menu and 2 to exit: ");
            fflush(stdin);
            fgets(Tname, 1024, stdin);
            Tname[strcspn(Tname, "\n")] = '\0';

            len = strlen(Tname);
            option = atoi(Tname);
            if (len == 1 && option >= 0 && option <= 2)
            {
                system("clear");
                break; // ->>> !
            }
            else
            {
                printf("Insert a valid operation!\n");
            }
        }

        if (option == 0)
        {
            f(u);
        }
        else if (option == 1)
        {
            mainMenu(u);
        }
        else if (option == 2)
        {
            exit(0);
        }
    }
    else
    {

        printf("\nEnter 1 to go to the main menu and 0 or (special) to exit: ");
        int i = 0;
        while (1)
        {
            if (i > 0)
            {
                // in
                printf("Entre 1 to go to the main menu any press 0 or Any special char to go to initmenu: ");
            }
            i++;
            if (fgets(Tname, 1024, stdin) != NULL)
            {
                Tname[strcspn(Tname, "\n")] = '\0';
            }

            len = strlen(Tname);
            option = atoi(Tname);
            if (len == 1 && (option == 0 || option == 1))
            {
                break;
            }
            else
            {
                printf("Invalid input! Try again : \n");
            }
        }
        if (option == 1)
        {
            system("clear");
            mainMenu(u);
        }
        else
        {
            system("clear");
            initMenu(u);
        }
    }
}

void success(struct User *u)
{
    char Option[1024];
    printf("\n✔ Success!\n\n");
    printf("Enter 1 to go to the main menu and 0 to exit!: ");
    int op;

    while (1)
    {
        if (fgets(Option, sizeof(Option), stdin) != NULL)
        {
            Option[strcspn(Option, "\n")] = '\0';  
        }

        if (strlen(Option) == 1 && (Option[0] == '0' || Option[0] == '1'))
        {
            op = atoi(Option);
            break;
        }
        else
        {
            printf("Invalid input! Please enter 1 for main menu or 0 to exit: ");
        }
    }

    if (op == 1)
    {
        mainMenu(u);
    }
    else if (op == 0)
    {
        initMenu(u);
    }
}

void createNewAcc(struct User *u)
{
    struct Record r;
    struct Record cr;
    FILE *pf = fopen(RECORDS, "a+");
    char day[1024];
    char Tnumber[1024];
    char country[1024];
    char phone[1024];
    char Amount[1024];
    char Type[1024];

    system("clear");

noAccount:
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy): ");
    fgets(day, 1024, stdin);
    day[strcspn(day, "\n")] = '\0';
    /// CHECK THE DATE FORMAT
    CheCkday(day, &r);
    printf("The date is: %s\n", day);
    printf("\nEnter number of Acount: ");
    // clear buffer from stdin file
    if (fgets(Tnumber, 1024, stdin) != NULL)
    {
        // ----- >  -----<< ----
        Tnumber[strcspn(Tnumber, "\n")] = '\0';
    }
    // CHECK THE NUMBER OF ACCOUNT
    validprompt(Tnumber, "N");
    r.accountNbr = atoi(Tnumber);
    int count = 0;
    fseek(pf, 0, SEEK_SET);
    while (getAccountFromFile(pf, &cr))
    {
        if (strcmp(cr.name, u->name) == 0 && cr.accountNbr == r.accountNbr)
        {
            system("clear");
            printf("✖ This Account already exists for this user\n\n");
            printf("<<Please try again with another number !!>\n\n");
            goto noAccount;
        }
        count++;
    }
    /// her i will after i check a valid date i need to put it in the struct
    if (count == 0)
    {
        r.id = 0;
    }
    else
    {
        r.id = cr.id + 1;
    }
    // printf("--- >>> %d", count);
    //  r.id = cr.id + 1;
    printf("\nEnter the country: ");
    fgets(country, sizeof(country), stdin);
    // ----- >  -----<< ----
    country[strcspn(country, "\n")] = '\0';
    validprompt(country, "C");
    strcpy(r.country, country);
    printf("\nEnter the phone number: ");
    fgets(phone, sizeof(phone), stdin);
    // ----- >  -----<< ----
    phone[strcspn(phone, "\n")] = '\0';
    validprompt(phone, "P");
    r.phone = atoi(phone);
    // ------------------------------ > ||
    // problem from read stdin get suck at this instruction !
    //
    printf("\nEnter amount to deposit $: ");
    // hehehe
    fgets(Amount, 1024, stdin);
    // hehhehe
    Amount[strcspn(Amount, "\n")] = '\0';
    // (----- >>> Problem of stdin file !<<<-----)
    validprompt(Amount, "D");
    // (----- >>>convert <<<----)
    r.amount = strtod(Amount, NULL);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    fgets(Type, 1024, stdin);
    Type[strcspn(Type, "\n")] = '\0';
    validprompt(Type, "T");
    strcpy(r.accountType, Type);
    saveAccountToFile(pf, u, &r);
    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User *u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");
    int i = 0;
    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u->name);
    while (getAccountFromFile(pf, &r))
    {
        if (strcmp(r.name, u->name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
                   i++;
        }
         
    }
    if (i== 0)
        {
            printf("u are still don't have any account !!\n");
        }
    fclose(pf);
    success(u);
}
