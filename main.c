s#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

/////////////////////////////////////////////////////////////////////////////////////
// Inore Inode Structure
/////////////////////////////////////////////////////////////////////////////////////
typedef struct inode
{
	unsigned int inodeNo;       //Inode No.
	unsigned int userId;		//User Id
	unsigned int groupId;		//Group Id
	unsigned int linkCount;	    //Link Count
	unsigned int referenceCount;  //Reference count
	unsigned int fileSize;	   //File Size
	char fileType[20];		   //File Type
	struct TEXTFILE *txtnext;        //Pointer to a Text File
	unsigned fileAccessPermission;   //File Access Permission
	struct inode  *next;		   //Pinter to next Inode Entry
}INODE;

/////////////////////////////////////////////////////////////////////////////////////
//File Table Structure
/////////////////////////////////////////////////////////////////////////////////////

typedef struct FileTable
{
	int cnt;			   //File Count
	int fileOffset;		   //File Offset
	int fileMode;			   //File Mode
	INODE  *inodeEntry;	           //Pointer to Inode Entry
	struct FileTable  *next;	   //Pointer to next File Table Entry
}FILETABLE;

/////////////////////////////////////////////////////////////////////////////////////
//UFDT Structure
/////////////////////////////////////////////////////////////////////////////////////

typedef struct UFDTable
{
	int fdIndex;			  //File Index i.e. File descriptor
	FILETABLE *fileTableEntry;      //Pointer to File Table Entry
	struct UFDTable *next;          //Pointer to next UFDT Entry
}UFDT;

/////////////////////////////////////////////////////////////////////////////////////
//Actual File Structure
/////////////////////////////////////////////////////////////////////////////////////

typedef struct TextFile
{
	char fileName[20];             //File Name
	char fileContent[1024];        //File Contents
}TEXTFILE;



/////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
	int totalBlock;    //Total No. of blocks in file system
	int usedBlock;     //Total No. of used blocks in file system
	int totalInode;    //Total No. of inodes in file system
	int usedInode;     //Total No. of used inodes in file system
}S;


// function prototype
int createInode(INODE **inode_second,FILETABLE **ft_second,UFDT **ufdt_second,char fname[],unsigned int perm);
void createFile(TEXTFILE **temp,char *fname);
void createFT(FILETABLE ***ft_third,UFDT ***ufdt_third,INODE *inode);
void createUFDT(UFDT ****ufdt_four,FILETABLE *ft);
void displayfd(UFDT *head);
void createman(char *name);

////////////////////////////////////////////////////////////////////////////////////
////create man command
/////////////////////////////////////////////////////////////////////////////////////
void createman(char *name)
{
	if(name==NULL)
		return;

	if(strcmp(name,"create")==0)
	{
		printf("\t**************************************************\n");
		printf("\tNAME        :create\n");
		printf("\tDESCIPTION  :Used to create new regular file. \n");
		printf("\tUSAGE       :Create File_name.\n");
		printf("\t**************************************************\n");
	}

	else if(strcmp(name,"read")==0)
	{
		printf("\t**************************************************\n");
		printf("\tNAME         :read\n");
		printf("\tDESCRIPTION  :Used to read regular file. \n");
		printf("\tUSAGE        :read File_name.\n");
		printf("\t**************************************************\n");

	}

	else if(strcmp(name,"write")==0)
	{
		printf("\t**************************************************\n");
		printf("\tNAME         :write\n");
		printf("\tDESCRIPTION  :Used to read regular file. \n");
		printf("\tUSAGE        :read File_name.\n");
		printf("\t**************************************************\n");

	}
	else if(strcmp(name,"ls")==0)
	{
		printf("\t**************************************************\n");
		printf("\tName       :ls\n");
		printf("\tDESCRIPTION:Used to list all information of file. \n");
		printf("\tUSAGE      :ls.\n");
		printf("\t**************************************************\n");

	}
	else if(strcmp(name,"ls-i")==0)
	{
		printf("\t**************************************************\n");
		printf("\tName        :ls -i\n");
		printf("\tDESCRIPTION :Used to list all information of file and inode.\n");
		printf("\tUSAGE       :ls command with paramter -i.\n");
		printf("\t**************************************************\n");

	}
	else if(strcmp(name,"stat")==0)
	{
		printf("\t**************************************************\n");
		printf("\tName       :stat\n");
		printf("\tDESCRIPTION:Used to display information of file.\n");
		printf("\tUSAGE      :stat File_Descriptor.\n");
		printf("\t**************************************************\n");

	}
	else
	if(strcmp(name,"delete")==0)
	{
		printf("\t**************************************************\n");
		printf("\tName        :delete\n");
		printf("\tDESCRIPTION :Used to close opened file. \n");
		printf("\tUSAGE       :close file_name.\n");
		printf("\t**************************************************\n");

	}
	else
	if(strcmp(name,"exit")==0)
	{
		printf("\t*************************************************\n");
		printf("\tName        :exit\n");
		printf("\tDESCRIPTION :Used to exit all. \n");
		printf("\tUSAGE       :exit.\n");
		printf("\t**************************************************\n");
	}
	else
	{
		printf("\t**************************************************\n");
		printf("\tERROR:NO mannual Entery available\n");
	}

}

/////////////////////////// INODE  //////////////////////////

int createInode(INODE **inode_second,FILETABLE **ft_second,UFDT **ufdt_second,char fname[],unsigned int perm)
{
	INODE *newnode=NULL,*itemp=NULL;
	TEXTFILE *txt1=NULL;
	static int icnt=0;
	if((S.usedInode<S.totalInode) && (S.usedBlock<S.totalBlock))
	{
		printf("\n INODE IS CREATEING in IIT.......\n");

		newnode=(INODE *) malloc (sizeof(INODE ));
		newnode->inodeNo=++icnt;
		newnode->userId=10;
		newnode->groupId=10;
		newnode->linkCount=1;
		newnode->referenceCount=1;
		newnode->fileSize=0;
		strcpy(newnode->fileType,"regular");
		newnode->fileAccessPermission=perm;

		printf("\n INODE is CREATED SUCCESSFULLY in IIT.\n");
		createFile(&txt1,fname);            // call to create file

		S.usedBlock++;  //One block is used for one file
		S.usedInode++;  //One Inode block is used for one Inode
		S.usedBlock++;  //One block is used for one Inode
		newnode->txtnext=(struct TEXTFILE *)txt1;
		newnode->next=NULL;
		newnode->fileSize=strlen(txt1->fileContent);
		newnode->next=NULL;

		if(*inode_second==NULL)         	//if IIT is empty
		(*inode_second)=newnode;
		else					//if IIT contain atleast 1 inode means IIT is not empty
		{
			for(itemp=*inode_second;itemp->next!=NULL;itemp=itemp->next);
				itemp->next=newnode;
		}

		createFT(&ft_second,&ufdt_second,newnode);
	}

	else
	{	printf("\nFile System has no enough memory");
		return -1;
	}
}
/////////////////////////////////////////////create textfile////////////////////////////////////////
void createFile(TEXTFILE **temp,char *fname)
{
	char ch={'\0'},buf[1024];
	int i=0;
	*temp=(TEXTFILE *) malloc (sizeof(TEXTFILE ));
	strcpy((*temp)->fileName,fname);
	printf("\n\t\tEnter the content :\n");
	printf("\n\t\tPlease enter ctrl+d to stop writing contents :-\n") ;

	while((ch=getchar())!= EOF )
	{
		buf[i++]=ch;
	}
	buf[i]='\0';

	printf("\n FILE IS CREATED\n");
	strcat((*temp)->fileContent,buf);
}
//////////////////////////////////// creat file table//////////////////////////////////////////////

void createFT(FILETABLE ***ft_third,UFDT ***ufdt_third,INODE *inode)
{
	FILETABLE *newnode=NULL,*ftemp=NULL;
	TEXTFILE *t1=NULL;
	printf("\n FILE TABLE IS CREATING....\n");
	newnode=(FILETABLE *) malloc (sizeof(FILETABLE ));
	newnode->cnt=1;
	newnode->fileOffset=0;
	newnode->fileMode=6;
	newnode->inodeEntry=inode;
	newnode->next=NULL;

	if(**ft_third==NULL)  		//if file table is empty
		**ft_third=newnode;
	else				//if file table is not empty
    {
		for(ftemp=(**ft_third);ftemp->next!=NULL;ftemp=ftemp->next);
		ftemp->next=newnode;
    }
    printf("\n FILE TABLE IS CREATED SUCCESSFULLY...\n");
	createUFDT(&ufdt_third,newnode);
}

///////////////////////////////////// creat UFDT/////////////////////////////////////////////
void createUFDT(UFDT ****ufdt_four,FILETABLE *ft)
{
	static int ind=3;
	UFDT *newnode=NULL,*utemp=NULL;
	newnode=(UFDT *)malloc(sizeof(UFDT ));
	newnode->fdIndex=ind++;
	newnode->fileTableEntry=ft;
	newnode->next=NULL;

	if(***ufdt_four==NULL) 			//if UFDT is empty
	{
		***ufdt_four=newnode;
		printf("\n USER FILE DISCRIPTOR IS INITIALISE...\n");
	}
	else 				// if UFDT is not empty
	{
		for(utemp=(***ufdt_four);utemp->next!=NULL;utemp=utemp->next);
		{
			utemp->next=newnode;
			printf("\n USER FILE DISCRIPTOR IS INITIALISE...\n");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void displayfd(UFDT *head)
{
	UFDT *ufdt=NULL;
	INODE *inode=NULL;
	FILETABLE *filetable=NULL;
	TEXTFILE *txtfile=NULL;
	printf("\n\t\tfile fd \t file name");
	for(ufdt=head;ufdt!=NULL;ufdt=ufdt->next)
	{
		filetable=(FILETABLE *)(ufdt->fileTableEntry);
		inode=(INODE *)(filetable->inodeEntry);
		txtfile=(TEXTFILE *)(inode->txtnext);
		printf("\n\t\t%d \t\t %s",ufdt->fdIndex,txtfile->fileName);
	}
}


///////////////////////HELPERFUNCTIONS///////////////////////////////////////////////
int read_x(int fd,UFDT *ufdt_second)
{
	INODE *inode=NULL;
	FILETABLE *ft=NULL;
	TEXTFILE *txt=NULL;
	char buf[1024]={'\0'};
	int flag=0,n=0;
	for(;ufdt_second!=NULL;ufdt_second=ufdt_second->next)
	{
	    ft=(FILETABLE *)(ufdt_second->fileTableEntry);
	    inode=(INODE *)(ft->inodeEntry);
	    txt=(TEXTFILE *)(inode->txtnext);
	   	if(ufdt_second->fdIndex==fd )
	    {
	   		flag=1;
	        break;
	    }
	}

  	if(flag==0)
	{
	    printf("\n\t\twrong file descriptor");
	    return -1;
  	}

    if((inode->fileAccessPermission==744)||(inode->fileAccessPermission==766))
	{
        printf("\nhow many byte of data u want to see\n\n");
	    scanf("%d",&n);
        if(n<0)
        {
            printf("\nFile size should be positive.");
            return -1;
        }
        if(inode->fileSize<n)      ///if given no is max than file size
        {
            strcpy(buf,txt->fileContent);
	   	    printf("\n\t\t File content are :-");
            printf("\n\t\t\t %s",buf);
            return inode->fileSize;
        }
	    printf("\n\t\t File content are :-");  ///if file size nd given no is equeal or less
        strncpy(buf,txt->fileContent,(n+1));
        printf("\n\t\t\t %s",buf);
        return n;
    }
   	else
    {
        printf("\n\t\t you have not access to read this file");
        return -1;
    }

}

///////////////////////////
int write_x(int fd,UFDT *ufdt_second)
{
    INODE *inode=NULL;
    FILETABLE *ft=NULL;
    TEXTFILE *txt=NULL;
	int flag=0,ch,n=0,i=0;
    char buf[1024],c='\0';

    for(;ufdt_second!=NULL;ufdt_second=ufdt_second->next)
  	{
		ft=(FILETABLE *)(ufdt_second->fileTableEntry);
        inode=(INODE *)(ft->inodeEntry);
        txt=(TEXTFILE *)(inode->txtnext);
   	 	if(ufdt_second->fdIndex==fd )
      	{
      	   flag=1;
           break;
        }
    }

    if(flag==0)
  	{
	   printf("\n\t\t\twrong file descriptor");
       return -1;
    }
    if((inode->fileAccessPermission==722)||(inode->fileAccessPermission==766))
    {
	    printf("\n\t\tDo you want to\n\t\t 1.Overwrite the file\n \t\t 2.Append to the file\n");
	    scanf("%d",&ch);
	    if((ch>2) && (ch<1))
        {
            printf("\n\t\t Wrong choice");
            return -1;
        }
	    printf("\n\t\tEnter Contents you want to write to the file :");
		printf("\n\t\tPlease enter ctrl+d to stop writing contents :-\n") ;
        while((c=getchar())!=EOF)
	 	{
			buf[i++]=c;
	  	}
	  	buf[i]='\0';

        switch(ch)
 		{
			case 1: strcpy(txt->fileContent,buf);
					inode->fileSize=strlen(txt->fileContent);
					return strlen(txt->fileContent);

			case 2: if(strlen(txt->fileContent)+strlen(buf)>1024)
	        		{
					    n=1024-strlen(txt->fileContent);
				        strncpy(txt->fileContent,buf,n);
				        return n;
				    }
				    strcat(txt->fileContent,buf);
					inode->fileSize=strlen(txt->fileContent);
					return strlen(buf);
        }
    }
    else
    {
        printf("\n\t\tAccess Denied");
        return -1;
    }
}
////////////////////////// showing file name like ls command /////////////////////////////////////////

void display_ls(UFDT *ufdt_second)
{
	INODE *inode=NULL;
	FILETABLE *ft=NULL;
	TEXTFILE *txt=NULL;
	UFDT *ufdt;
	for(ufdt=ufdt_second;ufdt!=NULL;ufdt=ufdt->next)
	{
		ft=(FILETABLE *)(ufdt->fileTableEntry);
		inode=(INODE *)(ft->inodeEntry);
		txt=(TEXTFILE *)(inode->txtnext);
		printf("\n\t\t%s",txt->fileName);
	}
}

void displayl_si(UFDT *ufdt_second)
{
	INODE *inode=NULL;
	FILETABLE *ft=NULL;
	TEXTFILE *txt=NULL;
	UFDT *ufdt;
	printf("\n\t\tFile Name  Inode No.");
	for(ufdt=ufdt_second;ufdt!=NULL;ufdt=ufdt->next)
	{
		ft=(FILETABLE *)(ufdt->fileTableEntry);
		inode=(INODE *)(ft->inodeEntry);
		txt=(TEXTFILE *)(inode->txtnext);

		printf("\n\t\t  %s\t  %d ",txt->fileName,inode->inodeNo);

 	}
}

void stat_x(int fd,UFDT *ufdt_second)
{
	INODE *inode=NULL;
	FILETABLE *ft=NULL;
	TEXTFILE *txt=NULL;
	UFDT *ufdt=NULL;

	int flag=0;

    for(;ufdt_second!=NULL;ufdt_second=ufdt_second->next)
  	{
		ft=(FILETABLE *)(ufdt_second->fileTableEntry);
		inode=(INODE *)(ft->inodeEntry);
		txt=(TEXTFILE *)(inode->txtnext);
		if(ufdt_second->fdIndex==fd )
		{
      		flag=1;
            break;
        }
    }

    if(flag==0)
  	{
	   printf("\n\t\t\twrong file descriptor");
       return ;
    }

	printf("\n\t\t\tFile Information are:-\n\t------------------------------------------------------\n");
	printf("\n\t\t| file descriptor       :%d\t\t|",ufdt_second->fdIndex);
  	printf("\n\t\t| File Name             :%s\t\t|",txt->fileName);
  	printf("\n\t\t| User id               :%d\t\t|",inode->userId);
  	printf("\n\t\t| Group id              :%d\t\t|",inode->groupId);
  	printf("\n\t\t| File inode no.        :%d\t\t|",inode->inodeNo);
  	printf("\n\t\t| Link count            :%d\t\t|",inode->linkCount);
  	printf("\n\t\t| reference count       :%d\t\t|",inode->referenceCount);
  	printf("\n\t\t| File Size             :%d\t\t|",inode->fileSize);
  	printf("\n\t\t| File Type             :%s\t\t|",inode->fileType);
  	printf("\n\t\t| File Access Permission:%u\t\t|",inode->fileAccessPermission);
  	printf("\n\t\t| file current offset   :%d\t\t|",ft->fileOffset);

  	printf("\n\t------------------------------------------------------\n");
  }

/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////  DELETE FILE  ////////////////////////////////////////////////
void Delete_Text(TEXTFILE **temp)
{
	free(*temp);
}


void Delete_Inode(INODE ***ihead,INODE **del)
{
 	INODE *temp;
	if((*del)==(**ihead))
	{
		**ihead=(**ihead)->next;
		free(*del);
		return;
	}

	for(temp=**ihead;temp->next!=(*del);temp=temp->next);
		temp->next=temp->next->next;

	free(*del);
	//temp->inodeNo--;
	return;
}

void Delete_FT(FILETABLE ***fhead,FILETABLE **del)
{
	FILETABLE *temp;
	if((*del)==(**fhead))
	{
		**fhead=(**fhead)->next;
		free(*del);
		return;
	}

	for(temp=**fhead;temp->next!=(*del);temp=temp->next);
		temp->next=temp->next->next;
	free(*del);
	return;
}

void Delete_UFDT(UFDT ***uhead,UFDT **del)
{
	UFDT *temp;
	if((*del)==(**uhead))
    {
		**uhead=(**uhead)->next;
		free(*del);
		return;
   }

	for(temp=**uhead;temp->next!=(*del);temp=temp->next);
		temp->next=temp->next->next;
	free(*del);
	return;
}


void unlink_x(INODE **inode_second,FILETABLE **ft_second,UFDT **ufdt_second,int fd)
{
 	UFDT *ufdt=NULL;
	INODE *inode=NULL;
	FILETABLE *ft=NULL;
	TEXTFILE *txt=NULL;
	int ch,flag=0;


	for(ufdt=*ufdt_second;ufdt!=NULL;ufdt=ufdt->next)
	{
	   if(ufdt->fdIndex==fd)
	    {
			flag=1;
   			ft=(FILETABLE *)(ufdt->fileTableEntry);
	        inode=(INODE *)(ft->inodeEntry);
			txt=(TEXTFILE *)(inode->txtnext);
	         break;
         }
    }
	if(flag==0)
	{
		printf("\n\t\tInvalid File descriptor !!");
		return;
	}

	printf("\n\tDo you want to delete the file : %s\n\tpress 1 for yes / press 0 for no\n:",txt->fileName);
	scanf("%d",&ch);
	if(ch==1)
	{
		  printf("deleting the actual data\n");
		  Delete_Text(&txt);
		  printf("removing the inode entry from IIT\n");
		  Delete_Inode(&inode_second,&inode);
		  printf("removing the file table entry\n");
		  Delete_FT(&ft_second,&ft);
		  printf("removing the UFDT entry\n");
		  Delete_UFDT(&ufdt_second,&ufdt);

		printf("now deallocating use block and use inode the \n");
       	S.usedInode--;  //One Inode block is used for one Inode
		S.usedBlock--;  //One block is used for one Inode
		S.usedBlock--;  //One block is used for one File
       printf("\n\t\tFile has been deleted .");
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
  {
	char fname[255]={'\0'},ffname[20]={'\0'},pass[20]={'\0'},cmd[10],yn;
	int ch,ch1,no,fd,fd1,ino;
	unsigned int perm;

	INODE *inode_first=NULL;
	FILETABLE *ft_first=NULL;
	UFDT *ufdt_first=NULL;


    printf("\n\nEnter password > ");
    scanf("%s",pass);
    //here define password.current password is "sagar".
	if((strcmp(pass,"sagar")) != 0)
	{
		printf("invalid password\n");
		exit(0);
	}


	S.totalBlock=1024;   //super block cha instance S (initialise super block)
	S.usedBlock=0;		 // Used data Block
	S.totalInode=1024;	 //Total Inode Block
	S.usedInode=0;		 //Used Inode Block

		    printf("\t//////////////////////////////////////////////////\n");
		    printf("\t//                                              //\n");
		    printf("\t//      project name : Virtual File System      //\n");
		    printf("\t//                                              //\n");
   		    printf("\t//      Created By : Dere Sagar 			      //\n");
 		    printf("\t//                   		              //\n");
		    printf("\t//                                              //\n");
		    printf("\t//////////////////////////////////////////////////\n");


	while(1)
	{
    labelm:	printf("\n\t-----------------Menu--------------------\n");
			printf("\t1.creat - use for create new file\n");
			printf("\t2.read  - use for reading content of file exsiting file \n");
			printf("\t3.write - use for writing data on exsiting file\n");
			printf("\t4.ls	  - list the File with File Descriptor\n");
			printf("\t5.ls-i  - list the file with Inode Number\n");
			printf("\t6.stat  - Shows basic Inforamtion of file\n");
			printf("\t7.delete- Delete exsiting file \n");
			printf("\t8.man   - Mannual page\n");
			printf("\t9.exit  - exit from File System\n");
			printf("\t10.ALL  - List All Inbuilt Command\n");


	label:	printf("\n\tEnter your choice :");
	       	while(scanf("%d",&ch) != 1)        //validation only take integer
			{
				printf("\tPlease enter valid choice:");
				while(getchar()!= '\n');
			}
	        if(ch <1 || ch >10)
	        {
	        	printf("\tPlease enter valid choice:");
	        	goto label;
	        }
	       switch(ch)
		{
		     case 1:  //To create new file
	      		label1 	:	printf("\n\t\tEnter file name :");
					scanf("%s",fname);
					if(fname[0] >= '0' && fname[0] <= '9' || fname[0] == ' ')
					{
						printf("Please Enter valid Name\n");
						goto label1;
					}
			        do
		              {
						label2 :printf("\n\t\tWhat are permissins to your file  :");
								printf("\n\t\t1.read   2.write  3.both read and write  :");
								while(scanf("%d",&ch1) != 1 )
								{
									printf("Please enter valid permission:\n");
									while(getchar()!= '\n');
								}
							    if(ch1 <1 || ch1 >4)
							    {
							        printf("Please enter valid permission:\n");
							        goto label2;
		  						}

								switch(ch1)
								{
								   case 1:	perm=744;
									   		fd1=createInode(&inode_first,&ft_first,&ufdt_first,fname,perm);
									   		if(fd1==-1)
									   		{
												printf("Unable to Create File");
									   		}
									   		break;
								   case 2:	perm=722;
									   		fd1=createInode(&inode_first,&ft_first,&ufdt_first,fname,perm);
									   		if(fd1==-1)
									   	 	{
												printf("Unable to Create File");
									   		}
									   		break;
								   case 3:	perm=766;
									   		fd1=createInode(&inode_first,&ft_first,&ufdt_first,fname,perm);
									   		if(fd1 == -1)
									   	 	{
									   			printf("Unable to Create File");
									   		}
									   		break;
				  					 default:printf("\n\t\tPlease enter valid permission");
	  		        			}
		      	     }while((ch1>3)||(ch1<=0));

			    	 break;
				case 2:	if(ufdt_first==NULL)
						{
							printf("\n File is not present \n");
							break;
						}
						displayfd(ufdt_first);
						printf("\n\tEnter file descriptor :");
						scanf("%d",&fd);
					 	no=read_x(fd,ufdt_first);
						if(no==-1)
						     printf("\n\t\tread failed\n");
						else if(no==0)
							printf("file is empty\n");
						  else
						  printf("\n\t\tNo. of characters successfully read : %d",no);

						break;
			case 3:
			  	if(ufdt_first==NULL)
				{
					printf("\n \t\t File system does not have any file !!!");
					break;
				}
				  displayfd(ufdt_first);
				  printf("\n\tEnter file descriptor :");
				  scanf("%d",&fd);
				  no=write_x(fd,ufdt_first);
				 if(no==-1)
				     printf("\n\t\twrite failed");
				 else
				    printf("\n\t\tNo. of characters successfully written :%d",no);
		 		  break;
			case 4:
			  	if(ufdt_first==NULL)
				{
					printf("\n \t\t File system does not have any file !!!");
					break;
				}
   		                display_ls(ufdt_first);
			        break;
 			case 5:
			  	if(ufdt_first==NULL)
				{
					printf("\n \t\t File system does not have any file !!!");
					break;
				}
		               displayl_si(ufdt_first);
			       break;
			 case 6:
			  	if(ufdt_first==NULL)
				{
					printf("\n \t\t File system does not have any file !!!");
					break;
				}
				   displayfd(ufdt_first);
				   printf("\n\tEnter desciptor number :");
				   scanf("%d",&fd);


				   stat_x(fd,ufdt_first);
				   break;
			case 7:
			  	if(ufdt_first==NULL)
				{
					printf("\n \t\t File system does not have any file !!!");
					break;
				}
				   displayfd(ufdt_first);
				   printf("\n\tEnter file descriptor :");
				   scanf("%d",&fd);
				   unlink_x(&inode_first,&ft_first,&ufdt_first,fd);
				   break;
			case 8:	printf("\n\t\tEnter the command :");
			   		scanf("%s",cmd);
					createman(cmd);
					break;
			case 9:
			  		if(ufdt_first==NULL)
					{
						printf("No any File Created In File System\n");
						exit(0);
					}
					printf("\tDo you want Really exit from File System\n\tIf Press YES-Y and NO-N \n");
				  	yn=getchar();
				  	yn=getchar();
				  	if(yn == 'Y' || yn == 'y')
				  	{
				  		printf("\t||///////////////////////////////////////////////////////////////////////////////||\n");
				  		printf("\t|| File content is not save because in this memory data not stored permenantally ||\n");
				  		printf("\t|| now all files gets erased after closing the process                           ||\n");
				  		printf("\t||///////////////////////////////////////////////////////////////////////////////||\n\n");
				  		printf("\t||--------------------------------THANK YOU--------------------------------------||\n");
				  		exit(0);
				  	}



			case 10:
			  					
				printf("\t||///////////////////////////////////////////////////////////////////////////////||\n");
				printf("\t||1- cp \t 2- mv	 ||\n");
				printf("\t||Enter your Choice: ||\n");
				scanf("%d",&ino);
				printf("\t||///////////////////////////////////////////////////////////////////////////////||\n\n");
					
	if(ino==1)
  	{
		printf("\t**************************************************\n");
		printf("\tName        :cp\n");
		printf("\tDESCRIPTION :Used to Copies sourcefile to targetfile\n");
		printf("\t**************************************************\n");

	}			
	else if(ino==2)
	{
		printf("\t**************************************************\n");
		printf("\tName        :mv\n");
		printf("\tDESCRIPTION :Copies sourcefile to targetfile then deletes the original sourcefile.\n");
		printf("\t**************************************************\n");

	}
			
	else{
		exit(0);		
		}

		
			default:printf("\n\t\twrong choice");
					break;
		}
	}
}
