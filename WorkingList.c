#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* CURRENT IDEA:
   numbers are stored using a singly linked list where the orders of magnitude are backwards

   ex. 1024 would be stored as [ 4, 2, 0, 1 ]

   it should be stored backwards because in order for a recursive function to carry a remainder
   to the next, higher order of magnitude place, it needs to traverse up the int, and since it's
   singly linked it would be best to store it in reverse order.
*/

//list to store numbers
struct list {
   int digit;
   struct list *next;
};
//datatype to hold numbers
struct num {
   struct list *head;
   struct list *tail;
};

//functions
bool isEmpty(struct list *); //passed : head
int length(struct list *, struct list *); //passed : head, tail
void insertFront(struct list **, struct list **, int); //passed : head, tail, digit
void insertBack(struct list **, struct list **, int); //passed : head, tail, digit
struct list * deleteFront(struct list **); //passed : head
void printList(struct list **, struct list **); //passed : head, tail
void free_list(struct list **, struct list **); //passed : head, tail
struct num * my_add(struct list **, struct list **, struct list **, struct list **); //passed : head,tail for num1, and head,tail for num2
struct num * my_sub(struct list **, struct list **, struct list **, struct list **); //passed : head,tail for num1, and head,tail for num2
bool isLargerThan(struct list **, struct list **, struct list **, struct list **); //passed : old head,head,tail for num1, and old head,head,tail for num2
struct num * dup_num(struct list **, struct list **); //passed : head, tail

int main(int argc, char * argv[])
{
   struct num *x, *y;
   x = (struct num*) malloc(sizeof(struct num));
   y = (struct num*) malloc(sizeof(struct num));
   x->head = x->tail = y->head = y->tail = NULL;

   //change count to change size of numbers
   int i = 0, count = 10;
   srand(time(NULL)); //so we get random digits each iteration
   for (i; i < count; i++)
   {
      insertFront(&x->head, &x->tail, (rand() % 10));
      insertBack(&y->head, &y->tail, (rand() % 10));
   }

   printf("\nOriginal Lists: ");

   //prints list
   printList(&x->head, &x->tail);
   printf("x Length: %d", length(x->head, x->tail));
   printList(&y->head, &y->tail);
   printf("y Length: %d", length(x->head, x->tail));

   //tests isLargerThan
   printf("\n\nis x larger than y?: ");
   printf("%s", isLargerThan(&x->head, &x->tail, &y->head, &y->tail) ? "true" : "false");
   printf("\n\nis y larger than x?: ");
   printf("%s", isLargerThan(&y->head, &y->tail, &x->head, &x->tail) ? "true" : "false");

   //tests dup
   struct num* resultDup;
   resultDup = dup_num(&x->head, &x->tail);
   printf("\n\nduplicate x =");
   printList(&resultDup->head, &resultDup->tail);  
   //adds lists
   struct num *resultAdd;
   resultAdd = my_add(&x->head,&x->tail,&y->head,&y->tail);
   printf("\n\nx + y =");
   printList(&resultAdd->head, &resultAdd->tail);
   printf("result Length: %d", length(resultAdd->head, resultAdd->tail));

   //subtracts lists
   struct num *resultSub;
   resultSub = my_sub(&x->head, &x->tail, &y->head, &y->tail);
   printf("\n\nx - y =");
   printList(&resultSub->head, &resultSub->tail);
   printf("result Length: %d", length(resultSub->head, resultSub->tail));
   printf("\n");
   //clears list
   free_list(&x->head, &x->tail);
   free_list(&y->head, &y->tail);
   free_list(&resultAdd->head, &resultAdd->tail);
   free_list(&resultSub->head, &resultSub->tail);
   free_list(&resultDup->head, &resultDup->tail);



   printf("\nLists after deleting all items:\nx: ");
   printList(&x->head, &x->tail);
   printf("y:");
   printList(&y->head, &y->tail);
   printf("\nresultAdd:");
   printList(&resultAdd->head, &resultAdd->tail);
   printf("\nresultSub:");
   printList(&resultSub->head, &resultSub->tail);

   free(x);
   free(y);
   free(resultAdd);
   free(resultSub);
   free(resultDup);

   return 0;
}

bool isEmpty(struct list *ptr)
{
   return ptr  == NULL;
}

int length(struct list *start, struct list *end)
{
   int length = 0;
   //if list is empty
   if(isEmpty(start)) {
      return 0;
   }
   struct list *current = start;

   while(current != end) {
      length++;
      current = current->next;
   }
   length++;

   return length;
}

//insert link at the first location
void insertFront(struct list **head, struct list **tail, int digit)
{

   //creates a link
   struct list *node = (struct list*) malloc(sizeof(struct list));
      node -> digit = digit;

   if (isEmpty(*head)) {
      *head = *tail = node;
      (*head) -> next = (*tail) -> next = NULL;
   } else {
      //point it to old first list
      node -> next = *head;

      //point first to new first list
      *head = node;
   }

}

void insertBack(struct list **head, struct list **tail, int digit)
{
   struct list *node = (struct list*) malloc(sizeof(struct list));
   node -> digit = digit;

   if (isEmpty(*head))
   {
      *head = *tail = node;
      (*head) -> next = (*tail) -> next = NULL;
   }
   else
   {
      //point it to NULL
      node -> next = NULL;

      //update old tail
      (*tail) -> next = node;
	   *tail = node;
   }
}

//delete first item
struct list * deleteFront(struct list **head)
{
   if(isEmpty((*head)->next)) {
	printf("\nDeleted value:(%d)", (*head) -> digit);
	free(head);
	*head = NULL;
      return NULL;
   }
   //save reference to first link
	struct list *old_head = *head;

   //mark next to first link as first
   *head = (*head) -> next;
	printf("\nDeleted value:(%d)", old_head -> digit);
   free(old_head);
   //return the new head
   return *head;
}

//display the list
void printList(struct list **head, struct list **tail)
{
   printf("\n[ ");

   //start from the beginning
   if(!isEmpty(*head)) {
      struct list *ptr = *head;
      while(ptr != *tail) {
         if(ptr->digit != -1)
         {
            printf("%d ", ptr -> digit);
         }
         else
         {
            printf("- "); //for subtraction method
         }
         ptr = ptr -> next;
      }
      if(ptr->digit != -1)
      {
         printf("%d ", ptr -> digit);
      }
      else
      {
         printf("- ");
      }
   }
   printf(" ]\n");
}


// destructor
void free_list(struct list **head, struct list **tail)
{
   while(!isEmpty(*head)) {
      struct list *temp = deleteFront(head);
   }
    printf("\n");
}

//note carry can only ever be one
struct num * my_add(struct list **head1, struct list **tail1, struct list **head2, struct list **tail2){

    int val;
    int carry = 0;
    struct list *ptr1 = *head1;              //just creating like a temp ptr to 1st number's head
    struct list *ptr2 = *head2;              //same for above but for 2nd number
    struct list *first_tail = *tail1;        //wouldnt work unless i created a temp ptr to the tail, wouldnt let me use original
    struct list *second_tail = *tail2;       //same for above but for 2nd number
    int num1,num2;                           //values for the digit's used to calculate the value
    struct num *result;                      //where we will store our answer
    result = (struct num*) malloc(sizeof(struct num));
    result->head = result->tail = NULL;       //initialized

    //this loop makes sure we still have elements
    while(ptr1 != first_tail && ptr2 != second_tail){
        //for the case where num2 has more digits than num1
        if(ptr1 == first_tail && ptr2 != second_tail){
            num1 = 0;
            num2 = ptr2->digit;
            ptr2 = ptr2->next;
        }
        //for the case where num1 has more digits than num2
        else if(ptr2 == second_tail && ptr1 != first_tail){
            num2 = 0;
            num1 = ptr1->digit;
            ptr1 = ptr1->next;
        }
        else{
            num1 = ptr1 -> digit;
            num2 = ptr2 -> digit;
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }

        val  = num1 + num2 + carry;  //calculating val
        if(val > 9){                 //if val is greater than 9 we will have a carry of 1, and we need to mod val to get
            carry = 1;               //the least sig bit ex. say we have 5+7==12, 12 mod 10 = 2 which we would insert
            val = val % 10;
            insertBack(&result->head, &result->tail, val);
        }else{                       //case where we can just insert and not have a carry
            carry = 0;
            insertBack(&result->head, &result->tail, val);
        }
    }

    val = ptr1->digit + ptr2->digit + carry;
    if(val > 9){
        carry = 1;
        val = val % 10;
        insertBack(&result->head, &result->tail, val);
        insertBack(&result->head, &result->tail, carry);
    }else{
        carry = 0;
        insertBack(&result->head, & result->tail, val);
    }

   //printList(&result->head, &result->tail);
   return result;
}

struct num * my_sub(struct list **head1, struct list **tail1, struct list **head2, struct list **tail2){
    bool swap = false;  //needed for if 2nd number is greater than 1st number
    int carry = 0;  //basically just a suedo remainder
    struct num *result;                      
    result = (struct num*) malloc(sizeof(struct num)); 
    result->head = result->tail = NULL;

    struct list *ptr1, *ptr2, *first_tail, *second_tail;
    if(isLargerThan(head1, tail1, head2, tail2))
    {
       ptr1 = *head1; first_tail = *tail1;
       ptr2 = *head2; second_tail = *tail2;
    }
    else //if the 2nd number is larger, set the temp pointers the opposite of normal note that in swap variable
    {
       ptr2 = *head1; second_tail = *tail1;
       ptr1 = *head2; first_tail = *tail2;
       swap = true;
    }
    
    //traverses through the list
    while(ptr1 != first_tail && ptr2 != second_tail)
    {
       //if 2nd number is larger
       if(ptr1 != first_tail && ptr2 == second_tail)
       {
          insertBack(&result->head, &result->tail, ptr1->digit + carry);
          carry = 0;
          ptr1 = ptr1->next;
       }
       //if 1st number is larger
       else if(ptr1 == first_tail && ptr2 != second_tail)
       {
          insertBack(&result->head, &result->tail, ptr2->digit + carry);
          carry = 0;
          ptr2 = ptr2->next;
       }
       //if the 1st number is greater than 2nd number, subtract
       else if(ptr1->digit >= ptr2->digit)
       {
          //mainly just an edge case
          if(carry == -1 && ptr1->digit == ptr2->digit)
          {
             insertBack(&result->head, &result->tail, (ptr1->digit - ptr2->digit + carry + 10));
             ptr1 = ptr1->next;
             ptr2 = ptr2->next;
          }
          else
          {
             insertBack(&result->head, &result->tail, (ptr1->digit - ptr2->digit + carry));
             carry = 0;
             ptr1 = ptr1->next;
             ptr2 = ptr2->next;
          }
       }
       //if 2nd number is greater than 1st number, +10 then subtract, note that in carry variable 
       else
       {
          insertBack(&result->head, &result->tail, (ptr1->digit + 10 - ptr2->digit + carry));
          carry = -1;
          ptr1 = ptr1->next;
          ptr2 = ptr2->next;
       }
    }

   //same steps as while loop just done for tail
   if(ptr1->digit >= ptr2->digit)
   {
      insertBack(&result->head, &result->tail, (ptr1->digit - ptr2->digit + carry));
   }
   else
   {
      insertBack(&result->head, &result->tail, (ptr1->digit + 10 - ptr2->digit + carry));
   }
   if(swap)
   {
      insertBack(&result->head, &result->tail, -1);
   }
   return result;
}

//needed for subtraction
bool isLargerThan(struct list **head1, struct list **tail1, struct list **head2, struct list **tail2)
{
    struct list *ptr1, *ptr2, *first_tail, *second_tail;
    ptr1 = *head1;
    ptr2 = *head2; 
    first_tail = *tail1;
    second_tail = *tail2;

    while(ptr1->next != first_tail && ptr2->next != second_tail)
    {
       //if statements in while loop check for equality amoung the last digits (since they're singly linked i had to choose how many to back-check; decide 3 digits is enough)
       if( (ptr1->next->next == first_tail && ptr2->next->next == second_tail) && ptr1->next->next->digit == ptr2->next->next->digit )
       {
          if(ptr1->next->digit == ptr2->next->digit)
          {
             if(ptr1->digit >= ptr2->digit)
             {
                return true;
             }
             return false;
          }
          else if(ptr1->next->digit > ptr2->next->digit)
          {
             return true;
          }
          return false;
       }

       ptr1 = ptr1->next;
       ptr2 = ptr2->next;
    }

    ptr1 = ptr1->next;
    ptr2 = ptr2->next;

    if(ptr1 == first_tail && ptr2 == second_tail)
    {
       if(ptr1->digit > ptr2-> digit)
       {
          return true;
       }
       return false;
    }

    else if(ptr1 == first_tail)
    {
       return false;
    }
    return true;
}

struct num * dup_num(struct list **head1, struct list **tail1)
{
   struct num * res = (struct num*) malloc(sizeof(struct num));
   res->head = res->tail = NULL;
   struct list *ptr1, *ptr2;
   ptr1 = *head1;
   ptr2 = *tail1;

   while(ptr1 != ptr2)
   {
      insertBack(&res->head, &res->tail, ptr1->digit);
      ptr1 = ptr1->next;
   }
   insertBack(&res->head, &res->tail, ptr1->digit);
   return res;
}
