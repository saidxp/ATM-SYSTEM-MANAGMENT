#include <termios.h>
#include "header.h"
#include <string.h>
#include <stdio.h>
char *USERS = "../data/users.txt";

void loginMenu(struct User *u)
{
    struct termios oflags, nflags;
    int r = 0;
    char Tname[100];
    char Tpassword[100];
    bool n = false;
    bool p = false;
    
    int c = 0;
    while (!r)
    {
        system("clear");
         
        printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
        if (fgets(Tname, sizeof(Tname), stdin) == NULL)
        {
            printf("\nError reading input. Please try again.\n");
            sleep(1);
            c++;
            continue;
        }
        // Check for buffer overflow in name
        if (strchr(Tname, '\n') == NULL)
        {
            // Input was too long - clear the input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            printf("\n\n\t\t\t\tInput too long. Maximum %d characters allowed.", (int)sizeof(Tname) - 1);
            sleep(1);
            c++;
            continue;
        }

        // Remove newline character
        Tname[strcspn(Tname, "\n")] = '\0';
        if (strlen(Tname) == 0 || ISNAME(Tname))
        {
            printf("\n\n\t\t\t\tPlease enter a valid name Mybe empty or invalid characters");
            n = false;
            c += 1;
            continue;
        }
        else
        {
            n = true;
        }
        // disabling echo
        tcgetattr(fileno(stdin), &oflags);
        nflags = oflags;
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            return exit(1);
        }
        printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
        if (fgets(Tpassword, sizeof(Tpassword), stdin) == NULL) {
            // Restore terminal settings first
            if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
                perror("tcsetattr");
                return exit(1);
            }
            
            printf("\nError reading input. Please try again.\n");
            sleep(1);
            p = false;
            c++;
            continue;
        }
        
        // Check for buffer overflow in password
        if (strchr(Tpassword, '\n') == NULL) {
            // Restore terminal settings first
            if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
                perror("tcsetattr");
                return exit(1);
            }
            
            // Clear the input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            printf("\n\n\t\t\t\tPassword too long. Maximum %d characters allowed.", (int)sizeof(Tpassword) - 1);
            sleep(1);
            p = false;
            c++;
            continue;
        }
        
        // Remove newline character
        Tpassword[strcspn(Tpassword, "\n")] = '\0';
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
            return exit(1);
        }
        if (strlen(Tpassword) == 0)
        {
            printf("\n\n\t\t\t\tPlease enter a valid password");
            p = false;
            c += 1;
            continue;
        }
        else
        {
            p = true;
        }
        if (n == true && p == true)
        {
            strcpy(u->name, Tname);
            strcpy(u->password, Tpassword);
            r = 1;
        }
        // restore terminal
    }
}

const char *getPassword(struct User *u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("../data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file ----");
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u->name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;

            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}