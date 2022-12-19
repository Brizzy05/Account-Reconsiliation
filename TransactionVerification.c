#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Brisnel Etou Bosseba
// 26096889

void errorMsg(char *msg, char *arg){
        printf("ErrorMessage: %s %s\n", msg, arg);
        printf("Program Syntax: ./tv STATE TRANSACTIONS\n");
        printf("Legal Usage:\n./tv state.csv transaction.csv\n./tv ../state.csv /data/log/transaction.csv\n");
}

void errorMsg2(char *msg, int acc, float sb, float eb){
        printf("ErrorMessage: %s %d %f %f\n", msg, acc, sb, eb);
}

struct ACCOUNT {
        int accountNumber;
        double startingBalance;
        double endingBalance;
        struct ACCOUNT *next;
};


 struct ACCOUNT* sortInsert(struct ACCOUNT* headnode, struct ACCOUNT* newNode){
        char *msg = "Duplicate account number [account, start, end]";
        struct ACCOUNT* curr;
        
        // if head is null
        if(headnode == NULL) {
                headnode = newNode;
                return headnode;
        }
        else if(headnode->accountNumber > newNode->accountNumber){
                
                newNode->next = headnode;
                headnode = newNode;
                return headnode;
        }
        else if(headnode->accountNumber == newNode->accountNumber){
                errorMsg2(msg, newNode->accountNumber, newNode->startingBalance, newNode->endingBalance);
                free(newNode);
                return headnode;
        }
        else {
                curr = headnode;
                
                while(curr->next != NULL && curr->next->accountNumber < newNode->accountNumber) { 
                                
                                curr = curr->next;
                }
                if(curr->next != NULL && curr->next->accountNumber == newNode->accountNumber){
                        errorMsg2(msg, newNode->accountNumber, newNode->startingBalance, newNode->endingBalance);
                        free(newNode);

                } else{
                        
                        newNode->next = curr->next;
                        curr->next = newNode;
                }
                return headnode;
                
        }

}

struct ACCOUNT* locateVal(struct ACCOUNT* head, int acc){
        
        struct ACCOUNT* node = NULL;
        struct ACCOUNT* tmp = head;

        while(tmp != NULL){
                if (tmp->accountNumber == acc){
                        node = tmp;
                        break;
                }
                tmp = tmp->next;
        }
        
        return node;
}

int main(int argc, char* argv[]){
        // check if you have all args
        if (argc != 3){
                errorMsg("Wrong number of Arguments", " ");
                exit(1);
        }
        // checks if file is readable
        FILE *file1 = fopen(argv[1],"rt");
        FILE *file2 = fopen(argv[2],"rt");

        if (file1 == NULL){
                errorMsg("Unable to open filename",argv[1]);
                fclose(file1);
                fclose(file2);
                exit(2);
        }
        if (file2 == NULL){
                errorMsg("Unable to open filename",argv[2]);
                fclose(file1);
                fclose(file2);
                exit(2);
        }
        
        // read state
        struct ACCOUNT* headState = NULL;

        char line[1024];
        char line2[1024];

        int accNum;
        double stBl, endBl;
        char mode;
        double absVal;

        
        // checks if  file empty

        if(fgets(line, 1024, file1) == NULL && fgets(line2, 1024, file2) != NULL){

                printf("ErrorMessage: File %s empty. Unable to validate %s\n", argv[1], argv[2]);
                free(headState);
                fclose(file1);
                fclose(file2);
                exit(3);
        }

        while(fgets(line, 1024, file1) != NULL){
            // do something similar strtok
                // new node
                struct ACCOUNT* newNode = (struct ACCOUNT*)malloc(sizeof(struct ACCOUNT));
                sscanf(line,"%d,%lf,%lf", &accNum, &stBl, &endBl);

                
                //printf("%d %lf %lf\n", accNum, stBl, endBl);
                newNode->accountNumber = accNum;
                newNode->startingBalance = stBl;
                newNode->endingBalance = endBl;

                headState =  sortInsert(headState, newNode);

        }
        puts("\n");
        fclose(file1);
        // test if working 
        struct ACCOUNT* tmp = headState;    
        // while(tmp != NULL){
        //         printf("%d\n", tmp->accountNumber);
        //         tmp = tmp->next;
        // }

        // reading Transaction File
        
        while(fgets(line2, 1024, file2) != NULL){
            // loop through the link list a find the account number
                char *msg = "ErrorMessage: Account not found (account, mode, amount):";
                char *msg2 = "ErrorMessage: Balance below zero error (account, mode, transaction, startingBalance before):";
                sscanf(line2,"%d,%c,%lf", &accNum, &mode, &absVal);

                struct ACCOUNT* account = locateVal(headState, accNum);

                if(account == NULL) printf("%s %d %c %f \n", msg, accNum, mode, absVal);
                else{   // change 
                        //printf("%d\n", account->accountNumber);
                        if(mode == 'w'){
                                account->startingBalance -= absVal;
                        } 
                        else if (mode == 'd'){
                                account->startingBalance += absVal;
                        }
                        if(account->startingBalance < 0){
                                printf("%s %d %c %f %f \n", msg2, accNum, mode, absVal, account->startingBalance);
                                account->startingBalance = 0;
                                
                        }
                }

        }
        puts("\n");
        fclose(file2);
        // making the stats
        char* msg3 = "ErrorMessage: End of day balances do not agree (account, starting, ending):";
        while(tmp != NULL){
                if(tmp->startingBalance != tmp->endingBalance){
                        printf("%s %d %f %f\n", msg3, tmp->accountNumber, tmp->startingBalance, tmp->endingBalance);
                }
                tmp = tmp->next;
        }
        
       struct ACCOUNT* curr = headState;

        while(curr != NULL){
                struct ACCOUNT* tmps = curr->next;
                free(curr);
                curr = tmp;
        }
}	

