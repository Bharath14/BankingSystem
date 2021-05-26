#ifndef USER_H
#define USER_H


typedef struct User{
	int userID;
    int account_no;
	char name[50];
	char name2[50];
	char password[10];
	float balance;
	char status[25];
	int n_users;
}User;

#endif
