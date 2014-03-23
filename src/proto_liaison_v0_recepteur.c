/*********************************************
 * protocole_liaison_00 - côté récepteur     * 
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
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
	char message[MTU];      /* message de l'application */
	int taille_msg;         /* taille du message */
	trame_t trame;          /* trame utilisée par le protocole liaison */
		
	init_physique(RECEPTION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");

	/* récupération d'une trame de la couche physique */
	de_canal(&trame);

	/* A FAIRE... lire données */		
	
	/* remise des données à la couche application */
	vers_application(L_UNIT_DATA_ind, message, taille_msg);

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;

}

