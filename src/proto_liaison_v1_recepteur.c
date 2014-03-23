/*********************************************
 * protocole_liaison_00 - côté récepteur     * 
 *                                           *
 * Exemple INCOMPLET de protocole liaison    *
 *                                           *
 *********************************************/
 
#include <stdio.h>
 #include <string.h>
#include "application.h"
#include "services_liaison.h"
#include "couche_liaison.h"
#include "services_physique.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
	char message[MTU+1];      /* message de l'application */
	int taille_msg=0;         /* taille du message */
	trame_t trame;          /* trame utilisée par le protocole liaison */
		
	init_physique(RECEPTION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");

	do {	
		/* récupération d'une trame de la couche physique */
		de_canal(&trame);

		taille_msg = trame.lg_info;
		strcpy(message, trame.info);		
		
		/* remise des données à la couche application */
		vers_application(L_UNIT_DATA_ind, message, taille_msg);
		
		
	} while(taille_msg > 0);

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;

}

