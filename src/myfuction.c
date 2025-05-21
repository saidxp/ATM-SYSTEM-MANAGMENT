#include <termios.h>
#include "header.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>
// const char *RECORDS = "../data/records.txt";

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

bool signUp(struct User *u)
{
    struct termios oflags, nflags;
    // struct Record r;
    struct Temp us;
    int len;
    FILE *pf = fopen("../data/users.txt", "a+");
    system("clear");
    printf("\n\n\t\t   ======= ATM =======\n");
    printf("\n\n\t\t=======  register =======\n");
    printf("\n\t\tEntre your Login: ");

    // read name with buffer overflow check
    if (fgets(us.name, sizeof(us.name), stdin) == NULL)
    {
        printf("\nError reading input. Please try again.");
        fclose(pf);
        return false;
    }

    // check for buffer overflow in name
    if (strchr(us.name, '\n') == NULL)
    {
        // input was too long - clear input buffer
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        printf("\nUsername too long. Maximum length is %d characters.", (int)sizeof(us.name) - 1);
        fclose(pf);
        return false;
    }

    us.name[strcspn(us.name, "\n")] = '\0';
    int i = 0;
    int is = 0;

    while (us.name[i] != '\0')
    {
        if (!isalpha(us.name[i]))
        {
            printf("U are entre invalid character must be alphabetic only !!");
            fclose(pf);
            return false;
        }
        i++;
    }

    len = strlen(us.name);
    if ((len == 0) || (len > 14))
    {
        printf("U are put limit or empty please try again with valid character !!");
        fclose(pf);
        return false;
    }

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        fclose(pf);
        exit(1);
    }

    printf("\n\t\tPassword :");

    // read password with buffer overflow check
    if (fgets(us.password, sizeof(us.password), stdin) == NULL)
    {
        printf("\nError reading password. Please try again.\n");
        // restore terminal settings
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
        }
        fclose(pf);
        return false;
    }

    // check for buffer overflow in password
    if (strchr(us.password, '\n') == NULL)
    {
        // input was too long - clear input buffer
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        printf("\nPassword too long. Maximum length is %d characters.", (int)sizeof(us.password) - 1);
        // restore terminal settings
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
        }
        fclose(pf);
        return false;
    }

    us.password[strcspn(us.password, "\n")] = '\0';
    len = strlen(us.password);
    if ((len == 0) || (len > 14))
    {
        printf("U are put limit or empty !!");
        tcsetattr(fileno(stdin), TCSANOW, &oflags);
        fclose(pf);
        return false;
    }

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        fclose(pf);
        exit(1);
    }

    if (CheCk(us.name))
    {
        printf("\n\n\t\t\tThere Is Diplacate this user already exist");
        fclose(pf);
        return false;
    }
    else
    {
        // Use safer strncpy instead of strcpy to prevent buffer overflow
        strncpy(u->name, us.name, sizeof(u->name) - 1);
        u->name[sizeof(u->name) - 1] = '\0'; // Ensure null termination

        strncpy(u->password, us.password, sizeof(u->password) - 1);
        u->password[sizeof(u->password) - 1] = '\0'; // Ensure null termination

        saveusersToFile(pf, u);
        fclose(pf);

        printf("%s You are registered now should i give u a menu to chosse from it >>", u->name);
        return true;
    }
}

bool CheCk(char name[50])
{
    FILE *pf = fopen("../data/users.txt", "r");
    struct Temp us; // temporary struct becs when i check i ruin my original name of user in memory !!

    while (getNameFromFile(pf, &us))
    {
        if (strcmp(name, us.name) == 0)
        {
            return true;
        }
    }
    return false;
}

void saveusersToFile(FILE *ptr, struct User *u)
{
    // fprintf(ptr, "%d %s %s\n", u->id, u->name, u->password);
    struct Temp us; // temporary struct becs when i check i ruin my original name of user in memory !!
    while (getNameFromFile(ptr, &us))
    {
    }
    fprintf(ptr, "%d %s %s\n", us.id + 1, u->name, u->password);
}

int getUserId(const char *username)
{
    FILE *f = fopen("../data/users.txt", "r");
    if (!f)
        return -1;
    struct Temp us;

    fseek(f, 0, SEEK_SET);
    while (fscanf(f, "%d %s %s", &us.id, us.name, us.password) != EOF)
    {
        if (strcmp(us.name, username) == 0)
        {
            fclose(f);
            return us.id;
        }
    }
    fclose(f);
    return -1;
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

int ISNAME(char *s)
{

    if (!s || *s == '\0')
        return 0;

    while (*s)
    {
        if (!isdigit(*s))
            return 0;
        s++;
    }
    return 1;
}

int readCleanLine(char *buf, size_t size)
{
    int i = 0;
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {

        if (c == 27)
        {

            getchar();
            getchar();
            buf[0] = '\0';
            return 0;
        }

        if (!isprint(c))
            continue;

        if (i < size - 1)
        {
            buf[i++] = (char)c;
        }
    }

    buf[i] = '\0';
    return i > 0;
}

void CheCkday(char *date, struct Record *r)
{
    int month, day, year;
    int valid = 0;
    // The month of year !!
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while (!valid)
    {

        if (!isValidFormat(date))
        {
            printf("Invalid date format. Please use mm/dd/yyyy: ");
            fgets(date, 1024, stdin);
            date[strcspn(date, "\n")] = '\0';
            continue;
        }

        if (sscanf(date, "%d/%d/%d", &month, &day, &year) != 3)
        {
            printf("Invalid date. Please enter again (mm/dd/yyyy): ");
            fgets(date, 1024, stdin);
            date[strcspn(date, "\n")] = '\0';
            continue;
        }

        if (month < 1 || month > 12)
        {
            printf("Month must be between 1 and 12. Please enter again (mm/dd/yyyy): ");
            fgets(date, 1024, stdin);
            date[strcspn(date, "\n")] = '\0';
            continue;
        }

        if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
        {
            daysInMonth[2] = 29;
        }
        if (day < 1 || day > daysInMonth[month])
        {
            printf("Invalid day for the given month. Please enter again (mm/dd/yyyy): ");
            fgets(date, 1024, stdin);
            date[strcspn(date, "\n")] = '\0';
            continue;
        }

        valid = 1;
    }
    if (valid == 1)
    {
        r->deposit.month = month;
        r->deposit.day = day;
        r->deposit.year = year;
    }
}

void validprompt(char *input, char *option)
{
    int len;
    if (strcmp(option, "N") == 0)
    {
        while (1)
        {

            len = strlen(input);

            if (len == 0 || len > 6 || !ISNAME(input))
            {
                printf("Invalid input. Please enter a valid number (up to 6 digits) : ");

                if (fgets(input, 1024, stdin) != NULL)
                {
                    input[strcspn(input, "\n")] = '\0';
                }

                continue;
            }
            else
            {
                printf("Valid account number accepted.\n");
                break;
            }
        }
    }
    if (strcmp(option, "C") == 0)
    {
        while (1)
        {
            len = strlen(input);
            int i = 0;
            struct Country c;

            FILE *pf = fopen("../data/country.txt", "r");
            while (fscanf(pf, "%49s", c.country) != EOF) // her i limit the buffer
            {
                if (strcmp(c.country, input) == 0)
                {
                    i = 1;
                }
            }

            fclose(pf);

            if (len == 0 || !isPrintableString(input) || i == 0)
            {
                printf("Invalid input. Please enter a valid country name Must be Uppercase First Letter: ");
                if (fgets(input, 1024, stdin) != NULL)
                {
                    input[strcspn(input, "\n")] = '\0';
                }
                continue;
            }
            else
            {

                break;
            }
        }
    }
    if (strcmp(option, "P") == 0)
    {
        while (1)
        {
            int i = 0;
            int len;
            len = strlen(input);
            char *Temp = input;
            while (*Temp)
            {
                if (isalpha(*Temp))
                {
                    i = 1;
                }
                Temp++;
            }
            if (i == 1 || len == 0 || (len > 9 || len < 9))
            {
                printf("Invalid input. Please enter a valid number phone (up to 9 characters): ");
                if (fgets(input, 1024, stdin) != NULL)
                {
                    input[strcspn(input, "\n")] = '\0';
                }
                continue;
            }
            else
            {
                break;
                return;
            }
        }
    }

    if (strcmp(option, "D") == 0)
    {
        while (1)
        {

            int len = strlen(input);
            int i = 0;
            // char *Temp = input
            if ((i == 1 || len > 9) || len == 0 || checkvalidamount(input) == 0)
            {
                printf("Invalid input. Please enter a valid numbe of Amount $$$ : ");
                if (fgets(input, 1024, stdin) != NULL)
                {
                    input[strcspn(input, "\n")] = '\0';
                }
                continue;
            }
            else
            {
                break;
                return;
            }
        }
    }
    if (strcmp(option, "T") == 0)
    {

        char *Type[] = {"saving", "current", "fixed01", "fixed02", "fixed03"};
        while (1)
        {
            int i = 0;
            bool f = false;
            while (i < 5)
            {
                if (strcmp(Type[i], input) == 0)
                {
                    f = true;
                    break;
                }
                i++;
            }
            if (f == false)
            {
                printf("Invalid input. Please enter a valid type account : ");
                if (fgets(input, 1024, stdin) != NULL)
                {
                    input[strcspn(input, "\n")] = '\0';
                }
                continue;
            }
            else
            {
                break;
            }
        }
    }
}

int isPrintableString(const char *str)
{
    while (*str)
    {
        if (!isprint(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
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
                int t = atoi(take);
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
                    printf("U take %d $ from account successfully !\n", t);
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