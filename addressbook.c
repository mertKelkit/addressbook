#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAILURE -1
#define MODE_1 1
#define MODE_2 2


typedef struct a
{
    int id;
    char firstName[15];
    char lastName[15];
    long double phoneNum;
    char address[100];
} Record;

void menu(void);
void list(void);
void getInfo(void);
void add(Record*);
int countRecords(void);
void selectionSort(Record*);
void update(void);
int checkExistence(int);
void updatePhoneNumber(int, long double);
void updateAddress(int, char*);
void writeToTextFile(Record*);
void delete(void);
void deleteID(int);
void deleteProcess(Record*, int);
int checkID(char*);
char* checkName(char*);
long double checkNumber(char*);
int checkChoice(char*, int mode);
int myPow(int, int);
void getCurrentList(Record*);


int recordCounter = 0;

int main()
{
    printf("\t\tADDRESS BOOK\n\n----------------------------");
    menu();
    return 0;
}

void menu()
{
    char choice[10];
    while (1)
    {
        printf("\nPlease enter a valid choice\n----------------------------\n");
        printf("1 -> List All Records\n2 -> Update a Record\n3 -> Create a New Record\n4 -> Delete a record\n5 -> Exit\n");
        do
        {
            scanf("%s", choice);
        } while(checkChoice(choice, MODE_1) == FAILURE);
        if(!strcmp(choice, "1"))
            list();
        else if(!strcmp(choice, "2"))
            update();
        else if(!strcmp(choice, "3"))
            getInfo();
        else if(!strcmp(choice, "4"))
            delete();
        else if(!strcmp(choice, "5"))
        {
            printf("Bye !");
            return;
        }
    }
}

void list()
{
    if (countRecords())
    {
        printf("ID\tFirst Name\tLast Name\tPhone Number\tAddress\n==\t==========\t==========\t============\t=======\n");
        Record r[recordCounter];
        getCurrentList(r);
        int i;
        selectionSort(r);
        for (i = 0; i < recordCounter; i++)
            printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
    }
}

void getInfo()
{
    char tID[10], tFirst[15], tLast[15], tNum[20], tAddress[50];
    Record *r = (Record*)malloc(sizeof(Record));
    printf("Enter an ID for new contact: ");
    do
    {
        scanf("%s", tID);
    } while(checkID(tID) == FAILURE);
    r -> id = checkID(tID);
    printf("Enter a first name for new contact: ");
    do
    {
        scanf("%s", tFirst);
    } while(!checkName(tFirst));
    strcpy(r -> firstName, checkName(tFirst));
    printf("Enter a last name for new contact: ");
    do
    {
        scanf("%s", tLast);
    } while(!checkName(tLast));
    strcpy(r -> lastName, checkName(tLast));
    printf("Enter phone number of new contact: ");
    do
    {
        scanf("%s", tNum);
    } while(checkNumber(tNum) == FAILURE);
    r -> phoneNum = checkNumber(tNum);
    printf("Enter the address of new contact: ");
    scanf("%s", r -> address);
    add(r);
}

void add(Record* r)
{
    FILE* file;
    if(file = fopen("/home/mertkelkit/CLionProjects/AddressBook/records", "a"))
    {
        if(recordCounter == 0)
            fprintf(file, "%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
        else
            fprintf(file, "\n%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
        fclose(file);
        printf("New record added.\n");
        recordCounter++;
    }
    else
        printf("Address Book cannot found !");
}

int countRecords()
{
    recordCounter = 0;
    FILE *file;
    int len;
    if (file = fopen("/home/mertkelkit/CLionProjects/AddressBook/records", "r"))
    {
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        if(len == 0)
        {
            printf("Address Book is Empty !\n");
            return 0;
        }
        rewind(file);
        while (!feof(file))
        {
            Record *rec = (Record *)malloc(sizeof(Record));
            fscanf(file, "%d %s %s %Lf %s", &rec->id, rec->firstName, rec->lastName, &rec->phoneNum, rec->address);
            recordCounter++;
        }
        fclose(file);
    }
    else
    {
        printf("Address Book Cannot Found !");
        return 0;
    }
    return 1;
}

void selectionSort(Record* r)
{
    int i, j;
    for(i=0; i<recordCounter-1; i++)
    {
        for(j=i+1; j<recordCounter; j++)
        {
            if(strcmp(r[i].lastName, r[j].lastName) > 0)
            {
                Record temp = r[i];
                r[i] = r[j];
                r[j] = temp;
            }
            else if(strcmp(r[i].lastName, r[j].lastName) == 0)
            {
                if(strcmp(r[i].firstName, r[j].firstName) > 0)
                {
                    Record temp = r[i];
                    r[i] = r[j];
                    r[j] = temp;
                }
            }
        }
    }
}

void update()
{
    int id;
    if (countRecords())
    {
        printf("Enter id for the record which you want to update: ");
        scanf("%d", &id);
        if (checkExistence(id))
        {
            char choice[10];
            int cInt;
            long double newPhoneNum;
            char newAddress[100];
            char tPhoneNum[20];
            printf("\n1 -> Update phone number\n2 -> Update address\nEnter your choice: ");
            do
            {
                scanf("%s", choice);
            } while(checkChoice(choice, MODE_2) == FAILURE);
            cInt = checkChoice(choice, MODE_2);
            switch(cInt)
            {
                case 1:
                    printf("Enter new phone number: ");
                    do
                    {
                        scanf("%s", tPhoneNum);
                    } while(checkNumber(tPhoneNum) == FAILURE);
                    newPhoneNum = checkNumber(tPhoneNum);
                    updatePhoneNumber(id, newPhoneNum);
                    break;
                case 2:
                    printf("Enter new address: ");
                    scanf("%s", newAddress);
                    updateAddress(id, newAddress);
                    break;
            }
        }
    }
}

int checkExistence(int id)
{
    FILE *file;
    if(file = fopen("/home/mertkelkit/CLionProjects/AddressBook/records", "r"))
    {
        while(!feof(file))
        {
            Record *temp = (Record*)malloc(sizeof(Record));
            fscanf(file, "%d %s %s %Lf %s", &temp -> id, temp -> firstName, temp -> lastName, &temp -> phoneNum, temp -> address);
            if(temp -> id == id)
            {
                printf("Record is found !\n");
                printf("%d%12s%12s%14.0Lf\t\t%s\n", temp -> id, temp -> firstName, temp -> lastName, temp -> phoneNum, temp -> address);
                return 1;
            }
        }
        printf("Record cannot found !\n");
        return 0;
    }
    else
        printf("Address Book cannot found !\n");
    return 0;
}

void updatePhoneNumber(int id, long double newNumber)
{
    Record r[recordCounter];
    getCurrentList(r);
    int i;
    for(i=0; i<recordCounter; i++)
    {
        if(r[i].id == id)
        {
            r[i].phoneNum = newNumber;
            break;
        }
    }
    writeToTextFile(r);
    printf("Updated successfuly !\n");
    printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
}

void updateAddress(int id, char *newAddress)
{
    Record r[recordCounter];
    getCurrentList(r);
    int i;
    for(i=0; i<recordCounter; i++)
    {
        if(r[i].id == id)
        {
            strcpy(r[i].address, newAddress);
            break;
        }
    }
    writeToTextFile(r);
    printf("Updated successfuly !\n");
    printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
}

void writeToTextFile(Record *r)
{
    FILE *file;
    int i;
    if(file = fopen("/home/mertkelkit/CLionProjects/AddressBook/records", "w"))
    {
        for(i=0; i<recordCounter; i++)
        {
            if(i == 0)
                fprintf(file, "%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
            else
                fprintf(file, "\n%d\t%s\t%s\t%.0Lf\t%s", r -> id, r -> firstName, r -> lastName, r -> phoneNum, r -> address);
            r++;
        }
        fclose(file);
    }
    else
        printf("Address Book cannot found !");
}

void delete()
{
    int id;
    if(countRecords())
    {
        printf("Enter id for the record which you want to delete: ");
        scanf("%d", &id);
        deleteID(id);
    }
}

void deleteID(int id)
{
    Record r[recordCounter];
    getCurrentList(r);
    int i;
    for(i=0; i<recordCounter; i++)
    {
        if(r[i].id == id)
        {
            printf("%d%12s%12s%14.0Lf\t\t%s\n", r[i].id, r[i].firstName, r[i].lastName, r[i].phoneNum, r[i].address);
            break;
        }
    }
    if(i == recordCounter)
        printf("\nRecord cannot found !\n");
    else
        deleteProcess(r, i);
}

void deleteProcess(Record *r, int index)
{
    int j;
    Record *newR = (Record*)malloc(sizeof(Record)*(recordCounter-1));
    for(j=0; j<recordCounter; j++)
    {
        if(j != index)
            newR[j] = *r;
        r++;
    }
    printf("Deleted successfuly !\n");
    recordCounter--;
    writeToTextFile(newR);
}

int checkID(char* id)
{
    int i;
    int val = 0;
    double ctr = 0;
    for(i=0; i<strlen(id); i++)
    {
        if(*(id+i) < 48 || *(id+i) > 57)
        {
            printf("ID must be an integer. Please enter again: ");
            return FAILURE;
        }
    }
    for(i = strlen(id)-1; i>=0; i--)
        val += (*(id+i)-48) * myPow(10, ctr);
    Record r[recordCounter];
    getCurrentList(r);
    for(i=0; i<recordCounter; i++)
    {
        if(r[i].id == val)
        {
            printf("This ID already exists. Enter a new one: ");
            return FAILURE;
        }
    }
    return val;
}

char* checkName(char *name)
{
    int i;
    for(i=0; i<strlen(name); i++)
    {
        if(name[i] >= 48 && name[i] <= 57)
        {
            printf("First Name/Last Name cannot contain an integer. Please enter again: ");
            return NULL;
        }
    }
    return name;
}

long double checkNumber(char *number)
{
    int i, ctr = 0;
    long double val = 0;
    for(i=strlen(number)-1; i>=0; i--)
    {
        if(number[i] < 48 || number[i] > 57)
        {
            printf("Phone Number must be a number. Please enter again: ");
            return FAILURE;
        }
        val += (number[i]-48)*(long double)myPow(10, ctr);
        ctr++;
    }
    if(strlen(number) < 5)
    {
        printf("Number is too short ! Please enter again: ");
        return FAILURE;
    }
    return val;
}

int checkChoice(char* c, int mode)
{
    int i, ctr = 0;
    int val = 0;
    for(i=strlen(c)-1; i>=0; i--)
    {
        if (mode == MODE_1)
        {
            if (c[i] < 49 || c[i] > 53)
            {
                printf("Your choice must be a valid integer. Enter again: ");
                return FAILURE;
            }
        }
        else if(mode == MODE_2)
        {
            if(c[i] < 49 || c[i] > 50)
            {
                printf("Your choice must be a valid integer. Enter again: ");
                return FAILURE;
            }
        }
        val += myPow(10, ctr);
        ctr++;
    }
    return val;
}


int myPow(int base, int power)
{
    int i;
    int val = 1;
    for(i=0; i<power; i++)
        val *= base;
    return val;
}

void getCurrentList(Record r[recordCounter])
{
    FILE *file;
    int i = 0;
    if(file = fopen("/home/mertkelkit/CLionProjects/AddressBook/records", "r"))
    {
        while(!feof(file))
        {
            fscanf(file, "%d %s %s %Lf %s", &r[i].id, r[i].firstName, r[i].lastName, &r[i].phoneNum, r[i].address);
            i++;
        }
        fclose(file);
    }
    else
        printf("Address Book cannot found !\n");
}
