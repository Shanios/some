#include<stdio.h>
#include<stdlib.h>
#define BUCKET_SIZE 10
#define OP_RATE 3

void leaky_bucket(int packets[],int n){
    int bucket = 0;
    int i;

    printf("\nTime\tIncoming\tBucket Level\tOutgoing\n");
    printf("-------------------------------------------------\n");

    for(i=0;i<n;i++){
        printf("%d\t%d\t",i+1,packets[i]);
        if(packets[i]>BUCKET_SIZE){
            printf("EXCEEDED!\tDropped!\n");
            continue;
        }

        if(bucket+packets[i]<=BUCKET_SIZE){
            bucket+=packets[i];
        }
        else{
            printf("OVERFLOW\tDropped!\n");
            bucket=BUCKET_SIZE;
            continue;
        }
        printf("%d\t\t",bucket);

        if(bucket<=OP_RATE){
            printf("%d\n",bucket);
            bucket=0;
        }
        else{
            printf("%d\n",OP_RATE);
            bucket-=OP_RATE;
        }
    }
    while (bucket>0){
        printf("%d\t0\t%d\t\t",++i,bucket);
        if(bucket<=OP_RATE){
            printf("%d\n",bucket);
            bucket=0;
        }
        else{
            printf("%d\n",OP_RATE);
            bucket-=OP_RATE;
        }
    }
}
int main(){
    int packets[]={4,8,2,5,3,12};
    int n = sizeof(packets)/sizeof(packets[0]);
    leaky_bucket(packets,n);
    return 0;
}