#include<stdio.h>
int vertex;
int old_path[50],new_path[50];
int output[50][50];
int update[50];
int wait_to_change[50];
int count=0,h=0;

int merge(int point){
 int k;
 k=new_path[point];

 if(update[k]==1)
   merge(k);
 else
   return k;
}

int find(int point){
  for(int i=0;i<count;i++)
    if(wait_to_change[i]==point)
       return i;
  return count;
}

int graph(int point){
 int k;
 k=old_path[point];
 if(k==vertex-1)
    return -1;
 if(old_path[k]!=-1 && new_path[k]!=-1)
    return k;
 else
    graph(k);
}

int main(){

  FILE *fp_r=fopen("input.txt","r");

  //input
  int round=0,countt=0;
  fscanf(fp_r,"%d",&vertex);
  int i,j;

  for(i=0;i<vertex;i++)
    fscanf(fp_r,"%d",&old_path[i]);
  for(i=0;i<vertex;i++)
    fscanf(fp_r,"%d",&new_path[i]);

  //the arrangement of output
  for(i=0;i<vertex;i++)
    output[round][i]=old_path[i];
  round++;

  for(i=0;i<vertex;i++)
    update[i]=0;

  //the first round
  for(i=0;i<vertex;i++){
    if(old_path[i]==-1){
       output[round][i]=new_path[i];
       update[i]=1;
    }
    else
       output[round][i]=output[round-1][i];
  }
  round++;

  //wait_to_change
  int h;
  wait_to_change[0]=0;
  count=1;
  h=wait_to_change[0];
  while(1){
   h=graph(h);
   if(h==-1)
     break;
   wait_to_change[count]=h;
   count++;
  }

  //merge
  for(i=0;i<vertex;i++)
    if(update[i]==1)
       old_path[i]=merge(i);

  for(i=0;i<vertex;i++)
    if(update[i]==0 && new_path[i]!=-1)
      old_path[i]=merge(i);

  int step[count];
  int max,maxx,maxxx;
  int non_conflict[count],ii=0;
  int change[vertex];
  int location,location1,location2,location3;
  int cannotchoose[count];

  while(countt<count){

    //calculate the steps
    for(i=0;i<count;i++){
      if(update[wait_to_change[i]]==0){
        location=find(old_path[wait_to_change[i]]);
        if(location>i)
           step[i]=location-i-1;
        else
           step[i]=location-i+1;
      }
    }

     //find the maximum step
    max=-1000;
    for(i=0;i<count;i++)
     if(update[wait_to_change[i]]==0){
       if(step[i]>max){
         max=step[i]; //程˙计琌ぶ
         maxx=wait_to_change[i]; //程˙计翴
         maxxx=i; //程˙计竚
       }
     }

    //update the maximum step
    for(i=0;i<vertex;i++){
      if(i==maxx){
         output[round][i]=new_path[i];
         update[i]=1;
         countt++;
      }
      else
         output[round][i]=output[round-1][i];
    }

    //find and update non-conflict
    ii=0;

    for(i=0;i<count;i++)
        cannotchoose[i]=0;

    for(i=0;i<count;i++){
     location=find(old_path[maxx]);  //程˙计翴 穝 竚
     location1=find(old_path[wait_to_change[i]]);  //ㄤ翴 穝 竚

     if(cannotchoose[location1]==1)
        continue;

     if(location>maxxx && update[wait_to_change[i]]==0 && cannotchoose[i]==0){ //程˙计翴┕传
      if(i>=location){
         if(location1>=location){
           non_conflict[ii]=wait_to_change[i];
           ii++;
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           countt++;
         }
       }
       else if(i<=maxxx){
         if(location1<=maxxx){
           non_conflict[ii]=wait_to_change[i];
           ii++;
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           countt++;
         }
       }
     }

     else if(location<maxxx && update[wait_to_change[i]]==0 && cannotchoose[i]==0){ //程˙计翴┕オ传
       if(i<=location){
         if(location1<=location){
           non_conflict[ii]=wait_to_change[i];
           ii++;
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           countt++;
         }
       }
       else if(i>=maxxx){
         if(location1>=maxxx){
           non_conflict[ii]=wait_to_change[i];
           ii++;
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           countt++;
         }
       }
     }

     if(ii!=0){
      location2=find(non_conflict[ii-1]);
      location3=find(old_path[non_conflict[ii-1]]);
      if(location3>location2)
        for(j=location2+1;j<location3;j++)
          cannotchoose[j]=1;
      else if(location3<location2)
        for(j=location3+1;j<location2;j++)
          cannotchoose[j]=1;
     }
    }
    round++; //the end of odd round

    //check
    if(countt==count)break;

    //if there is no value in the middle
    location=find(old_path[maxx]);
    if((location>maxxx && location-maxxx==1) || (location<maxxx && location-maxxx==-1))
        goto line1;

    for(i=0;i<vertex;i++)
        change[i]=0;      //癘魁硂э跑

    //update the middle between the step
    for(i=0;i<count;i++){
      if(update[wait_to_change[i]]==0){
        location=find(old_path[maxx]);
        if(location>maxxx && i>maxxx && i<location){
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           change[wait_to_change[i]]=1;
           countt++;
        }
        else if(location<maxxx && i<maxxx && i>location){
           output[round][wait_to_change[i]]=new_path[wait_to_change[i]];
           update[wait_to_change[i]]=1;
           change[wait_to_change[i]]=1;
           countt++;
        }
      }
    }

    line1:
    //if there is no value in the middle
    if(ii==0)
      goto line;

    //update the middle between non_conflict
    for(i=0;i<ii;i++){
      location=find(old_path[non_conflict[i]]);
      location1=find(non_conflict[i]);
      if(location>location1){
        for(j=location1+1;j<location;j++){
          output[round][wait_to_change[j]]=new_path[wait_to_change[j]];
          update[wait_to_change[j]]=1;
          change[wait_to_change[j]]=1;
          countt++;
        }
      }
      else if(location<location1){
        for(j=location+1;j<location1;j++){
          output[round][wait_to_change[j]]=new_path[wait_to_change[j]];
          update[wait_to_change[j]]=1;
          change[wait_to_change[j]]=1;
          countt++;
        }
      }
    }

    line:
    for(i=0;i<vertex;i++)
      if(change[i]==0)
         output[round][i]=output[round-1][i];

    round++; //the end of even round

    //merge
    for(i=0;i<vertex;i++)
     if(update[i]==0 && new_path[i]!=-1)
       old_path[i]=merge(i);
  }

  int check=0;
  for(i=0;i<vertex-1;i++)
    if(new_path[i]==-1){
      check=1;
      break;
    }

  //the final round
  if(check==1){
    for(i=0;i<vertex;i++){
      if(new_path[i]==-1)
        output[round][i]=new_path[i];
      else
        output[round][i]=output[round-1][i];
    }
    round++;
  }

  //print
  //freopen("output.txt","w",stdout);
  printf("%d\n",round);
  for(i=0;i<round;i++){
    for(j=0;j<vertex;j++)
      printf("%d ",output[i][j]);
    printf("\n");
  }
  return 0;
}
