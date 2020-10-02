#include<stdio.h>
#include<ctype.h>
int number;
int number_compare(char c){  //change character into integer
  if(c=='0')
    number=0;
  else if(c=='1')
    number=1;
  else if(c=='2')
    number=2;
  else if(c=='3')
    number=3;
  else if(c=='4')
    number=4;
  else if(c=='5')
    number=5;
  else if(c=='6')
    number=6;
  else if(c=='7')
    number=7;
  else if(c=='8')
    number=8;
  else
    number=9;
  return number;
}

int main(){
  //freopen("input.txt","r",stdin);  //read the file

  int rule_count;   //the number of rules
  scanf("%d\n",&rule_count);

  int i,j;  //index
  int k=0;  //check if it is a negative number
  char c;
  int rule[rule_count][2];    //the array of rules
  int port[rule_count];      //the array of ports

  for(i=0;i<rule_count;i++)
    for(j=0;j<2;j++)
       rule[i][j]=0;    //make array zero

  i=0;j=0;
  while(i<rule_count){
   c=getchar();  //read the character

   if(c=='*')    //if it is *
     rule[i][j]=42;

   else if(c=='-')  //if it is -
     k=1;

   else if(isspace(c)){  //if it is the space
     if(j==1){    //rule[i][j]
       scanf("%d\n",&port[i]);
       i++;
       j=0;
     }
     else
       j++;
   }

   else{    //if it is a number
    number=number_compare(c);  //change character into integer
    if(k==1){   //if it is a negative number
      number=number*-1;
      k=0;
    }
    rule[i][j]=10*rule[i][j]+number;  //if next character is also a number
   }
  }

  int flow_count;    //the number of flows
  scanf("%d",&flow_count);
  int flow[flow_count][2];    //the array of flows
  for(i=0;i<flow_count;i++)
    scanf("%d %d",&flow[i][0],&flow[i][1]);

  int output[flow_count];       //the result of port
  for(i=0;i<flow_count;i++){
    for(j=0;j<rule_count;j++){
       if(flow[i][0]==rule[j][0] || rule[j][0]==42){        //compare the first number of each rule
          if(flow[i][1]==rule[j][1] || rule[j][1]==42){     //compare the second one
             output[i]=port[j];
             break;
          }
       }
    }
    if(j==rule_count)   //if there is no proper output port
       output[i]=-1;
  }

  //freopen("output.txt","w",stdout);
  printf("%d\n",flow_count);
  for(i=0;i<flow_count;i++){
    if(output[i]==-1)
      printf("%d %d drop\n",flow[i][0],flow[i][1]);
    else
      printf("%d %d %d\n",flow[i][0],flow[i][1],output[i]);
  }

  return 0;
}
