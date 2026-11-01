#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

struct cpuflags {
	uint64_t user;
	uint64_t nice;
	uint64_t system;
	uint64_t idle;
	uint64_t iowait;
	uint64_t irq;
	uint64_t softirq;
	uint64_t steal;
};

typedef struct cpuflags cpuF;

float memoria();
float processador();
float swap();

int main() {

	while(1) {
		printf("Uso de CPU: %.2f%% \n", processador());
		printf("Uso de Memoria: %.2f%% \n", memoria());
		printf("Uso de Swap: %.2f%% \n", swap());
		sleep(2);
		printf("\n");	
	}
}

float memoria() {
	FILE *f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return -1;
	}

	double total, free, available;
	float uso = 0;

	fscanf(f, "%*s %lf %*s", &total);
	fscanf(f, "%*s %lf %*s", &free);
	fscanf(f, "%*s %lf %*s", &available);

	total = total / (1024.0 * 1024.0);
	free = free / (1024.0 * 1024.0);
	available = available / (1024.0 * 1024.0);

	uso = ((total - available) / total) * 100;

	fclose(f);
	return uso;
}

float processador() {
	FILE *f = fopen("/proc/stat", "r");

	if (f == NULL) {
		return -1;
	}
	
	cpuF c1 = {0};
	cpuF c2 = {0};

	fscanf(f, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %*llu %*llu", &c1.user, &c1.nice, &c1.system, &c1.idle,
			&c1.iowait, &c1.irq, &c1.softirq, &c1.steal);


	fclose(f);
	sleep(1);

	FILE *g = fopen("/proc/stat", "r");
		
	fscanf(g, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %*llu %*llu", &c2.user, &c2.nice, &c2.system, &c2.idle,
			&c2.iowait, &c2.irq, &c2.softirq, &c2.steal);

	fclose(g);

	int ocioso1, ocioso2;
	int ativo1, ativo2;
	int total1, total2;
	int dif, difOcioso;
	float uso = 0.0;

	ocioso1 = c1.idle + c1.iowait;
	ocioso2 = c2.idle + c2.iowait;

	ativo1 = c1.user + c1.nice + c1.system + c1.irq + c1.softirq + c1.steal;
	ativo2 = c2.user + c2.nice + c2.system + c2.irq + c2.softirq + c2.steal;

	total1 = ativo1 + ocioso1;
	total2 = ativo2 + ocioso2;

	dif = total2 - total1;
	difOcioso = ocioso2 - ocioso1;

	uso = ((float) (dif - difOcioso) / (float) (dif)) * 100.0;
	return uso;
}

float swap() {
	FILE *f = fopen("/proc/meminfo", "r");
	char *teste = malloc(sizeof(char) * 32);
	
	double swapTotal = 0;
	double swapFree = 0;
	float uso = 0;

	while (fscanf(f, "%s %lf %*s", teste, &swapTotal) != EOF) {
		if (strcmp(teste, "SwapTotal:") == 0) {
			break;
		}
	}
	rewind(f);
	while (fscanf(f, "%s %lf %*s", teste, &swapFree) != EOF) {
		if (strcmp(teste, "SwapFree:") == 0) {
			break;
		}
	}

	fclose(f);
	free(teste);

	if (swapTotal == 0) {
		return 0;
	}

	swapTotal = swapTotal / (1024.0 * 1024.0);
	swapFree = swapFree / (1024.0 * 1024.0);
	
	uso = ((swapTotal - swapFree) / swapTotal) * 100; 
	return uso;
}
