//最佳路徑流量爆掉後，會找次佳解
#include<stdio.h>
int node,link;
int graph[50][100];
int graph_count[50];
int left_node[50],right_node[50];
int load[50];
int flow_size[50];

int shortcut[50][100];
int shortcut_count[50],shortcut_location[50][50];
int level[50],parent[50];
int count=0,check[50];
int update[25],update_count=0;

int two_shortcuts[50][25],two_shortcuts_count[50];

int output[25][25],output_count=0,output_flow=0,output_length[50];

int check_child(int point){ //update the child's level and parent
  int i;
  update_count=0;

  for(i=0;i<graph_count[point];i++)
    if(level[graph[point][i]]==-1 && parent[graph[point][i]]==-2){
      level[graph[point][i]]=level[point]+1;
      parent[graph[point][i]]=point;
      update[update_count++]=graph[point][i];
      count++;
    }

  return 0;
}

int BFS(int point){
   int i,ii,j=0,k,temp[25],pointt[50],temp1[50],h=0;

   level[point]=0;
   parent[point]=-1;
   count++;
   check_child(point);
   for(i=0;i<graph_count[point];i++)
      pointt[j++]=graph[point][i];

   while(count!=node){
    h=0;
    for(k=0;k<j;k++){
      check_child(pointt[k]);
      if(count==node)
         goto line;
      for(i=0;i<update_count;i++)
        temp1[h++]=update[i];
    }
    for(j=0;j<h;j++)
       pointt[j]=temp1[j];
   }

   line:
   for(i=0;i<node;i++){
     j=0;
     shortcut[point][shortcut_count[point]++]=point;
     shortcut_location[point][i]=shortcut_count[point]-1;
     if(level[i]==0)
        continue;
     else if(level[i]==1)
        shortcut[point][shortcut_count[point]++]=i;
     else{
       ii=i;
       temp[j++]=ii;
       while(parent[ii]!=point){
        ii=parent[ii];
        temp[j++]=ii;
       }
       for(k=j-1;k>=0;k--)
         shortcut[point][shortcut_count[point]++]=temp[k];
     }
   }
   shortcut_location[point][i]=shortcut_count[point];
   return 0;
}

int find_flow(int s,int d){
  for(int i=0;i<link;i++)
    if(s==left_node[i] || s==right_node[i])
      if(d==right_node[i] || d==left_node[i])
         return i;
}

float calculate_weight(int linkk,int floww){
  float w;
  w=(float)flow_size[floww]/(load[linkk]-flow_size[floww]);
  if(load[linkk]-flow_size[floww]==0)
    w=100;
  return w;
}

int check_same(int a,int b){
  int i,j;
  for(i=0,j=0;i<two_shortcuts_count[a],j<two_shortcuts_count[b];i++,j++)
    if(two_shortcuts[a][i]!=two_shortcuts[b][j])
        return 0;
  return 1;
}

int main(){

  freopen("input.txt","r",stdin);

  int i,j,k,a,b,c,y=0;
  scanf("%d %d",&node,&link);

  for(i=0;i<node;i++)
    graph_count[i]=0;

  for(i=0;i<link;i++){
    scanf("%d",&a);
    scanf("%d",&b);
    scanf("%d",&c);
    graph[b][graph_count[b]++]=c;
    graph[c][graph_count[c]++]=b;
    left_node[i]=b;
    right_node[i]=c;
    scanf("%d",&load[i]);
  }

  int flow;
  scanf("%d",&flow);
  int source[flow],destination[flow];
  for(i=0;i<flow;i++){
    scanf("%d",&a);
    scanf("%d",&source[i]);
    scanf("%d",&destination[i]);
    scanf("%d",&flow_size[i]);
  }

  //shortcut
  for(i=0;i<node;i++)
    shortcut_count[i]=0;

  for(i=0;i<node;i++){
    for(j=0;j<node;j++){
       level[j]=-1;
       parent[j]=-2;
    }
    count=0;
    BFS(i);
  }

  /*for(i=0;i<node;i++){
   for(j=0;j<shortcut_count[i];j++)
     printf("%d ",shortcut[i][j]);
   printf("\n");
  }*/

  int f,minn,min1,minn1;
  float weight[link],weight_sum[node],min;

  for(j=0;j<link;j++)
      weight[j]=0;

  for(i=0;i<flow;i++){  //flow

    //choose two shortcuts
    for(j=0;j<node;j++)
      two_shortcuts_count[j]=0;

    //put into one shortcut
    for(j=0;j<node;j++)
      for(k=shortcut_location[source[i]][j];k<shortcut_location[source[i]][j+1];k++)
         two_shortcuts[j][two_shortcuts_count[j]++]=shortcut[source[i]][k];

    //put into the other shortcut
    for(j=0;j<node;j++)
      for(k=shortcut_location[j][destination[i]]+1;k<shortcut_location[j][destination[i]+1];k++)
        two_shortcuts[j][two_shortcuts_count[j]++]=shortcut[j][k];

    /*for(j=0;j<node;j++){
      for(k=0;k<two_shortcuts_count[j];k++)
        printf("%d ",two_shortcuts[j][k]);
      printf("\n");
    }
    printf("\n");*/

    //choose the shortest one(first time)
    if(y==0){
     min1=100;
     for(j=0;j<node;j++)
       if(two_shortcuts_count[j]<min1){
          min1=two_shortcuts_count[j];
          minn1=j;
       }

     for(j=0;j<two_shortcuts_count[minn1];j++)
       output[output_count][j]=two_shortcuts[minn1][j];
     output_length[output_count]=two_shortcuts_count[minn1];
     output_flow+=flow_size[i];
     output_count++;

     //update the weight
     for(j=0;j<two_shortcuts_count[minn1]-1;j++){
       f=find_flow(two_shortcuts[minn1][j],two_shortcuts[minn1][j+1]);
       weight[f]=calculate_weight(f,i);
       load[f]-=flow_size[i];
     }

     y=1;
     continue;
    }

    //calculate the weight
    for(j=0;j<node;j++)
        weight_sum[j]=0;

    for(j=0;j<node;j++)
      for(k=0;k<two_shortcuts_count[j]-1;k++){
         f=find_flow(two_shortcuts[j][k],two_shortcuts[j][k+1]);
         if(load[f]<flow_size[i]){
            weight_sum[j]=100;
            continue;
         }
         weight_sum[j]+=weight[f];
      }

    //find the minimum weight
    min=99;minn=-1;
    for(j=0;j<node;j++)
      if(weight_sum[j]<=min && weight_sum[j]>=0){
        if(minn!=-1){
          if(check_same(minn,j)==1) //if they are the same
            continue;
        }
        if(weight_sum[j]==min){  //compare the hop
          if(two_shortcuts_count[minn]>two_shortcuts_count[j]){
             min=weight_sum[j];
             minn=j;
             continue;
          }
          else
             continue;
        }
        min=weight_sum[j];
        minn=j;
      }

    if(min==99)  //reject
      continue;

    //update the weight
    for(j=0;j<two_shortcuts_count[minn]-1;j++){
      f=find_flow(two_shortcuts[minn][j],two_shortcuts[minn][j+1]);
      weight[f]=calculate_weight(f,i);
      load[f]-=flow_size[i];
    }

    //put into output
    for(j=0;j<two_shortcuts_count[minn];j++)
      output[output_count][j]=two_shortcuts[minn][j];
    output_length[output_count]=two_shortcuts_count[minn];
    output_flow+=flow_size[i];
    output_count++;
  }

  //output
  printf("%d %d\n",output_count,output_flow);
  for(j=0;j<output_count;j++){
    printf("%d ",j);
    for(k=0;k<output_length[j];k++)
      printf("%d ",output[j][k]);
    printf("\n");
  }

  return 0;
}
