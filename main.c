#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define PROC_INFO_FILE_RAM "/meminfo"
#define PROC_INFO_FILE_CPU "/stat"
#define LONGITUD_MAXIMA_CADENA 1000

struct Proceso{
	long memoria_total;
	long memoria_free;
	long memoria_avaliable;
	long memoria_buffer;
	long memoria_cache;

};

long info_mem_status_total_mem(FILE *file, char *field);
long info_mem_status_free_mem(FILE *file, char *field);
long info_mem_status_buffer_mem(FILE *file, char *field);
long info_mem_status_cache_mem(FILE *file, char *field);

int info_cpu_status(char *file);

int main(){
	int periodo = 3000; //periodo de prueba

	//Nombre de los archivo a abrir
	char *filename_info_cpu;
	char *filename_info_ram;

	struct Proceso *procesos;

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
	//free(filename_info_cpu);
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
	long mem_total = 0;
	long mem_free = 0;
	long mem_avaliable = 0;
	long mem_buffer = 0;
	long mem_cache = 0;
	//el que servira como variable para la memoria reali utilizada
	long mem_usada = 0;
	long mem_usada1 = 0;
	float porcentaje;
	long mem_porcentual_usada = 0;
	


	//LINEA HASTA MIENTRAS PARA PROBAR
	/*mem_ocupada = info_mem_status(RamInfoFile,"MemFree:");
	printf("La memoria ocupada es: %ld\n", mem_ocupada);
	
	printf("Si se llega aqui es que no se llama a la funcion para obtener datos de la mem\n");

	info_cpu_status(CpuInfoFile);*/

	struct Proceso pr;
	pr.memoria_total = mem_total;
	pr.memoria_free = mem_free;
	pr.memoria_avaliable = mem_avaliable;
	pr.memoria_buffer = mem_buffer;
	pr.memoria_cache = mem_cache;
	struct Proceso *pr1 = &pr;
	
	do{
		printf("Memoria Total: %ld Memeoria free: %ld Memoria avaliable: %ld Memoria buffer: %ld Memoria cache: %ld \n", mem_total, mem_free, mem_avaliable, mem_buffer, mem_cache);
		//mem_free = info_mem_status_free_mem(RamInfoFile, "MemFree:");
		mem_usada = mem_total - mem_free - mem_buffer - mem_cache;
		porcentaje = (double)mem_usada / mem_total * 100;
		printf("Memoria usada %.2f \n", porcentaje);

		//obtiene el uso de la memoria ram que se esta ocupando
		info_cpu_status(filename_info_cpu);
		//Pasandi como parametro el nombre que se quiere y el archibo que va a leer
		mem_total = info_mem_status_total_mem(RamInfoFile, "MemTotal:");
		mem_free = info_mem_status_total_mem(RamInfoFile, "MemFree:");
		mem_avaliable = info_mem_status_total_mem(RamInfoFile, "MemAvaliable:");
		mem_buffer = info_mem_status_total_mem(RamInfoFile, "Buffers:");
		mem_cache = info_mem_status_total_mem(RamInfoFile, "Cached:");
		
		
		//mem_buffer = info_mem_status_buffer_mem(RamInfoFile, "MemAvaliable:");
		usleep(periodo);
		periodo = 0;

		

 		//Obtiene el uso del cpu llamando al cpu
 		//AQIU PONER EL PARAMETRO QUE LLAMA AL CPU
 		
 		
	}while(mem_free > 0);

	printf("%ld \n", pr1 -> memoria_free);
	printf("%ld \n", pr1 -> memoria_buffer);

	//Cerrando los dos archivos
	//fclose(CpuInfoFile);
	fclose(RamInfoFile);
	
	return 0;
	
}

//Funcion que va allamar a la memoria total 
long info_mem_status_total_mem(FILE *file, char *field){
	char line[MAXLINE]; //linea del archivo
	char field_name[MAXLINE]; //campo de la linea de MemTotal
	long total_mem = 0;



		
	printf("Si se entra a la funcion info_mem_status_total_mem\n");

	rewind(file);

	int n;

	while(fgets(line, MAXLINE, file) > 0){
		n = sscanf(line, "%s %ld Kb", field_name, &total_mem);
		printf("field_name en info_mem_status_total_mem es: %s\n", field_name);
		printf("La line en info_mem_status_total_mem es: %s\n", line);
		printf("Que es n: %d", n);
		if(n == 2)
			if(strcmp(field_name, field) == 0)
				return total_mem; 
	}
	return -1;	
}


//Funcion para leer el archivo stat del cpu, se pasa el archivo CpuInfoFile
int info_cpu_status(char *file){
	char str[MAXLINE];
	char field_name[MAXLINE];
	const char d[2] = " "; //Esto para hacer una sublinea de la linea partiendo desde la segunda elemnto del indice, borrando cpu
	char *token;
	int i=0;
	long int sum=0, idle, lastSum=0, lastIdle=0;
	long double idleFraction;
	int n;
	int times= 50;

	printf("Entra al archivo del Cpu\n");
	printf("Ruta: %s\n",file);

	FILE *fp = fopen("/proc/stat", "r");
	fgets(str, 100,fp);
	fclose(fp);
	token = strtok(str, d);
	sum = 0;
	printf("Linea:%s\n",str);
	printf("Token:%s\n", token);
	//token = strtok(line, d);
	//printf("Impresion de la linea: %s\n", token);
	while(token != NULL)
		token = strtok(NULL, d);
		printf("Token sin el cpu:%s/n", token);
		if(token != NULL)
			sum += atoi(token); 
		if(i==3)
			idle = atoi(token);
		i++;


	idleFraction = 100 - (idle-lastIdle)*100.0/(sum-lastSum);
	printf("Busy for: %Lf %% of the time", idleFraction);

	lastIdle = idle;
	lastSum = sum;
return 0;

	
	/*while(fgets(line, MAXLINE, file) > 0){
		printf("Entra en el while");
		if((line[0] == 'c') && (line[1] == 'p') && (line[2] == 'u')){
			token = strtok(line, d);
			sum = 0;
			while(token != NULL){
				token = strtok(NULL,d);
				if(token!=NULL){
					sum+=atoi(token);
				}
				if(i == 3){
					idle=atoi(token);
				}
				i++;
			}
			idleFraction = 100 - (idle-lastIdle)*100.0/(sum-lastSum);
			printf("Busy for: %Lf %% of the time", idleFraction);

			lastIdle = idle;
			lastSum = sum;
			return 0;
		}
	}
	return -1;*/
}
