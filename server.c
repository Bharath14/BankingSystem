#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>
#include "macros.h"
#include "user.h"

User get_user(int ID,int usertype);
bool validate(User currUser, int usertype);
bool depmoney(int usertype,int ID,float amount);
bool wdmoney(int usertype,int ID,float amount);
float get_balance(int usertype,int ID);
bool change_pass(int usertype,int ID,char newpassword[10]);
bool add_user(User record,int usertype);
bool delete_user(int ID,int usertype);
bool modifyUser(User modUser,int usertype);


void* client_Handler(void* s_fd)
{
	int sock_fd = *((int *)s_fd);
	int length_msg,select,type,usertype,user_id;
	bool result;
	while(1)
    {
		read(sock_fd,&usertype,sizeof(usertype));
        User currUser;
		read(sock_fd,&currUser,sizeof(User));
		printf("Username : %d\n",currUser.userID);
		printf("Password : %s\n",currUser.password);
		user_id=currUser.userID;
		result=validate(currUser,usertype);
		write(sock_fd,&result,sizeof(result));
		if(result)
        {
            	break;	
        }	
	}

	while(1){
		read(sock_fd,&select,sizeof(int));
		if(usertype==1 || usertype==2)
        {
			if(select==1)
            {
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=depmoney(usertype,user_id,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==2)
            {
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=wdmoney(usertype,user_id,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==3)
            {
				float amount;
				amount=get_balance(usertype,user_id);
				write(sock_fd,&amount,sizeof(float));
			}
			else if(select==4)
            {
				char password[10];
				read(sock_fd,password,sizeof(password));
				result=change_pass(usertype,user_id,password);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==5)
            {
                User user1=get_user(user_id,usertype);
				write(sock_fd,&user1,sizeof(User));
			}
			else if(select==6)
            {
                break;
            }
		}
		else if(usertype==3)
        {
			read(sock_fd,&type,sizeof(int));
			if(select==1)
            {
                User newUser1;
				read(sock_fd,&newUser1,sizeof(User));
				result=add_user(newUser1,type);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==2)
            {
                int delUserID1;
				read(sock_fd,&delUserID1,sizeof(int));
				result=delete_user(delUserID1,type);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==3)
            {
                User modUser1;
				read(sock_fd,&modUser1,sizeof(User));
				result=modifyUser(modUser1,type);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==4)
            {
                User searchUser1;
				int userid;
				read(sock_fd,&userid,sizeof(int));
				searchUser1=get_user(userid,type);
				write(sock_fd,&searchUser1,sizeof(User));
			}
			else
            {	
                break;
            }
		}
	}
	close(sock_fd);
	write(1,"Client session ended\n",22);
}

bool validate(User user, int type)
{

	int i,fd;
    if(type == 1)
    {
        i=user.userID-1000;
	    fd=open("NormalUsers",O_RDONLY,0744);
    }
    else if(type== 2)
    {
        i = user.userID - 2000;
	    fd=open("JointUsers",O_RDONLY,0744);
    }
    else if(type == 3)
    {
        i = user.userID - 3000;
	    fd=open("Administrators",O_RDONLY,0744);
    }
	
	bool result;
	User temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	    
	lock.l_len=sizeof(User);	            
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&temp,sizeof(User));
    if(type == 1 || type == 2)
    {
	    if(!strcmp(temp.password,user.password) && !strcmp(temp.status,"ACTIVE"))
        {	
            result=true;
        }
	    else
        {
				result=false;
        }
    }

    else if(type == 3)
    {
        if(!strcmp(temp.password,user.password))
        {	
            result=true;
        }
	    else
        {		
            result=false;
        }
    }

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depmoney(int type,int id,float amount)
{
	int i,fd;
	bool result;
	if(type==1)
    {
		i=id-1000;
		fd=open("NormalUsers",O_RDWR,0744);
		result = false;
	}
	else if(type==2)
    {
		i=id-2000;
		fd=open("JointUsers",O_RDWR,0744);
		result = false;
	}

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    
	lock.l_len=sizeof(User);	         
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);

	User user;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&user,sizeof(User));
		
	if(!strcmp(user.status,"ACTIVE"))
    {
		user.balance +=amount;
		lseek(fd,-sizeof(User),SEEK_CUR);
		write(fd,&user,sizeof(User));
		result=true;
	}
	else
    {
			result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;		
}

User get_user(int ID,int accType)
{
	int i, fd;
    if(accType == 1)
    {
        i = ID - 1000;
	    fd=open("NormalUsers",O_RDONLY,0744);
    }

    else if(accType == 2)
    {
        i = ID - 2000;
	    fd=open("JointUsers",O_RDONLY,0744);
    }

    else if(accType == 3)
    {
        i = ID - 3000;
	    fd=open("Administrators",O_RDONLY,0744);
    }
    
    User user;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);   
	lock.l_len=sizeof(User);	             
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);

	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&user,sizeof(user));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return user;
}
bool change_pass(int type,int id,char password[10])
{
	
	int i, fd;
	bool result;
	if(type==1)
    {
		i=id-1000;
		fd=open("NormalUsers",O_RDWR,0744);
	}
	else if(type==2)
    {
		i=id-2000;
		fd=open("JointUsers",O_RDWR,0744);
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);   
	lock.l_len=sizeof(User);	           
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	
	User user;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&user,sizeof(User));
		
	if(!strcmp(user.status,"ACTIVE"))
    {
		strcpy(user.password,password);
		lseek(fd,-sizeof(User),SEEK_CUR);
		write(fd,&user,sizeof(User));
		result=true;
	}
	else
    {
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool wdmoney(int type,int id,float amount){

	int i,fd;
	bool result;
	if(type==1)
    {
		i=id-1000;
		fd=open("NormalUsers",O_RDWR,0744);
	}
	else if(type==2)
    {
		i=id-2000;
		fd=open("JointUsers",O_RDWR,0744);
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    
	lock.l_len=sizeof(User);	           
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

	User user;
	lseek(fd,(i)*sizeof(User),SEEK_SET); 
	read(fd,&user,sizeof(User));
		
	if(!strcmp(user.status,"ACTIVE") && user.balance>=amount)
    {
		user.balance -=amount;
		lseek(fd,-sizeof(User),SEEK_CUR);
		write(fd,&user,sizeof(User));
		result=true;
	}
	else
    {	
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;	
}

float get_balance(int type,int id)
{
	float result = 0;
	int i,fd;
	if(type==1)
    {
		i=id-1000;
		fd=open("NormalUsers",O_RDONLY,0744);
	}
	else if(type==2)
    {
		i=id-2000;
		fd=open("JointUsers",O_RDONLY,0744);
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     
	lock.l_len=sizeof(User);	             
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

    User user;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&user,sizeof(User));
	if(!strcmp(user.status,"ACTIVE"))
    {
		result=user.balance;
	}
	else
    {					
		result=0;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool add_user(User newuser,int type)
{
    
	int fd;
    if(type == 1)
    {
        fd=open("NormalUsers",O_RDWR,0744);
    }
    else if(type == 2)
    {
        fd=open("JointUsers",O_RDWR,0744);
    }
    bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(User);    
	lock.l_len=sizeof(User);	         
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

	User user;
	lseek(fd,(-1)*sizeof(User),SEEK_END); 
	read(fd,&user,sizeof(User));
		
	newuser.userID=user.userID+1;
	newuser.account_no=user.account_no+1;
	strcpy(newuser.status,"ACTIVE");
	
	int x=write(fd,&newuser,sizeof(User));
	if(x!=0)
    {	
        result=true;
    }
	else
    {	
        result=false;
    }
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool delete_user(int id,int type)
{
	int i, fd;
	bool result = false;
    if(type == 1)
    {
        i=id-1000;
	    fd=open("NormalUsers",O_RDWR,0744);
    }

    else if(type == 2)
    {
        i=id-2000;
	    fd=open("JointUsers",O_RDWR,0744);
    }
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    
	lock.l_len=sizeof(User);	             
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

	User user;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&user,sizeof(User));
	
	if(!strcmp(user.status,"ACTIVE"))
    {	
		strcpy(user.status,"CLOSED");
		user.balance=0;
		
		lseek(fd,-sizeof(User),SEEK_CUR); 
		int x=write(fd,&user,sizeof(User));
		if(x!=0)
        {	
            result=true;
        }
		else
        {		
            result=false;
        }
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool modifyUser(User user,int type){
	int i, fd;
	bool result;
    if(type == 1)
    {
        i=user.userID-1000;
	    fd=open("NormalUsers",O_RDWR,0744);
    }

    else if(type == 2)
    {
        i=user.userID-2000;
	    fd=open("JointUsers",O_RDWR,0744);
    }

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);   
	lock.l_len=sizeof(User);	            
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	

	User temp;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  
	read(fd,&temp,sizeof(User));
	
	if(!(strcmp(temp.status,"ACTIVE")&&(user.account_no==temp.account_no)))
    {	
		strcpy(user.status,"ACTIVE");
		lseek(fd,-sizeof(User),SEEK_CUR); 
		int x=write(fd,&user,sizeof(User));
		if(x!=0)
        {	
            result=true;
        }
		else 
        {	
            result=false;
        }
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

int main()
{
    int server_fd, new_server_fd, opt=1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(server_addr);

    if(!(server_fd = socket(AF_INET, SOCK_STREAM, 0))){
        perror("server socket");
        exit(EXIT_FAILURE);
    }


    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("server setsocketopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("server bind");
        exit(EXIT_FAILURE);
    }

   
    if(listen(server_fd, MAX_CONNECTIONS) < 0){
        perror("server listen");
        exit(EXIT_FAILURE);
    }
  
    while(1)
    {
        new_server_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
        if(new_server_fd > 0)
        {
            pthread_t threadID;
            int* socket_ptr = malloc(sizeof(int));
            *socket_ptr = new_server_fd;

            pthread_create(&threadID, NULL, client_Handler, (void *)socket_ptr);
        }
    }
	pthread_exit(NULL);
    close(server_fd);
    return 0;
}




