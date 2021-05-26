#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include "macros.h"
#include "user.h"

void showMenu(int s_fd);
void deposit(int s_fd);
void withdraw(int s_fd);
void balanceEnquiry(int s_fd);
void changePassword(int s_fd);
void viewDetails(int s_fd);
void addAccount(int s_fd);
void deleteAccount(int s_fd);
void modifyAccount(int s_fd);
void searchAccount(int s_fd);
void choose(int s_fd);
void attemptUserLogin(int s_fd);

int option,userid;

void choose(int s_fd)
{
	while(1)
    {
		write(1,"1 : Normal User\n",17);
		write(1,"2 : Joint Account User\n",24);
		write(1,"3 : Administrator\n",19);
		
		write(1,"Choose an option from above menu : ",36);
		scanf("%d",&option);
		printf("Option : %d\n",option);

		if(option == 1 ||option == 2||option == 3)
        {
			attemptUserLogin(s_fd); 
			break;
		}
		else
        {
            write(1,"Invalid option\n",16);
        }
	}
	return;
}

void attemptUserLogin(int s_fd)
{
	bool result;
	User user;
    write(1,"Enter Credentials ",19);
	write(1,"User ID : ",11);
	scanf("%d",&user.userID);
	userid=user.userID;
	write(1,"Password : ",12);
	scanf("%s",user.password);
	write(s_fd,&option,sizeof(int));
	write(s_fd,&user,sizeof(User));
	read(s_fd,&result,sizeof(result)); 
	if(!result)
    {
		write(1,"Invalid login\n",15);
		choose(s_fd);
	}
	else
    {
		write(1,"Login Successful\n",18);
	}
	return;
}

void showMenu(int s_fd)
{
	int select;
	if(option==1 || option==2){
		write(1,"1 : Deposit\n",13);
		write(1,"2 : Withdraw\n",14);
		write(1,"3 : Balance Enquiry\n",21);
		write(1,"4 : Password Change\n",21);
		write(1,"5 : View Details\n",18);
		write(1,"6 : Exit\n",10);
	
		write(1,"Choose an option from above menu: ",35);
		scanf("%d",&select);
		printf("Option : %d\n",select);

        if(select == 1)
        {
            deposit(s_fd);
        }
		else if(select == 2)
        {
            withdraw(s_fd);
        }
        else if(select == 3)
        {
            balanceEnquiry(s_fd);
        }
        else if(select == 4)
        {
            changePassword(s_fd);
        }
        else if(select == 5)
        {
            viewDetails(s_fd);
        }
        else if(select == 6)
        {
            write(s_fd,&select,sizeof(int));
			write(1,"Exiting...\n",12);
			exit(0);
        }
        else
        {
            write(1,"Invalid option\n",16);
			showMenu(s_fd);
        }
	}
	else if(option==3)
    {
		write(1,"1 : Add Account\n",17);
		write(1,"2 : Delete Account\n",20);
		write(1,"3 : Modify Account\n",20);
		write(1,"4 : Search Account\n",20);
		write(1,"5 : Exit\n",10);

		write(1,"Choose an option from above menu: ",35);
		scanf("%d",&select);
		printf("Option : %d\n",select);
        if(select == 1)
        {
            addAccount(s_fd);
        }
		else if(select == 2)
        {
            deleteAccount(s_fd);
        }
        else if(select == 3)
        {
            modifyAccount(s_fd);
        }
        else if(select == 4)
        {
            searchAccount(s_fd);
        }
        else if(select == 5)
        {
            write(s_fd,&select,sizeof(int));
		    write(1,"Exiting...\n",12);
			exit(0);
        }
        else
        {
            write(1,"Invalid option\n",16);
			showMenu(s_fd);
        }
	}
}

void balanceEnquiry(int s_fd){
	float amount;
	int select=3;
	write(s_fd,&select,sizeof(int));
	read(s_fd,&amount,sizeof(float));
	write(1,"Available Balance in account :: Rs.",36);
	printf("%0.2f\n\n",amount);
	showMenu(s_fd);
	return;
}

void changePassword(int s_fd){
	int select=4;
	char nPassword[10];
	bool result;

	write(1,"Enter new password(max 9 characters) : ",40);
	scanf("%s",nPassword);

	write(s_fd,&select,sizeof(int));
	write(s_fd,nPassword,sizeof(nPassword));

	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Error changing password\n",25);
	}
	else
    {
		write(1,"Succesfully changed password\n",30);
	}
	showMenu(s_fd);
	return;
}

void deposit(int s_fd)
{
	float amount;
	int select=1;
	bool result;

	write(1,"Enter Amount to be deposited : Rs.",35);
	scanf("%f",&amount);

	write(s_fd,&select,sizeof(int));
	write(s_fd,&amount,sizeof(float));

	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Couldn't process the transaction\n",34);
	}
	else
    {
		write(1,"Succesfully deposited\n",23);
	}
	showMenu(s_fd);
	return;
}

void withdraw(int s_fd){
	float amount;
	int select=2;
	bool result;

	write(1,"Amount to be withdrawn : Rs.",29);
	scanf("%f",&amount);

	write(s_fd,&select,sizeof(int));
	write(s_fd,&amount,sizeof(float));

	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Couldn't process the transaction\n",34);
	}
	else
    {
		write(1,"Withdrawn successfully\n",24);
	}
    showMenu(s_fd);
	return;
}

void viewDetails(int s_fd)
{
	int select=5;

	write(s_fd,&select,sizeof(int));

    User user;
	read(s_fd,&user,sizeof(User));

	if(option==1)
    {
		printf("User ID : %d\n",user.userID);
		printf("Name : %s\n",user.name);
		printf("Account Number : %d\n",user.account_no);
		printf("Available Balance : Rs.%0.2f\n",user.balance);
		printf("Status : %s\n\n",user.status);
	}
	else if(option==2)
    {	
		printf("User ID : %d\n",user.userID);
		printf("Main Account Holder's Name : %s\n",user.name);
		printf("Other Account Holder's Name : %s\n",user.name2);
		printf("Account Number : %d\n",user.account_no);
		printf("Available Balance : Rs.%0.2f\n",user.balance);
		printf("Status : %s\n\n",user.status);
	}
	showMenu(s_fd);
	return;
}

void addAccount(int s_fd){
	int select=1;
	int type;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter type of account to be added(1: Normal Account 2: Joint Account) : ",73);
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));

    User user;
	if(type==1)
    {
		
		write(1,"Name of the account holder : ",30);
		scanf(" %[^\n]",user.name);
		write(1,"Password(max 9 characters) : ",30);
		scanf("%s",user.password);
		write(1,"Initial Deposit : Rs.",22);
		scanf("%f",&user.balance);
		write(s_fd,&user,sizeof(User));
	}

	if(type==2){
		write(1,"Name of the primary account holder : ",38);
		scanf(" %[^\n]",user.name);
		write(1,"Name of the other account holder : ",36);
		scanf(" %[^\n]",user.name2);
	    write(1,"Password(max 9 characters) : ",30);
		scanf("%s",user.password);
		write(1,"Initial Deposit : Rs.",22);
		scanf("%f",&user.balance);
		write(s_fd,&user,sizeof(User));
	}
	
	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Error adding account\n",22);
	}
	else
    {
		write(1,"Succesfully added account\n",27);
	}
	showMenu(s_fd);
	return;
}

void modifyAccount(int s_fd){
	int select=3;
	int type;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter type of account to be modified(1: Normal Account 2: Joint Account) : ",76);
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));
    User user;
	if(type==1)
    {
		write(1,"User ID : ",11);
		scanf("%d",&user.userID);
		write(1,"Account Number : ",18);
		scanf("%d",&user.account_no);
		write(1,"New Name of the account holder : ", 34);
		scanf(" %[^\n]",user.name);
		write(1,"New Password(max 09 characters) : ",35);
		scanf("%s",user.password);
		write(1,"New Balance : ",15);
		scanf("%f",&user.balance);
		write(s_fd,&user,sizeof(User));
	}

	if(type==2)
    {
		write(1,"User ID : ",11);
		scanf("%d",&user.userID);
		write(1,"Account Number : ",18);
		scanf("%d",&user.account_no);
		write(1,"New Name of the primary account holder : ",42);
		scanf(" %[^\n]",user.name);
		write(1,"New Name of the other account holder : ",40);
		scanf(" %[^\n]",user.name2);
		write(1,"New Password(max 09 characters) : ",35);
		scanf("%s",user.password);
		write(1,"New Balance : ",15);
		scanf("%f",&user.balance);
		write(s_fd,&user,sizeof(User));
	}
	
	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Error modifying account\n",25);
	}
	else
    {
		write(1,"Succesfully modified account\n",30);
	}
	showMenu(s_fd);
	return;
}

void deleteAccount(int s_fd)
{
	int select=2;
	int type,id;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter type of account to be deleted(1: Normal Account 2: Joint Account) : ",75);
	scanf("%d",&type);
	
	write(s_fd,&type,sizeof(int));

	write(1,"User ID to be deleted: ",24);
	scanf("%d",&id);
	write(s_fd,&id,sizeof(int));
	
	read(s_fd,&result,sizeof(result)); 

	if(!result)
    {
		write(1,"Error deleting account\n",24);
	}
	else
    {
		write(1,"Succesfully deleted account\n",29);
	}
	showMenu(s_fd);
	return;
}

void searchAccount(int s_fd){
	int select=4;
	int type,len;
	bool result;

	write(s_fd,&select,sizeof(int));
	write(1,"Enter type of account to be searched(1: Normal Account 2: Joint Account) : ",76);
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));
	write(1,"User ID : ",sizeof("User ID : "));
	int userID;
	scanf("%d",&userID);
	write(s_fd,&userID,sizeof(int));
	User user;
	len = read(s_fd,&user,sizeof(User));
	if(user.userID!=userID)
	{
		write(1,"Please re-check the User ID\n",29);
	}
	else
	{
		if(type==1)
    	{
			printf("User ID : %d\n",user.userID);
			printf("Name : %s\n",user.name);
			printf("Account Number : %d\n",user.account_no);
			printf("Available Balance : Rs.%0.2f\n",user.balance);
			printf("Status : %s\n\n",user.status);
		}
		else if(type==2)
    	{	
			printf("User ID : %d\n",user.userID);
			printf("Main Account Holder's Name : %s\n",user.name);
			printf("Other Account Holder's Name : %s\n",user.name2);
			printf("Account Number : %d\n",user.account_no);
			printf("Available Balance : Rs.%0.2f\n",user.balance);
			printf("Status : %s\n\n",user.status);
		}
	}
	showMenu(s_fd);
	return;
}

int main(){
	struct sockaddr_in server;
	int s_fd,msgLength;
	char buff[50];
	char result;
    int client_fd;
    int login_status;
    char Buffer[1024] = "";

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("client socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(CLIENT_PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0){
        perror("ip address");
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("client connect");
        exit(EXIT_FAILURE);
    }
	printf("Connected to server!!!\n\n");
	choose(client_fd);
	showMenu(client_fd);	

	close(client_fd);

	return 0;
}









