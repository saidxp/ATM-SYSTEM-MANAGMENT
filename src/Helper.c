#include "header.h"

yearandrate calculaterate(double Amount, char *Type, int year)
{

    struct RateData rateData;
    
    if (strcmp(Type, "saving") == 0)
    {
        double tempo = Amount * 7;
        double rate = tempo / 100;
        double t = rate / 12;
        rateData.rate = t;
        return rateData;
    }

    if (strcmp(Type, "fixed01") == 0)
    {
        double tempo = Amount * 4;
        double rate = tempo / 100;
        double t = rate ;
        year += 1;
        rateData.year = year;
        rateData.rate = t;
        return rateData;
    }

    if (strcmp(Type, "fixed02") == 0)
    {
        double i = 2.00;
        double tempo = Amount * 5;
        double rate = tempo / 100;
        double t = rate * i;
        year += 2;
        rateData.year = year;
        rateData.rate = t;
        return rateData;
    }
    if (strcmp(Type, "fixed03") == 0)
    {   
        double i = 3.00;
        double tempo = Amount * 8;
        double rate = tempo / 100;
        double t = rate * i;
        year += 3;
        rateData.year = year;
        rateData.rate = t;
        return rateData;
    }
}

void transferownership(struct User *u)
{

    struct Record r;
    struct User tempo;

    char accountNb[1024];
    char newowner[1024];
    int found = 0;
    FILE *pf = fopen("../data/records.txt", "r");
    FILE *temp = fopen("../data/temp.txt", "w");

    system("clear");
    printf("\t\t\t===== Transfer ownership =====\n");
    printf("\t\t\t\t====  %s =====\n", u->name);
    printf("\nEnter the account number: ");
    fgets(accountNb, sizeof(accountNb), stdin);
    accountNb[strcspn(accountNb, "\n")] = '\0';
    validprompt(accountNb, "N");
    int Ex = Checkifexist(atoi(accountNb), u->name);
    if (Ex == 0)
    {
        stayOrReturn(0,  transferownership, u);
    }
    printf("\nEnter the new owner name: ");
    fgets(newowner, sizeof(newowner), stdin);
    newowner[strcspn(newowner,"\n")] = '\0';
    int i =0;
    int not = 0;
    int l = strlen(newowner);
    while(newowner[i] != '\0')
    {
        if (!isalpha(newowner[i]))
        {
           not = 1;
            
        }
        i++;
    }
    if (not == 1)
    {
        printf("\n\n <<=== U are entre invalid character must be alphabetic only !! ===>>\n");
        printf("\n\n <<==== after 5 seconde u will see Recorde Not found! ====>> \n");

        sleep(5);
        stayOrReturn(0,transferownership,u);
    }
    int Nbr = atoi(accountNb);
    int userid = getUserId(newowner);
    if (userid == -1)
    {
        stayOrReturn(0, transferownership, u);
    }
    // user Alice > 123
    
    fseek(pf, 0, SEEK_SET);
    while (getAccountFromFile(pf, &r))
    {
        if (r.accountNbr == Nbr && strcmp(r.name,u->name) == 0) 
        {
            found = 1;
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                userid,
                newowner,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
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
    fclose(pf);
    fclose(temp);
    if (found)
    {
        remove("../data/records.txt");
        rename("../data/temp.txt", "../data/records.txt");
        printf("Account it's Transfer successfully!\n");
    }
    else
    {
        stayOrReturn(found, transferownership, u);
        printf("Account number not found.\n");
        remove("../data/temp.txt");
    }
   success(u);
}
// herlper for valid from user input


int isValidFormat(const char *date) {
   
    if (strlen(date) != 10) 
    return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '/') 
            return 0;
        } else {
            if (!isdigit(date[i])) 
            return 0;
        }
    }
    return 1;
}

int Checkifexist(int nbr, char *name)
{
    struct Record r;
    FILE *pf = fopen("../data/records.txt", "r");
    fseek(pf, 0, SEEK_SET);
    while (getAccountFromFile(pf, &r))
    {
        if (r.accountNbr == nbr && strcmp(r.name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

bool checkvalidamount(char *str)
{
     
    if (str[0] == '.')
    {
        return 0;
    }
    int i = 0;
    int detect = 0;
    while (str[i] != '\0')
    {
        if ((str[i] >= '0' && str[i] <= '9'))
        {
            ;
        }
        else if (str[i] == '.')
        {
            detect++;
        }
        else
        {
            return false;
        }
        if (detect > 1)
        {
            return false;
        }
        i++;
    }
    return true;
}