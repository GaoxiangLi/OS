#include<studio.h>
#include<math.h>

int main(){
	double a[]={1,2,3,4,5,6,7,8,9,10};
	double sum=0;
	for(int i=0;i<10;i++){
		sum+=sqrt(a[i]);
	}
	double avg=sum/10;
	return avg;
}