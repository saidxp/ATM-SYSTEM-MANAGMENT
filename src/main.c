#include "header.h"
#include <string.h>
#include <stdio.h>



void mainMenu(struct User *u)
{
    int option;
    char optionStr[100];
    char *result;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    printf("\n\n\t\tEnter your choice: ");
    result = fgets(optionStr, sizeof(optionStr), stdin);
    
    // handle input validation and buffer overflow
    while (1) {
        // check if fgets succeeded
        if (result == NULL) {
            printf("Error reading input. Please try again: ");
            result = fgets(optionStr, sizeof(optionStr), stdin);
            continue;
        }
        
        // check for buffer overflow if newline is missing !!
        if (strchr(optionStr, '\n') == NULL) {
            // input was too long (buffer overflow attempt)
            // clear the input buffer !!
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("Invalid input! Please enter a single digit (1-8): ");
            result = fgets(optionStr, sizeof(optionStr), stdin);
            continue;
        }
        // remove newline
        optionStr[strcspn(optionStr, "\n")] = '\0';
        
        // check if input is valid (single digit between 1-8)
        if (strlen(optionStr) != 1 || optionStr[0] < '1' || optionStr[0] > '8') {
            printf("Invalid input! Please enter a single digit (1-8): ");
            result = fgets(optionStr, sizeof(optionStr), stdin);
            continue;
        }
        // Valid input, convert to integer and break safe then atoi();
        option = optionStr[0] - '0';
        break;
    }

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        //getchar();
        updateacount(u);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        //printf("u.name : %s\n", u->name);
        checkAcount(u);
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // here
        //getchar();
        transaction(u);
        break;
    case 6:
     
        // here
        removeacount(u);
        break;
    case 7:
        // here
        //getchar();
        transferownership(u);
        break;
    case 8:
        initMenu(u);
         break;
    default:
        stayOrReturn(0, mainMenu, u);
    }
}

void initMenu(struct User *u)
{
    int r = 0;
    char option[50];
    int op;
    int j;
    bool sign = false;

    while (!r)
    {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");
        printf("\n\t\tEnter your choice: ");
        
        // Flush output buffer to ensure prompt is displayed
       
        
        // Read input with error checking
        if (fgets(option, sizeof(option), stdin) == NULL) {
            printf("\nError reading input. Please try again.\n");
            sleep(1);
            continue;
        }
        
        // Check for buffer overflow
        if (strchr(option, '\n') == NULL) {
            // Input was too long - clear the input buffer
            system("clear");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            printf("\n\n\t\tInput too long. Please enter a single digit (1-3).\n");
            printf("\n\n\t\tPress Enter to continue...");
            fgets(option, sizeof(option), stdin); // Wait for Enter key
            continue;
        }
        
        // Remove newline character
        option[strcspn(option, "\n")] = '\0';
        
        // validate input
        j = strlen(option);
        if (j == 0 || j > 1 || !isdigit(option[0]) || option[0] < '1' || option[0] > '3')
        {
            system("clear");
            printf("\n\n\t\tInvalid input! Please enter a single digit (1-3).\n");
            printf("\n\n\t\tPress Enter to continue...");
            fflush(stdout);
            fgets(option, sizeof(option), stdin); // Wait for Enter key
            continue;
        }
        
        // convert character to integer (safer than atoi)
        op = option[0] - '0';
        
        switch (op)
        {
        case 1:
            loginMenu(u);  
            if ((strcmp(u->password, getPassword(u)) == 0) && CheCk(u->name))
            {   
                printf("\n\n\t\t\tWelcome %s\n", u->name);
                mainMenu(u);
                r = 1;
            }
            else
            {   
                system("clear");
                printf("\n\n\t\t\tWrong password!! or User Name\n");
                printf("\n\n\t\t\tPlease try again\n");
                printf("\n\n\t\t\tPress Enter to continue...");
                
                // clear any existing input in buffer
                int ch;
                
                // safely read the "press enter" input with overflow protection
                if (fgets(option, sizeof(option), stdin) == NULL) {
                    // Error handling if needed
                }
                
                // clear input buffer if overflow occurred
                if (strchr(option, '\n') == NULL) {
                    while ((ch = getchar()) != '\n' && ch != EOF);
                }
            }
            break;
            
        case 2:
            sign = signUp(u);
            if (sign == false) {
                continue;
            } else {
                mainMenu(u);
            }
            r = 1; // exit loop after registration and main menu
            break;
            
        case 3:
            printf("\n\n\t\t\tThank you for using our ATM. Goodbye!\n");
            exit(0); 
            break;
            
        default:
            printf("\n\n\t\t\tInsert a valid operation (1-3)!\n");
            printf("\n\n\t\t\tPress Enter to continue...");
            
            // clear any existing input in buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            // safely read the "press enter" input with overflow protection
            if (fgets(option, sizeof(option), stdin) == NULL) {
                ;
            }
            // clear input buffer if overflow occurred
            if (strchr(option, '\n') == NULL) {
                while ((ch = getchar()) != '\n' && ch != EOF);
            }
            break;
        }
    }
}

int main()
{
    struct User u;
    // ATM FRONT USER
    initMenu(&u);
    /// ATM Menu TO Choose
    //mainMenu(&u);
    return 0;
}
