/*
* Nombre      : RenderBG
* Autor       : Juan Lautaro Silva Ortíz, Rodrigo Alexis Venegas Muñoz
* fecha       : 03-02-2012
* Descripción : Aplicación que permite migrar procesos de renderizado a distintos equipos dentr de un cluster.
*
* Require     : gcc compiler and OpenSSL library
* Ejecutar    : gcc RenderGPUandCPU.c -o RenderBG
*
*
****************************************************************************
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA
*
****************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 12
#define MAXCPU 10

/*MAX = Cantidad de frames que se renderizan a la vez utilizando GPU, si lo modifica, considere la memoria dedicada a GPU*/
/*MAXCPU = Cantidad de frames que se renderizan a la vez, si lo modifica, considere la memoria RAM disponible*/

int main(int argc, char *argv[]){
	if(argc==5){
		system("date");
		int inicio = atoi(argv[3])-1; //Se aumenta en un frame al inicio, pero se elimina en el primer ciclo
		int final = atoi(argv[4]);
		int hijos=0;
		int flag=0;
		
		//Define message como la orden de renderizar solo en los nodos con mejor GPU
		char *bin = "mosrun -E -j2,3 blender -b -o ./ ";
		char *message = (char*) malloc(strlen(bin)+strlen(argv[2])+strlen(argv[1]));
		strcat(message, bin);
		strcat(message, argv[2]);
		strcat(message, " ");
		strcat(message, "-f ");
		
		//Define message como la orden de renderizar en todos los nodos
		char *bin2 = "mosrun -E -j2,3,4,5 blender -b -o ./ ";
		char *message2 = (char*) malloc(strlen(bin)+strlen(argv[2])+strlen(argv[1]));
		strcat(message2, bin2);
		strcat(message2, argv[2]);
		strcat(message2, " ");
		strcat(message2, "-f ");
		
		
		if(strcmp(argv[1], "GPU")==0){
		//GPU Render - Envia menos carga a los nodos con GPU de menos capacidad
		while(inicio<final){
			if(hijos<=MAX){
				inicio=inicio+1; //Eliminacion del frame inicial en cada ciclo
				if(fork()==0){
					char frame[4];
					sprintf(frame,"%d", inicio);
					
					if(flag==0){
					    strcat(message, frame);
					    printf("%s", message);//Muestra la llamada al sistema (fork) que se esta ejecutando
					    system(message);
					    system("date");
					    exit(1);
					}else{
					    strcat(message2, frame);
					    printf("%s", message2);//Muestra la llamada al sistema (fork) que se esta ejecutando
					    system(message2);
					    system("date");
					    exit(1);
					}
					
				}else{
					//printf("NO FORK\n");
					
				}
				if(flag==0) flag=1;
				else flag=0;
				hijos=hijos+1;
			}else{
				wait(NULL);
				hijos=hijos-1;
			}
		}
		
		}else{
		//CPU Render
		while(inicio<final){
			if(hijos<=MAXCPU){
				inicio=inicio+1; //Eliminacion del frame inicial en cada ciclo
				if(fork()==0){
					char frame[4];
					sprintf(frame,"%d", inicio);
					strcat(message2, frame);
					printf("%s", message2);
					system(message2);
				    	system("date");
					exit(1);
				}else{
					//printf("NO FORK\n");
					
				}
				hijos=hijos+1;
			}else{
				wait(NULL);
				hijos=hijos-1;
			}
		}
		}
	}else{
		printf("Modo de uso:\n");
		printf("RenderBG [CPU | GPU] [Archivo.blend] [frame inicial] [frame final]\n");
	}
// Generación automatica de video de los frames renderizados
	system("mencoder \"mf://*.jpg\" -mf fps=24 -o test.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=5000 -vf scale=1920:1080");

	system("echo \\ \\");
	return 0;
}
