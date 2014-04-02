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
	trame_t trame; 			  /* trame utilisée par le protocole liaison */
	trame_t trame_ACK;		  /* trame d'acquittement */
	int termine;
	int numero_sequence = -1;
	int i;

	trame_ACK.type = ACK;

	init_physique(RECEPTION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");

	do {	
		/* récupération d'une trame de la couche physique */
		attendre();
		de_canal(&trame);

		
		if(verifier_controle(trame))
		{
			if(trame.num_seq != numero_sequence)
			{
				printf("Numéros de sequence différents.\n");
				taille_msg = trame.lg_info;

				for(i=0;i<taille_msg;i++){
					message[i] = trame.info[i]; 
				}

				termine = vers_application(L_UNIT_DATA_ind, message, taille_msg);
				numero_sequence = trame.num_seq;
			}
			
			/* remise des données à la couche application */
			vers_canal(&trame_ACK);
		}
		
	} while(termine == 0);

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;

}

