#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100  // Maximum number of users

#define RED     "\x1b[31m"
#define BRIGHT_RED "\x1b[38;5;196m" // Bright red for incorrect password
#define GREEN   "\x1b[32m" // Green for printed text
#define TURQUOISE "\x1b[38;5;51m" // Turquoise for options
#define ORANGE  "\x1b[38;5;214m" // Orange for inputs
#define RESET   "\x1b[0m"

// Function prototypes
void print_border();
void print_title(const char* title);
void print_main_title();
void clear_screen();

struct user {
    long int ac_num;
    char name[100];
    char password[30];
    double balance;
} u[MAX_USERS];  // Array of users

// Global variables
int user_count = 0;
int choice;
char passwords[30]; 
long int ac_no;
double depositm;
double withdrawm;

void main() {
    print_main_title(); // Print the main title once at the start
    
    while (1) {
        print_border();
        print_title("MAIN MENU");
        printf(TURQUOISE "1-Login\n2-New User\n3-Exit\n" RESET);
        printf(ORANGE "\nChoose an option:\t" RESET);
        scanf("%d", &choice);
        
        clear_screen();  // Clear screen after choice is taken
        
        switch (choice)
        {
        case 1: login(); 
            break;
        case 2: new_user();
            break;
        case 3: exit(0);  // Exit the program
            break;
        default: printf(GREEN "INVALID OPTION. \n" RESET);
            break;
        }
    }
}

/*******************************END OF MAIN*************************************/
void login()
{   
    FILE *fp;
    fp = fopen("Password_records.txt", "rb");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    print_border();
    print_title("LOGIN");
    printf(ORANGE "\nEnter Account Number:" RESET);
    scanf("%ld", &ac_no);
    printf(ORANGE "\nEnter password: " RESET);
    scanf("%s", passwords);

    clear_screen();  // Clear screen after login attempt
    
    int found = 0;
    user_count = 0; // Reset user_count for login
    while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
        if (u[user_count].ac_num == ac_no && strcmp(u[user_count].password, passwords) == 0) {
            found = 1;
            break;
        }
        user_count++;
    }

    fclose(fp);

    if (found) {
        submenu();
    } else {
        printf(BRIGHT_RED "\nIncorrect account number or password.\n" RESET);
    }
}

/*******************************END OF Login*************************************/
void new_user() 
{                             
    FILE *fp;
    fp = fopen("Password_records.txt", "ab");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    if (user_count >= MAX_USERS) {
        printf(GREEN "User limit reached. Cannot add more users.\n" RESET);
        fclose(fp);
        return;
    }

    print_border();
    print_title("NEW USER");
    printf(ORANGE "\nEnter Name:" RESET);
    scanf("%s", u[user_count].name);
    printf(ORANGE "\nEnter Password:" RESET);
    scanf("%s", u[user_count].password);
    printf(ORANGE "\nEnter Account number:" RESET);
    scanf("%ld", &u[user_count].ac_num);
    u[user_count].balance = 0.0;  // Initialize balance to 0

    fwrite(&u[user_count], sizeof(struct user), 1, fp);
    fclose(fp);

    clear_screen();  // Clear screen after new user registration
    
    user_count++;
    printf(GREEN "\nNew user added successfully!\n" RESET);
}

/*******************************END OF new user*************************************/
void submenu() 
{
    print_border();
    print_title("SUB MENU");
    printf(TURQUOISE "1-Deposit\n2-Withdraw\n3-Balance Inquiry\n4-Account Settings\n5-Log Out\n" RESET);
    printf("\n***************************************************************\n");
    printf(ORANGE "Choose an option:\n" RESET);
    scanf("%d", &choice);
    
    clear_screen();  // Clear screen after submenu selection
    
    switch (choice)
    {
        case 1: deposit();
            break;
        case 2: withdraw();
            break;
        case 3: balance_inquiry();
            break;
        case 4: account_settings();
            break;
        case 5: return; // Return to the main menu
            break;
        default: printf(GREEN "\nINVALID INPUT\n" RESET);
            break;
    }
}

/*******************************END OF SUB MENU*************************************/
void deposit()
{
    FILE *fp;
    fp = fopen("Password_records.txt", "rb+");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    print_border();
    print_title("DEPOSIT");
    printf(ORANGE "\nENTER THE AMOUNT TO DEPOSIT:\t" RESET);
    scanf("%lf", &depositm);

    clear_screen();  // Clear screen after deposit attempt
    
    int found = 0;
    while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
        if (u[user_count].ac_num == ac_no && strcmp(u[user_count].password, passwords) == 0) {
            found = 1;
            u[user_count].balance += depositm;
            fseek(fp, -sizeof(struct user), SEEK_CUR);
            fwrite(&u[user_count], sizeof(struct user), 1, fp);
            printf(GREEN "\nDeposit successful. New Balance: %.2lf\n" RESET, u[user_count].balance);
            break;
        }
    }

    if (!found) {
        printf(GREEN "User not found.\n" RESET);
    }

    fclose(fp);
}

/*******************************END OF deposit*************************************/
void withdraw() {
    FILE *fp;
    fp = fopen("Password_records.txt", "rb+");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    print_border();
    print_title("WITHDRAW");
    while (1) {  // Use a loop to continuously prompt for withdrawal amount
        printf(ORANGE "\nENTER THE AMOUNT TO WITHDRAW:\t" RESET);
        scanf("%lf", &withdrawm);

        clear_screen();  // Clear screen after withdrawal attempt
        
        int found = 0;
        // Search for the correct user record
        while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
            if (u[user_count].ac_num == ac_no && strcmp(u[user_count].password, passwords) == 0) {
                found = 1;
                if (withdrawm > u[user_count].balance) {
                    printf(GREEN "\nNot enough balance. Your current balance is %.2lf\n" RESET, u[user_count].balance);
                    fseek(fp, 0, SEEK_SET);  // Reset file pointer to re-read the user data
                    break;
                } else {
                    u[user_count].balance -= withdrawm;
                    fseek(fp, -sizeof(struct user), SEEK_CUR);  // Move the file pointer back
                    fwrite(&u[user_count], sizeof(struct user), 1, fp);  // Rewrite the updated user data
                    printf(GREEN "\nWithdrawal successful. New Balance: %.2lf\n" RESET, u[user_count].balance);
                    fclose(fp);
                    return;  // Exit the function after successful withdrawal
                }
            }
        }

        if (!found) {
            printf(GREEN "User not found.\n" RESET);
            break;
        }

        fseek(fp, 0, SEEK_SET);  // Reset file pointer to re-read the user data
    }

    fclose(fp);
    return;
}

/*******************************END OF Withdraw*************************************/
void balance_inquiry()
{
    FILE *fp;
    fp = fopen("Password_records.txt", "rb");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    print_border();
    print_title("BALANCE INQUIRY");
    int found = 0;
    while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
        if (u[user_count].ac_num == ac_no && strcmp(u[user_count].password, passwords) == 0) {
            found = 1;
            printf(GREEN "\nAccount Number: %ld" RESET, u[user_count].ac_num);
            printf(GREEN "\nName: %s" RESET, u[user_count].name);
            printf(GREEN "\nBalance: %.2lf\n" RESET, u[user_count].balance);
            break;
        }
    }

    if (!found) {
        printf(GREEN "User not found.\n" RESET);
    }

    fclose(fp);
}

/*******************************END OF Balance Inquiry*************************************/
void account_settings()
{
    print_border();
    print_title("ACCOUNT SETTINGS");
    printf(TURQUOISE "1-Change Password\n2-Delete Account\n3-Back\n" RESET);
    printf("\n***************************************************************\n");
    printf(ORANGE "Choose an option:\n" RESET);
    scanf("%d", &choice);

    clear_screen();  // Clear screen after settings choice
    
    switch (choice)
    {
        case 1: change_password();
            break;
        case 2: delete_account();
            break;
        case 3: return; // Back to the submenu
            break;
        default: printf(GREEN "INVALID INPUT\n" RESET);
            break;
    }
}

/*******************************END OF ACCOUNT SETTINGS*************************************/
void change_password()
{
    FILE *fp;
    fp = fopen("Password_records.txt", "rb+");
    if (fp == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    print_border();
    print_title("CHANGE PASSWORD");
    printf(ORANGE "\nEnter New Password:" RESET);
    scanf("%s", passwords);

    clear_screen();  // Clear screen after password change attempt
    
    int found = 0;
    while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
        if (u[user_count].ac_num == ac_no) {
            found = 1;
            strcpy(u[user_count].password, passwords);
            fseek(fp, -sizeof(struct user), SEEK_CUR);
            fwrite(&u[user_count], sizeof(struct user), 1, fp);
            printf(GREEN "\nPassword changed successfully.\n" RESET);
            break;
        }
    }

    if (!found) {
        printf(GREEN "User not found.\n" RESET);
    }

    fclose(fp);
}

/*******************************END OF CHANGE PASSWORD*************************************/
void delete_account()
{
    FILE *fp, *temp_fp;
    if ((fp = fopen("Password_records.txt", "rb")) == NULL) {
        printf(GREEN "Error opening file.\n" RESET);
        return;
    }

    if ((temp_fp = fopen("Temp_records.txt", "wb")) == NULL) {
        printf(GREEN "Error creating temporary file.\n" RESET);
        fclose(fp);
        return;
    }

    print_border();
    print_title("DELETE ACCOUNT");
    printf(ORANGE "\nEnter Account Number to Delete: " RESET);
    scanf("%ld", &ac_no); // Ensure ac_no is set properly

    clear_screen();  // Clear screen after user input

    int found = 0;
    while (fread(&u[user_count], sizeof(struct user), 1, fp)) {
        if (u[user_count].ac_num != ac_no) {
            fwrite(&u[user_count], sizeof(struct user), 1, temp_fp);
        } else {
            found = 1;
        }
    }

    if (fclose(fp) != 0) {
        printf(GREEN "Error closing original file.\n" RESET);
    }
    if (fclose(temp_fp) != 0) {
        printf(GREEN "Error closing temporary file.\n" RESET);
    }

    if (found) {
        if (remove("Password_records.txt") != 0) {
            printf(GREEN "Error removing original file. Ensure the file is not open in another program.\n" RESET);
        } else {
            printf(GREEN "Original file removed successfully.\n" RESET);
        }

        if (rename("Temp_records.txt", "Password_records.txt") != 0) {
            printf(GREEN "Error renaming temporary file. Ensure the file is not open in another program.\n" RESET);
        } else {
            printf(GREEN "Temporary file renamed successfully.\n" RESET);
        }
    } else {
        printf(GREEN "Account not found.\n" RESET);
        remove("Temp_records.txt"); // Remove temporary file if no account is deleted
    }
}


/*******************************END OF DELETE ACCOUNT*************************************/
void print_border()
{
    printf("\n************************************************************************************************************\n");
}

void print_title(const char* title) {
    int length = strlen(title);
    int padding = (100 - length) / 2;  // Center the title in a 100-character wide space

    printf("\n");
    printf("%*s%s%*s\n", padding, "", title, padding, "");
}

void print_main_title() {
    print_border();
    print_title("BANK MANAGEMENT SYSTEM");
    print_border();
}

void clear_screen() {
    system("cls"); // For Windows
    // system("clear"); // Uncomment this line if using Unix os
}

