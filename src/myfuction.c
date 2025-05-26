#include <termios.h>
#include "header.h"
#include <unistd.h>
#include <string.h>
#include <ctype.h>
// const char *RECORDS = "../data/records.txt";

 
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
                printf("Invalid input. Please enter a valid number (up to 6 digits): ");

                if (fgets(input, 1024, stdin) != NULL)
                {
                    // check for buffer overflow protection
                    if (strchr(input, '\n') == NULL) {
                        // input too long, clear the input buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("\033[0;31m");
                        printf("<<(Input too long)!>> ");
                        printf("\033[0m");
                        continue;
                    }
                    
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
            if (pf == NULL) {
                printf("Error opening country file.\n");
                break;
            }
            
            while (fgets(c.country,  50, pf) != NULL) // Limited buffer as you had
            {
                c.country[strcspn(c.country, "\n")] = '\0';
                if (strcmp(c.country, input) == 0)
                {
                    i = 1;
                    break;
                }
            }
            fclose(pf);
            if (len == 0 || !isPrintableString(input) || i == 0)
            {
                printf("Invalid input. Please enter a valid country name (Must be Uppercase First Letter): ");
                if (fgets(input, 1024, stdin) != NULL)
                {
                    // Check for buffer overflow protection
                    if (strchr(input, '\n') == NULL) {
                          //printf("input : %s\n", input);
                        // Input too long, clear the input buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("\033[0;31m");
                        printf("<<(Input too long)!>> ");
                        printf("\033[0m");
                        continue;
                    }
                    input[strcspn(input, "\n")] = '\0';
                    //printf("input : %s\n", input);
                    
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
            len = strlen(input);
            char *Temp = input;
            
            while (*Temp)
            {
                if (isalpha(*Temp))
                {
                    i = 1;
                    break;
                }
                Temp++;
            }
            
            if (i == 1 || len == 0 || len != 9) // Fixed: should be exactly 9, not range check
            {
                printf("Invalid input. Please enter a valid phone number (exactly 9 digits): ");
                if (fgets(input,  1024, stdin) != NULL)
                {
                    // Check for buffer overflow protection
                    if (strchr(input, '\n') == NULL) {
                        // Input too long, clear the input buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("\033[0;31m");
                        printf("<<(Input too long)!>> ");
                        printf("\033[0m");
                        continue;
                    }
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
                    // Check for buffer overflow protection
                    if (strchr(input, '\n') == NULL) {
                        // Input too long, clear the input buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("\033[0;31m");
                        printf("<<(Input too long)!>> ");
                        printf("\033[0m");
                        continue;
                    }
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
                    // Check for buffer overflow protection
                    if (strchr(input, '\n') == NULL) {
                        // Input too long, clear the input buffer
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        printf("\033[0;31m");
                        printf("<<(Input too long)!>> ");
                        printf("\033[0m");
                        continue;
                    }
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
 