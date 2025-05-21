#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

struct Date
{
    int month, day, year;
};

typedef struct RateData
{
    int year;
    double rate;
}yearandrate;

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

struct  Temp
{
    int id;
    char name[50];
    char password[50];
};

struct Country 
{
    char country[50];
};



// authentication functions
void loginMenu(struct User *u);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User *u);

// system function
void createNewAcc(struct User *u);
void mainMenu(struct User *u);
void checkAllAccounts(struct User *u);
void checkAcount(struct User *u);
int getAccountFromFile(FILE *ptr, struct Record *r);
bool signUp(struct User *u);
bool CheCk(char name[50]);
int getNameFromFile(FILE *ptr, struct Temp *us);
void initMenu(struct User *u);
void saveusersToFile(FILE *ptr, struct User *u);
int getUserId(const char *username);
void updateacount(struct User *u);
void  removeacount(struct  User *u);
void stayOrReturn(int notGood, void f(struct User *u), struct User *u);
int ISNAME(char name[50]);
int readCleanLine(char *buffer, size_t size);
void  CheCkday(char *date, struct Record *r);
void validprompt(char *input, char *option);
int isPrintableString(const char *str);
void transaction(struct User *u);
void success(struct User *u);
void transferownership(struct  User *u);
int isValidFormat(const char *date);
int Checkifexist(int nbr, char *name);
bool checkvalidamount(char *str);
yearandrate calculaterate(double Amount, char *Type, int year);
#endif
