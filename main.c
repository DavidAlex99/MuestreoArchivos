#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define PROC_INFO_FILE_RAM "/meminfo"
#define PROC_INFO_FILE_CPU "/stat"
#define LONGITUD_MAXIMA_CADENA 1000

long info_mem_status(FILE *file, char *field);
int info_cpu_status(FILE *file);

int main(){

	//Nombre de los archivo a abrir
	char *filename_info_cpu;
	char *filename_info_ram;

	//Almacenamiento en el HEAP
	filename_info_cpu = malloc(strlen("/proc") + strlen(PROC_INFO_FILE_CPU));
	filename_info_ram = malloc(strlen("/proc") + strlen(PROC_INFO_FILE_RAM));

	strcpy(filename_info_cpu, "/proc");
	strcat(filename_info_cpu, PROC_INFO_FILE_CPU);

	strcpy(filename_info_ram, "/proc");
	strcat(filename_info_ram, PROC_INFO_FILE_RAM);

	printf("Si se imprime %s\n", filename_info_cpu);
	printf("Si se imprime %s\n", filename_info_ram);

	
	//Creacion de puntero par lectura de los archivos de la RAM como del CPU
	FILE *CpuInfoFile;
	FILE *RamInfoFile;

	//Primero abrimos el arcchivo del cpu
	CpuInfoFile = fopen(filename_info_cpu, "r");
	//Despues el otro archivo
	RamInfoFile = fopen(filename_info_ram, "r");

	//Si por si acaso el archivo no existe
	if(CpuInfoFile == NULL){
		fprintf(stderr , "Error al leer al informacion del archivo procInfoFile");
		return 1;
	}
	printf("Esto es que si se encuentra el archivo CpuInfoFile\n");

	if(RamInfoFile == NULL){
		fprintf(stderr, "Error al leer la informacion de RamInfoFile");
		return 1;
	}
	printf("Esto es que si se encuentra el archivo RamInfoFile\n");

	//Libera reservas
	free(filename_info_cpu);
	free(filename_info_ram); 

	//Deshabilia el buffer
	setbuf(stdout, NULL);
	//Desabilita el buffer del archivo
	setbuf(CpuInfoFile, NULL);
	setbuf(RamInfoFile, NULL);

	//Para guardar en variables para el nombre del proceso
	char proc_name[MAXLINE];

	//Variables para guardar el numero de los procesos, que es CPU y RAM
	long carga_cpu = 0;
	long mem_ocupada = 0;


	//LINEA HASTA MIENTRAS PARA PROBAR
	mem_ocupada = info_mem_status(RamInfoFile,"MemFree:");
	printf("La memoria ocupada es: %ld\n", mem_ocupada);
	
	printf("Si se llega aqui es que no se llama a la funcion para obtener datos de la mem\n");

	info_cpu_status(CpuInfoFile);
	/*
	do{
		printf("Memeoria usada: %ld", mem_ocupada);
	
		//obtiene el uso de la memoria ram que se esta ocupando
		//Pasandi como parametro el nombre que se quiere y el archibo que va a leer
		mem_ocupada = info_mem_status(RamInfoFile, "MemFree:");

 		//Obtiene el uso del cpu llamando al cpu
 		//AQIU PONER EL PARAMETRO QUE LLAMA AL CPU
 		
	}while(mem_ocupada > 0);*/

	//Cerrando los dos archivos
	fclose(CpuInfoFile);
	fclose(RamInfoFile);
	
	return 0;
	
}


//Funcion que se va a llamar cuando se pueda leer el archivo stat.
long info_mem_status(FILE *file, char *field){
	char line[MAXLINE]; //linea del archivo
	char field_name[MAXLINE]; //campo de la linea
	long uso_mem = 0;
	printf("Si se entra a la funcion\n");

	rewind(file);

	int n;

	while(fgets(line, MAXLINE, file) > 0){
		n = sscanf(line, "%s %ld Kb", field_name, &uso_mem);
		if(n == 2)
			if(strcmp(field_name, field) == 0)
				return uso_mem;
	}
	return -1;	
}


//Funcion para leer el archivo stat del cpu, se pasa el archivo CpuInfoFile
int info_cpu_status(FILE *file){
	char str[100];
	const char d[6] = " "; //Esto para hacer una sublinea de la linea [partiendo desde la segunda elemnto del indice, borrando cpu
	char *token;
	int i = 0, times, lag;
	long int sum = 0, idle, lastSum = 0, lastIdle = 0;
	long double idleFraction;
	

	rewind(file);

	char bufer[LONGITUD_MAXIMA_CADENA];
	/*while(fgets(bufer, LONGITUD_MAXIMA_CADENA, file)){
		strtok(bufer, d); //sublinea a partir del salto e linea
		printf("La linea es: %s\n", bufer);
	}*/
	fgets(bufer, LONGITUD_MAXIMA_CADENA, file);
	i = 0;
	token = strtok(bufer, d);
	sum = 0;
	while(token!=NULL){
		token = strtok(NULL, d);
		if(token!=NULL){
			sum += atoi(token);
		}
		if(i==3){
			idle = atoi(token);
		}
	}
	
	printf("%ld", sum);
	
	return 0;
}

	
