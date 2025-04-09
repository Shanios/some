#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define MAX_NODES 10
#define INF INT_MAX

struct node{
    int id;
    int distance[MAX_NODES];
    int nexthop[MAX_NODES];
};

struct node nodes[MAX_NODES];
int num_nodes;

void initNetwork(int x){
    num_nodes = x;
    for(int i=0;i<num_nodes;i++){
        nodes[i].id = i;
        for(int j=0;j<num_nodes;j++){
            if(i==j){
                nodes[i].distance[j] = 0;
                nodes[i].nexthop[j] = i;
            }
            else{
                nodes[i].distance[j] = INF;
                nodes[i].nexthop[j] = -1;
            }
        }
    }
}

void updateVectors(){
    int updated;
    do{
        updated=0;
        for(int i=0;i<num_nodes;i++){
            for(int j=0;j<num_nodes;j++){
                if(i!=j){
                    for(int k=0;k<num_nodes;k++){
                        if(nodes[i].distance[k] == INF || nodes[k].distance[j] == INF)
                        {
                            continue;
                        }
                        int new_distance = nodes[i].distance[k] + nodes[k].distance[j];
                        if(new_distance<nodes[i].distance[j]){
                            nodes[i].distance[j]=new_distance;
                            nodes[i].nexthop[j]=nodes[i].nexthop[k];
                            updated=1;
                        }
                        }
                    }
                }

            }
        }while(updated);
    }

void printRT(){
    printf("Routing Table\n");
    for(int i=0;i<num_nodes;i++){
        printf("Node %d: ",nodes[i].id);
        for(int j=0;j<num_nodes;j++){
            if(nodes[i].distance[j] == INF){
                printf("INF ");
            }
            else
                printf("%d ",nodes[i].distance[j]);
        }
        printf("\n");
    }
}

int main(){
    initNetwork(4);

    nodes[0].distance[1]=1;
    nodes[1].distance[0]=1;
    nodes[1].distance[2]=1;
    nodes[2].distance[1]=1;
    nodes[2].distance[3]=1;
    nodes[3].distance[2]=1;

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(nodes[i].distance[j]==1)
                nodes[i].nexthop[j] = j;
        }
    }

updateVectors();
printRT();
return 0;
}