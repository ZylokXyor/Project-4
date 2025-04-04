#include <stdio.h>
#include <string.h>




// Function Declarations
void inc(register* head,int r,int k);
void dec(register* head, int r, int k);
void jump(register* head, int q, int k);
void initializeRegisters(int k);

typedef struct register_node 
{
int reg_num;
int count;
struct reg *next;
} Reg;

void inc(register* head,int r, int k) 
{
    if (r > k)
    {
        printf(stderr, "%s", "Register number is invalid");
    }
Reg* current = head;
while (current != NULL)
{
    if(current->reg_num == r)
    {
        current->count++;
    }
}
current = current->next;
}

void dec(register* head, int r, int k)
{
    if (r > k)
    {
       printf(stderr, "%s", "Register number is invalid");
    }
    Reg* current = head;
    while (current != NULL)
    {

    if(current->reg_num == r)
    {
        current->count--;
    }

    }
    current = current->next;

}


void jump(register* head, int q, int k)
{

Reg* current = head;

}

void initializeRegisters(int k) 
{
    
    Reg* prev = NULL;

for (int i = 0; i < k; i++)
{
Reg* newReg = (Reg*)malloc(sizeof(Reg));

if (newReg == NULL)
{
    printf(stderr, "%s", "Memory Allocation Failed. Exiting Program");
    exit(1);
} else
{
newReg->count = 0;
newReg->reg_num = i;
if (prev != NULL)
{
    prev->next = newReg;
}
prev = newReg;


}
}
}





int main()
{
Reg* head = NULL;
int r, q, k;




}
