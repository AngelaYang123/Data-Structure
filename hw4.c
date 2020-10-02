#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int node,link,prime,cost[50];
int graph[50][50],graph_count[50];
int left_node[50],right_node[50];
int sample_point[25],sample_point_count=1;
int real_distance[50],temp_distance[50];
int countt=0;

typedef struct hashing_table *pointer;
typedef struct hashing_table{
  int left;
  int right;
  int distance;
  pointer next;
} ;

pointer table[100000000];
pointer x;

int find_cost(int l,int r){
  for(int i=0;i<link;i++)
    if(l==left_node[i] || l==right_node[i])
      if(r==right_node[i] || r==left_node[i])
         return i;
}

int location_of_hashing_table(int a, int b){
  return ((a+1)*(b+1))%prime%((int)pow(node,1.5));
}

int* find_pair_in_hashing_table(int a, int b){
  int k;

  k=location_of_hashing_table(a,b);
  for(x=table[k];x!=NULL;x=x->next)
    if(x->left==a || x->right==a)
      if(x->left==b || x->right==b)
         return x;

  return NULL;
}

int find_nearest_sample_point(int point,int d){
 if(d==0)
    return point;
 for(int i=0;i<sample_point_count;i++){
   x=find_pair_in_hashing_table(sample_point[i],point);
   if(x->distance==d){
      if(x->left==point)
        return x->right;
      else
        return x->left;
   }
 }
 return 0;
}

int count;
int level[50];
int update[50],update_count=0;
int give_level(int point){
  int i;
  update_count=0;
  for(i=0;i<graph_count[point];i++)
    if(level[graph[point][i]]==-1){
      level[graph[point][i]]=level[point]+1;
      update[update_count++]=graph[point][i];
      count++;
    }

  return 0;
}

int gg=0;
int update_neighbor(int fixed_point,int point){
  int i,j,k;

  for(i=0;i<graph_count[point];i++){
    j=find_cost(point,graph[point][i]);
    x=find_pair_in_hashing_table(fixed_point,graph[point][i]);
    if(temp_distance[graph[point][i]]==0 && graph[point][i]!=fixed_point){
      if(gg==0){ //BFS for sample nodes
         k=location_of_hashing_table(fixed_point,graph[point][i]);
         x=(int*)malloc(3*sizeof(int));
         x->left=fixed_point;
         x->right=graph[point][i];
         x->distance=temp_distance[point]+cost[j];
         x->next=table[k];
         table[k]=x;
       }
       temp_distance[graph[point][i]]=temp_distance[point]+cost[j];
    }
    else{
       if(temp_distance[point]+cost[j]<temp_distance[graph[point][i]]){
          if(gg==0)
            x->distance=temp_distance[point]+cost[j];
          temp_distance[graph[point][i]]=temp_distance[point]+cost[j];
          update_neighbor(fixed_point,graph[point][i]);
       }
    }
  }

  return 0;
}

int BFS(int fixed_point,int point){
  int i,j=0,k,pointt[50],temp1[50],h,maxx=-1,l=0;
  temp_distance[fixed_point]=0;

  give_level(fixed_point);
  for(i=0;i<graph_count[point];i++)
      pointt[j++]=graph[point][i];

   while(count!=node){ //find the level of each node
    h=0;
    for(k=0;k<j;k++){
      give_level(pointt[k]);
      if(count==node)
         goto line;
      for(i=0;i<update_count;i++)
        temp1[h++]=update[i];
    }
    for(j=0;j<h;j++)
       pointt[j]=temp1[j];
   }

  line:

  for(i=0;i<node;i++)
    if(maxx<level[i])
       maxx=level[i];

  //update the neighbors of each node orderly according to its level
  for(l=0;l<=maxx;l++)
   for(i=0;i<node;i++)
     if(level[i]==l)
       update_neighbor(fixed_point,i);

  /*for(i=0;i<node;i++)
    printf("%d ",temp_distance[i]);
  printf("\n");*/

  return 0;
}

int main(){
  //freopen("input.txt","r",stdin);
  int i,j,k;
  int a,b,c,d,e=0;
  int max;
  scanf("%d %d %d",&node,&link,&prime);
  for(i=0;i<link;i++){
    scanf("%d",&a);
    scanf("%d %d",&b,&c);
    graph[b][graph_count[b]++]=c;
    graph[c][graph_count[c]++]=b;
    left_node[i]=b;
    right_node[i]=c;
    scanf("%d",&cost[i]);
  }

  int pair;
  scanf("%d",&pair);
  int source[pair],destination[pair];
  for(i=0;i<pair;i++){
    scanf("%d",&a);
    scanf("%d %d",&source[i],&destination[i]);
  }

  sample_point[0]=0;
  if(sqrt(node)!=(int)sqrt(node))
    d=(int)sqrt(node)+1;
  else
    d=(int)sqrt(node);

  while(1){ //find sample nodes

   for(i=0;i<node;i++){
     temp_distance[i]=0;
     level[i]=-1;
   }
   count=1;
   level[sample_point[sample_point_count-1]]=0;
   BFS(sample_point[sample_point_count-1],sample_point[sample_point_count-1]);

   if(e==0){
    for(i=0;i<node;i++)
      real_distance[i]=temp_distance[i];
   }
   else{ //update the distance of nodes to their nearest sample nodes
     for(i=0;i<node;i++)
       if(real_distance[i]>temp_distance[i])
         real_distance[i]=temp_distance[i];
   }

   if(sample_point_count==d)
      break;

   max=-1; //find the next sample node
   for(i=0;i<node;i++)
     if(real_distance[i]>max){
       max=real_distance[i];
       sample_point[sample_point_count]=i;
     }
   sample_point_count++;
   e=1;
  }

  /**********************************************/

  //count the table
  for(i=0;i<(int)pow(node,1.5);i++)
    for(x=table[i];x!=NULL;x=x->next)
      if(x!=NULL)
        countt++;

  countt-=sample_point_count*(sample_point_count-1)/2;

  //make circles of each node except sample nodes
  int f,counttt,m,ii;
  for(i=0;i<node;i++){
    counttt=0;
    for(j=0;j<sample_point_count;j++)
      if(sample_point[j]!=i)
         counttt++;
    if(counttt==sample_point_count){
       for(ii=0;ii<node;ii++){
         temp_distance[ii]=0;
         level[ii]=-1;
        }
       count=1;
       level[i]=0;
       gg=1; //BFS for drawing circles
       BFS(i,i);
       for(k=0;k<node;k++)
         if(temp_distance[k]<=real_distance[i]){
           m=location_of_hashing_table(i,k);
           x=find_pair_in_hashing_table(i,k);
           if(x==NULL && i!=k){
             x=(int*)malloc(3*sizeof(int));
             x->left=i;
             x->right=k;
             x->distance=temp_distance[k];
             x->next=table[m];
             table[m]=x;
             countt++;
           }
         }
    }
  }

  //find the approximative or exact distance of pairs
  int output[pair],g,h;
  for(i=0;i<pair;i++){
    x=find_pair_in_hashing_table(source[i],destination[i]);
    if(x!=NULL)
      output[i]=x->distance;
    else{
      f=find_nearest_sample_point(source[i],real_distance[source[i]]);
      x=find_pair_in_hashing_table(f,destination[i]);
      g=x->distance+real_distance[source[i]];

      f=find_nearest_sample_point(destination[i],real_distance[destination[i]]);
      x=find_pair_in_hashing_table(f,source[i]);
      h=x->distance+real_distance[destination[i]];

      if(g>h)
        output[i]=h;
      else
        output[i]=g;
    }
  }

  /*for(i=0;i<(int)pow(node,1.5);i++)
    for(x=table[i];x!=NULL;x=x->next)
      if(x!=NULL)
        printf("%d %d %d\n",x->left,x->right,x->distance);*/

  //output
  printf("%d\n",countt);
  for(i=0;i<pair;i++){
    printf("%d ",i);
    printf("%d %d ",source[i],destination[i]);
    printf("%d\n",output[i]);
  }

  return 0;
}
