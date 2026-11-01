#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void memoria();

int main() {

	while (1) {
		system("clear");
		printf("--- Memoria --- \n");
		memoria();
		sleep(1);
	}


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
