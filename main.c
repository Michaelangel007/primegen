/*made by alex, compile with pthread flag (-pthread for gcc)
 *this is just a rough test version upon which I will expand
 * */
#include <signal.h>
#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <pthread.h>
#define FORK_DEFAULT 2
void primedump(void);
void cleanExit(pthread_t *thread_id, int count);
int reallocArray(int buffincrement);
int returnResults(char *file,int max,bool check);
int readResults(char *file2);
int extendArray(int size2, int buffincrement);
void *checkPrime(void *threadarg);
unsigned int *primes,prime,count,*temp,buffsize;
void leave(int sig);

struct thread_data{
	int temp_id;
	int prime;
};

int main(int argc,char **argv){
(void) signal(SIGINT,leave);
bool verbose=0;
int max,maxfork,children,i,increment;
bool read=0;
bool check=0;
char *file=NULL;
char *file2=NULL;
maxfork=2;
max=30;
increment=100;
buffsize=100;
for(i=1;i<argc;i++){
if(argv[i][0]=='-'){
switch(argv[i][1]){
	case 'o':
	if(argv[i+1]!=NULL)
	file=argv[i+1];
	break;
	case 'b':
	if(argv[i+1]!=NULL)
	buffsize=atoi(argv[i+1]);
	break;
	case 's':
	if(argv[i+1]!=NULL)
	max=atoi(argv[i+1]);
	break;
	case 't':
	if(argv[i+1]!=NULL)
	maxfork=atoi(argv[i+1]);
	case 'v':
	verbose=1;
	break;
	case 'i':
	if(argv[i+1]!=NULL){
	read=1;
	file2=argv[i+1];}
	break;
	case 'c':
	if(argv[i+1]!=NULL){
	max=atoi(argv[i+1])+1;
	check=1;
	}
	break;
	case 'h':
	printf("Usage: proggy -s 10000\n\t-b\tset buffersize\tless need to update buffer/bigger mem use\n\t-s\tset max nr\tpick the number until where you want to calculate all primes\n\t-t\tset max threads\trecommended to set this to the number of processes you can run simultaneously, default is %i\n\t-v\tset verbosity\n\t-o\tset output file\tdump the results in specified file\n\t-i\tset input file\n\t-c\tcheck number\tchecks if given number is a prime\n\t-h\tcall 911\n",maxfork);
	return 1;
	default:break;
}
}
}
if(maxfork<2 || maxfork%2!=0){printf("Bare minimum of threads is 2,and has to be a multitude of 2\nSetting to default\n\n");maxfork=2;}
printf("btw: if you want to see the help use -h\n\nLoading buffers\n\n");
pthread_t thread_id[maxfork];
struct thread_data thread_args_array[maxfork];
temp=calloc(maxfork, sizeof(int *));
primes=malloc(buffsize * sizeof(int *));
prime=1;
count=0;
children=-1;
if(read==0)
{
primes[0]=5;
printf("Calculating until: %i\n", max);
}
else{
if(readResults(file2)==0){
prime=primes[count]/6;
if(primes[count]/6>max/6){
goto end;}
}
}
bool running=1;
while (running){
	while (prime*6+5<=primes[count]*5 && children < maxfork-2 && prime<(max)/6){
		children+=2;
		thread_args_array[children-1].temp_id=children-1;
		thread_args_array[children-1].prime=prime*6+1;
		thread_args_array[children].temp_id=children;
		thread_args_array[children].prime=prime*6+5;
		if(pthread_create(&thread_id[children-1],NULL,checkPrime,(void *) &thread_args_array[children-1]) != 0 || pthread_create(&thread_id[children],NULL,checkPrime,(void *) &thread_args_array[children]) !=0){
		cleanExit(thread_id,children);
		printf("pthread error, quitting\n");
		return 1;
		}
		prime++;
	}
	printf("Progress: %f with %i threads running\n", (float )prime/(float)max*600.0,children+1);
for(i=0;i<=children;i++){
		pthread_join(thread_id[i], NULL);
		}
children=-1;
if(extendArray(maxfork,increment)!=0){
	printf("Could not allocte more memory to primes array, quitting\n");
	cleanExit(thread_id,children);
	return 1;}
memset(temp,0,maxfork*sizeof(int *));
if(prime>=(max)/6)
running=0;
}
end:
returnResults(file,max,check);
cleanExit(thread_id,children);
return 0;
}
void cleanExit(pthread_t *threads, int count){
int i;
if(count>0){
for(i=0;i<=count;i++){
pthread_join(threads[i],NULL);
}}
free(primes);
free(temp);
}
void primedump(void){
int i;
for(i=0;i<=count;i++)
	printf("Prime[%i]: %i\n",i,primes[i]);
}

int readResults(char *file){
struct stat file_status;
int i;
FILE *read;
if(stat(file,&file_status)!=0){
printf("Can't stat %s\n",file);return 1;}else{
if((read=fopen(file,"rb"))!=NULL){
/*printf("file is %d bytes large, buffer has to be: %i bigger\n",(int)file_status.st_size,(int)file_status.st_size/sizeof(int *)-buffsize);*/
reallocArray((int)file_status.st_size/sizeof(int *)-buffsize);
i=fread(primes,sizeof(int *),buffsize,read);
count=i-1;
fclose(read);
return 0;
}else{
return 1;
}
}
}

int returnResults(char *file,int max,bool check){
int i,n;
if(file!=NULL){
FILE *dump;
dump = fopen(file,"wb");
n=fwrite(primes,sizeof(int *),count+1,dump);
fclose(dump);
printf("\n\nPrime numbers written to: %s, %i bytes\n",file,(count+1)*sizeof(int *));
}
if(check==0){
printf("\n\nPrime numbers found:\n2,3");
for(i=0;i<=count;i++){
printf(",%i",primes[i]);
}
printf("\n\n");
printf("Found %i primes\n", count+3);
}else{
if(primes[count]==max-1){
printf("%i is a prime!\n",max-1);
}
else{
printf("%i is not a prime!\n",max-1);
}
}
return 0;
}


int extendArray(int size2, int buffincrement){
int i=0;
check:
while(i<size2 && count<buffsize-1){
if(temp[i]!=0){
		count++;
		primes[count]=temp[i];
	}
	i++;
}
if(i>=size2){
return 0;
}else{
if(reallocArray(buffincrement)!=0)
	return 1;
goto check;
}
return 0;
}

int reallocArray(int buffincrement){
unsigned int *tmp= realloc(primes,(buffsize+buffincrement)*sizeof(int *));
if (tmp!=NULL){
buffsize=buffsize+buffincrement;
primes=tmp;
return 0;
}else{
return 1;
}
}

void *checkPrime(void *threadarg){
struct thread_data *my_data;
my_data=(struct thread_data *) threadarg;
int i;
for(i=0;primes[i]<=my_data->prime/5;i++){
if(my_data->prime%primes[i]==0){
pthread_exit(NULL);
}
}
temp[my_data->temp_id]=my_data->prime;
pthread_exit(NULL);
}

void leave(int sig){
free(primes);
free(temp);
printf("Received SIGINT, freed memory, quitting now\n");
exit(sig);
}
