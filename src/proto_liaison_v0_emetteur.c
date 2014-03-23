/*********************************************
 * protocole_liaison_00 - côté émetteur      * 
 *                                           *
 * Exemple INCOMPLET de protocole liaison    *
 *                                           *
 *********************************************/

#include <stdio.h>
#include "application.h"
#include "services_liaison.h"
#include "couche_liaison.h"
#include "services_physique.h"

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
	char message[MTU];     /* message de l'application */
	int taille_msg;        /* taille du message */
	int service_liaison;   /* service de la couche liaison invoqué par appli */
	trame_t trame;         /* trame utilisée par le protocole liaison */

	init_physique(EMISSION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");

	de_application(&service_liaison, message, &taille_msg);
		
	/* lecture primitive de service */
	if (service_liaison == L_UNIT_DATA_req) {
		
		/* A FAIRE... construire trame */
		
		/* remise à la couche physique pour emission */
		vers_canal(&trame);
	}
	else {
		printf("[DL] Service liaison inconnu !\n");
	}
	
	printf("[DL] Fin execution protocole liaison.\n");
	return 0;
}

