#include <stdio.h>

struct memoria
{
  int instrucao;
  int operando;//operando;  
};

struct memoria mem[128]; 

int pc;

int R0, R1, PC=0,ACC;

void loader() //Ler arquivo
{
     FILE * f ;
     char nomeArq [30] ;
     int i=0;
     printf("digite o nome do arquivo\n") ;
     scanf("%s", nomeArq) ;
     f = fopen (nomeArq, "r") ;
     int cont;
     fscanf (f, "%d%d", &mem[i].instrucao, &mem[i].operando);
     cont=mem[i].instrucao;
     i=cont+1;
     while (!feof(f)) {
     fscanf (f, "%d%d", &mem[i].instrucao, &mem[i].operando);
     printf("memoria= %i",i);
     printf(" OP= %d OPP= %d\n", mem[i].instrucao, mem[i].operando);
     
     i++;
    }
    PC=cont+1;
    fclose(f); 
}

void cpu()
{   int final=0;
    
    
    while(final==0)
    {
        
        switch(mem[PC].instrucao)
        {
         
         case 0://load
             ACC=mem[mem[PC].operando].operando;
             PC++;
             break;     
         
         case 1://store
             mem[mem[PC].operando].operando=ACC;
             PC++;
             break;
         
         case 2://add
             ACC=ACC+mem[mem[PC].operando].operando;
             PC++;
             break;
         
         case 3://mul
            ACC=ACC*mem[mem[PC].operando].operando;
            PC++;
            break;
        
         case 4://div
            ACC=ACC/mem[mem[PC].operando].operando;
            PC++;
            break;
         
         case 5://sub
             ACC=ACC-mem[mem[PC].operando].operando;
             PC++;
             break;
         
         case 6://jmp
            PC=mem[PC].operando;
            
            break;
         
         case 7://jmp=0
            
            if(ACC==0)
            {
                
                PC=mem[PC].operando;
            }
            else
            {
                PC++;
            }
            break;
         
         case 8://jmp+
            if(ACC>0)
            {
                PC=mem[PC].operando;
            }
            else
            {
                PC++;
            }
           break;
         
         case 9://jmp-
            if(ACC<0)
            {
                PC=mem[PC].operando;
            }
            else
            {
                PC++;
            }
            break;
         
         case 10:
            printf("\nresultado=%d\n",mem[mem[PC].operando].operando);
            PC++;
            break;
         
         case 11:
            printf("\nInforme o valor:");
            scanf("%d",&mem[mem[PC].operando].operando);
            PC++;
            break;
         
         case 12:
            final=1;
            break;
        }
    }
}
    
int main()
{
    while(1)
    {
        printf(">>");
        loader();
        cpu();
    }
    return 0;
}
