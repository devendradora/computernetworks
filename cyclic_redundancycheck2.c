#include<stdio.h>
#include<string.h>
int divrlen,divlen,i,j;
char divisor[100],div[100],divcp[100];
int main()
{
  printf("Enter divisor : ");
  scanf("%s",&divisor);
  printf("Enter Dividend : ");
  scanf("%s",&div);
  divrlen=strlen(divisor);
  divlen=strlen(div);
  strcpy(divcp,div);
  for(i=0;i<divrlen-1;i++)
  {
    div[divlen+i]='0';
  }
  div[divlen+i]='\0';
  divlen=strlen(div);
  i=0;
  while(1)
  {
    if(div[i]=='0')
      i++;
    else if(divlen-i>=divrlen)
    {
      for(j=0;j<divrlen;j++)
      {
    div[i++]=(div[i]==divisor[j])?'0':'1';
      }
      i=i-divrlen;
    }
    else
      break;
  }
  printf("\nRemainder = %s\n\n",div);
  printf("Data sent = %s",divcp);
  for(i=divlen-divrlen+1;i<divlen;i++)
  {
     printf("%c",div[i]);
  }
  return 0;
}
