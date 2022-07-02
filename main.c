#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROC_INFO_FILE_RAM "/meminfo"
#define PROC_INFO_FILE_CPU "/stat"

int main(){

	//Nombre de los archivo a abrir
	char *filename_info_cpu;
	char *filename_info_ram;

	//Almacenamiento en el HEAP
	filename_info_cpu = malloc(strlen("/proc/") + strlen(PROC_INFO_FILE_CPU));
	filename_info_ram = malloc(strlen("/proc/") + strlen(PROC_INFO_FILE_RAM));

	strcpy(filename_info_cpu, "/proc/");
	strcat(filename_info_cpu, PROC_INFO_FILE_CPU);

	strcpy(filename_info_ram, "/proc/");
	strcat(filename_info_ram, PROC_INFO_FILE_RAM);

	printf("Si se imprime %s", filename_info_cpu);
	printf("Si se imprime %s", filename_info_ram);

	
	//Creacion de puntero par lectura de los archivos de la RAM como del CPU
	FILE *CpuInfoFile;
	FILE *RamInfoFile;

	//Primero abrimos el arcchivo del cpu
	CpuInfoFile = fopen(filename_info_cpu, "r");
	
	
	return 0;
	
}


//Funcion que se va a llamar cuando se pueda leer el archivo stat.
long info_mem_status(FILE *file, char *field){
	char line[MAXLINE] //linea del archivo
	char field_name[MAAXLINE]; //campo de la linea
	long uso_cpu = 0;
}

long info_cpu_stats(int argsC, char* argV[]){
	char str[100];
	const char d[2] = " ";
	char *token;
	int i = 0, times, lag;
	long int sum = 0, idle, lastSum = 0, lastIdle = 0;
	long double idleFraction;

	if(argC != 3)
		printf("Usage : %s <number of times /proc/stat should be polled followed by delay in seconds.>",argV[0]);
	else{
		times = 
	}
}

