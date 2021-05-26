#include "create_accounts.h"

int getNewNormalUserID(){
	int fd=open("NormalUsers",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

int getNewJointUserID(){
	int fd=open("JointUsers",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

int getNewAdminUserID(){
	int fd=open("Administrators",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

void chooseOption(){
	int option;
	while(1){
		write(1,"1 : Create Normal-User Account\n",sizeof("1 : Create Normal-User Account\n"));
		write(1,"2 : Create Joint-User Account\n",sizeof("2 : Create Joint-User Account\n"));
		write(1,"3 : Create Admin Account\n",sizeof("3 : Create Admin Account\n"));
		write(1,"4 : Exit\n",sizeof("4 : Exit\n"));
		
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&option);
		printf("Option : %d\n",option);
		if(option == 1){
			Create_NormalUser(); 
			break;
		}
		else if(option == 2){
			Create_JointUser(); 
			break;
		}
		else if(option == 3){
			Create_Admin();
			break;
		}
		else if(option == 4){
			break;
		}
		else write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
	}
	return;
}

void Create_JointUser(){
	int fd=open("JointUsers",O_RDWR | O_CREAT,0744);
	int choice=0;
	User newJUser;
	printf("Please enter the main name of the joint user1: ");
	scanf(" %[^\n]",newJUser.name);
	printf("Please enter the second name of the joint user1: ");
	scanf(" %[^\n]",newJUser.name2);
	printf("Please enter the password(max 15 characters): ");
	scanf(" %[^\n]",newJUser.password);
	newJUser.userID=2000;
	newJUser.balance=1000;
	newJUser.account_no=(newJUser.userID-2000)+200000;
	newJUser.n_users = MAX_JOINT_USERS;
	printf("Your userID is : %d\n",newJUser.userID);
	strcpy(newJUser.status,"ACTIVE");
	write(fd,&newJUser,sizeof(User));
	printf("Do you want to create another new joint account?\n\nIf yes : 1 or if no : 0 - ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the main name of the joint user: ");
		scanf(" %[^\n]",newJUser.name);
		printf("Please enter the second name of the joint user: ");
		scanf(" %[^\n]",newJUser.name2);
		printf("Please enter the password(max 15 characters): ");
		scanf(" %[^\n]",newJUser.password);
		newJUser.userID=getNewJointUserID();
		newJUser.balance=1000;
		newJUser.account_no=(newJUser.userID-2000)+200000;
		newJUser.n_users = MAX_JOINT_USERS;
		printf("Your userID is : %d\n",newJUser.userID);
		strcpy(newJUser.status,"ACTIVE");
		write(fd,&newJUser,sizeof(User));
		printf("Do you want to create another new joint account?\n\nIf yes : 1 or if no : 0 - ");
		scanf("%d",&choice);
	}
	close(fd);
	chooseOption();
	
}

void Create_NormalUser(){
	int fd=open("NormalUsers",O_RDWR | O_CREAT,0744);
	int choice=0;
	User newNUser;
	printf("Please enter the name of the normal user: ");
	scanf(" %[^\n]",newNUser.name);
	printf("Please enter the password(max 15 characters): ");
	scanf(" %[^\n]",newNUser.password);
	newNUser.userID=1000;
	newNUser.balance=1000;
	newNUser.account_no=(newNUser.userID-1000)+100000;
	newNUser.n_users = MAX_NORMAL_USERS;
    strcpy(newNUser.name2,"");
	printf("Your userID is : %d\n",newNUser.userID);
	strcpy(newNUser.status,"ACTIVE");
	write(fd,&newNUser,sizeof(User));
	printf("Do you want to create another new normal account?\n\nIf yes : 1 or if no : 0 - ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the normal user: ");
		scanf(" %[^\n]",newNUser.name);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]",newNUser.password);
		newNUser.userID=getNewNormalUserID();
		newNUser.balance=1000;
		newNUser.account_no=(newNUser.userID-1000)+100000;
		newNUser.n_users = MAX_NORMAL_USERS;
        strcpy(newNUser.name2,"");
		printf("Your userID is : %d\n",newNUser.userID);
		strcpy(newNUser.status,"ACTIVE");
		write(fd,&newNUser,sizeof(User));
		printf("Do you want to create another new normal account?\n\nIf yes : 1 or if no : 0 - ");
		scanf("%d",&choice);
	}
	close(fd);
	chooseOption();
	
}

void Create_Admin(){
	int fd=open("Administrators",O_RDWR | O_CREAT,0744);
	int choice=0;
	User newAdmin;
	printf("Please enter the name of the admin: ");
	scanf(" %s",newAdmin.name);
	printf("Please enter the password(max 15 characters): ");
	scanf(" %s",newAdmin.password);
	newAdmin.userID=3000;
    newAdmin.balance = 0;
    newAdmin.account_no = 0;
	newAdmin.n_users = MAX_ADMIN;
    strcpy(newAdmin.name2,"");
    strcpy(newAdmin.status,"Admin");
	printf("Your userID is : %d\n",newAdmin.userID);
	write(fd,&newAdmin,sizeof(newAdmin));
	printf("Do you want to create another new admin account?\n\nIf yes : 1 or if no : 0 - ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the admin: ");
		scanf(" %[^\n]",newAdmin.name);
		printf("Please enter the password(max 15 characters): ");
		scanf(" %[^\n]",newAdmin.password);
		newAdmin.userID=getNewAdminUserID();
        newAdmin.balance = 0;
        newAdmin.account_no = 0;
		newAdmin.n_users = MAX_ADMIN;
        strcpy(newAdmin.name2,"");
        strcpy(newAdmin.status,"Admin");
		printf("Your userID is : %d\n",newAdmin.userID);
		write(fd,&newAdmin,sizeof(User));
		printf("Do you want to create another new admin account?\n\nIf yes : 1 or if no : 0 - ");
		scanf("%d",&choice);
	}
	close(fd);
	chooseOption();
}

int main(){
	chooseOption();
	return 0;
}