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
    // ==== >> variables to handle input
    char day[16];         
    char Tnumber[16];    
    char country[64];     
    char phone[16];       
    char Amount[32];     
    char Type[32];        
    
    // Check if file was opened successfully
    if (pf == NULL) {
        perror("Error opening records file");
        return;
    }

    system("clear");

noAccount:
    system("clear");
    printf("\n\n\t\t\t   ======= ATM =======\n");
    printf("\t\t\t===== New record =====\n");

    // Get date with buffer overflow protection
    printf("\nEnter today's date(mm/dd/yyyy): ");
    if (fgets(day, sizeof(day), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // Check for overflow using strchr to find newline character
    if (strchr(day, '\n') == NULL) {
        // Input was too long, clear the remaining buffer
        clearInputBuffer();
        printf("Input too long. Please try again with a shorter date.");
        goto noAccount;
    }
    // Remove newline character
    day[strcspn(day, "\n")] = '\0';
    /// CHECK THE DATE FORMAT !!! 
    CheCkday(day, &r);
    printf("The date is: %s\n", day);
    
    // Get account number with buffer overflow protection
    printf("\nEnter number of Account: ");
    if (fgets(Tnumber, sizeof(Tnumber), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // Check for overflow
    if (strchr(Tnumber, '\n') == NULL) {
        clearInputBuffer();
        printf("Input too long, (Eroor at Number of Acount) << ! Please respect the limit of 6 digits or u will return to the starting point !!! >>");
        goto noAccount;
    }
    
    // Remove newline character
    Tnumber[strcspn(Tnumber, "\n")] = '\0';
    
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
    
    /// Set ID based on count
    if (count == 0)
    {
        r.id = 0;
    }
    else
    {
        r.id = cr.id + 1;
    }
    
    // Get country with buffer overflow protection
    printf("\nEnter the country: ");
    if (fgets(country, sizeof(country), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // Check for overflow
    if (strchr(country, '\n') == NULL) {
        clearInputBuffer();
        printf("Input too long, (Error at Country name) << Please respect the limit of 50 characters or u will return to the starting point >> !!");
        goto noAccount;
    }
    // Remove newline character
    country[strcspn(country, "\n")] = '\0';
    
    validprompt(country, "C");
    strcpy(r.country, country);
    
    // Get phone with buffer overflow protection
    printf("\nEnter the phone number: ");
    if (fgets(phone, sizeof(phone), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // Check for overflow
    if (strchr(phone, '\n') == NULL) {
        clearInputBuffer();
        printf("Input too long, (Error at Phone number) << Please respect the limit of 9 digits or u will return to the starting point >> !!");
        goto noAccount;
    }
    
    // remove newline character
    phone[strcspn(phone, "\n")] = '\0';
    
    validprompt(phone, "P");
    r.phone = atoi(phone);
    
    // get amount to deposit with buffer overflow protection
    printf("\nEnter amount to deposit $: ");
    if (fgets(Amount, sizeof(Amount), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // check for overflow
    if (strchr(Amount, '\n') == NULL) {
        clearInputBuffer();
        printf("Input too long ,(Error at Amount phase) << please respect the limit of 9 digits or u will return to the starting point >> !!");
        goto noAccount;
    }
    
    // remove newline character
    Amount[strcspn(Amount, "\n")] = '\0';
    // HER I DETETCT EROOOR !
    validprompt(Amount, "D");
    r.amount = strtod(Amount, NULL);
    
    // Get account type with buffer overflow protection
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    if (fgets(Type, sizeof(Type), stdin) == NULL) {
        printf("Error reading input. Please try again.\n");
        goto noAccount;
    }
    
    // Check for overflow
    if (strchr(Type, '\n') == NULL) {
        clearInputBuffer();
        printf("Input too long, (Error at Type of account) << Please respect the limit 7 characters or u will return to the starting point >> !!");
        goto noAccount;
    }
    
    // Remove newline character
    Type[strcspn(Type, "\n")] = '\0';
    
    validprompt(Type, "T");
    strcpy(r.accountType, Type);
    
    saveAccountToFile(pf, u, &r);
    fclose(pf);
    success(u);
}

// Helper function to safely clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
