#include<stdio.h>

int main(){	
	int P,R,i,j,flag;
	//Input number of processes and resources
	printf("\nEnter the number of processes :");
	scanf("%d",&P);
	printf("\nEnter the number of resources :");
	scanf("%d",&R);
	int max[P][R], allocation[P][R], available[R], need[P][R], unsatisfied,mark[P],temp;
	unsatisfied=P;
/******Enter maximum matrix******/
	printf("\nEnter the maximum matrix");
	for(i=0;i<P;i++){
		mark[i]=0;
		for(j=0;j<R;j++){
		printf("\nEnter element %d of row %d:",j+1,i+1);
		scanf("%d",&max[i][j]);
		}
	}

/******Enter allocation matrix******/
	printf("\nEnter the allocation matrix");
	for(i=0;i<P;i++){
		for(j=0;j<R;j++){
		printf("\nEnter element %d of row %d:",j+1,i+1);
		scanf("%d",&allocation[i][j]);
		}
	}

/******Enter available vector******/
	printf("\nEnter the available vector");
	for(j=0;j<R;j++){
		printf("\nEnter element %d of available vector:",j+1);
		scanf("%d",&available[j]);
	}

/******Compute the need matrix******/
	for(i=0;i<P;i++){
		for(j=0;j<R;j++){
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}

/******Logic to detect deadlock******/
	while(unsatisfied!=0){
		temp=unsatisfied;
		for(i=0;i<P;i++){			
			/*Check if process is already satisfied or not*/				
			if(mark[i]==0){
				flag=1;
				for(j=0;j<R;j++){			
					if((available[j]-need[i][j]) < 0){
						flag=0;
						break;
					}
				}
				/*request can be satisfied*/
				if(flag==1){
					printf("\nProcess %d satisfied\n",i+1);
					mark[i]=1;
					unsatisfied--;
					for(j=0;j<R;j++){
						available[j]+=allocation[i][j];			
					}
				}
			}		
		}			
		if(unsatisfied==temp){	
			printf("\nUnsafe state! Deadlock will take place!\n");
			break;
		}
	}
	return 0;
}
