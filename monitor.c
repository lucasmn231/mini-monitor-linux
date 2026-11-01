#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


struct cpuflags {
	int user;
	int nice;
	int system;
	int idle;
	int iowait;
	int irq;
	int softirq;
	int steal;
};

typedef struct cpuflags cpuF;

void memoria();


float processador() {
	FILE *f = fopen("/proc/stat", "r");

	if (f == NULL) {
		return -1;
	}
	
	cpuF c1 = {0};
	cpuF c2 = {0};

	fscanf(f, "%*s %d %d %d %d %d %d %d %d %*d %*d", &c1.user, &c1.nice, &c1.system, &c1.idle,
			&c1.iowait, &c1.irq, &c1.softirq, &c1.steal);


	fclose(f);
	sleep(1);

	FILE *g = fopen("/proc/stat", "r");
		
	fscanf(g, "%*s %d %d %d %d %d %d %d %d %*d %*d", &c2.user, &c2.nice, &c2.system, &c2.idle,
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

int main() {
	int i = 0;
	while (i < 15) {
		FILE *f = fopen("dados.txt", "w");
		fprintf(f, "CPU: %.2f%%", processador());
		sleep(2);
		i++;
		fclose(f);

	}
	return 0;
}



void memoria() {
	FILE *f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return;
	}

	double total, free, available, usada;
	float porcento;

	fscanf(f, "%*s %lf %*s", &total);
	fscanf(f, "%*s %lf %*s", &free);
	fscanf(f, "%*s %lf %*s", &available);

	total = total / (1024.0 * 1024.0);
	free = free / (1024.0 * 1024.0);
	available = available / (1024.0 * 1024.0);

	usada = total - available;
	porcento = (usada / total) * 100.0;

	printf("Total: %.2lf Gib\n", total);
	printf("Usada: %.2lf Gib (%.1f%%)\n", usada, porcento);	

	fclose(f);
	return;
}
