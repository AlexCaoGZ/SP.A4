#include "../inc/serverHeader.h"

Node *createLL(){
    Node *head=(Node*)malloc(sizeof(*head));
    return head;
}

Node *insert(Node *head, Node *pNewNode){

    Node *p,*curr;
    p=head;

    if(strlen(head->msg1)==0){
        strcpy(head->msg1,pNewNode->msg1);
        strcpy(head->msg2,pNewNode->msg2);
    }

    while(p!=NULL){
        curr = p;
        p=p->next;
    }

    if(p==NULL){
        curr->next=pNewNode;
        pNewNode->next=NULL;
    }

    return head;
}


Node *deleteNode(Node *head){
    Node *p,*curr;
    p=head;
    if(head!=NULL){
        if(head->sent==TRUE){
            head=p->next;
            free(p);
        }
    }
    return head;
}