#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include "macros.h"
#include "user.h"

int getNewNormalUserID();
int getNewJointUserID();
int getNewAdminUserID();
void chooseOption();
void Create_NormalUser(); 
void Create_JointUser();
void Create_Admin();
#endif