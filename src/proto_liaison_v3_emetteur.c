/*********************************************
 * protocole_liaison_00 - côté émetteur      * 
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
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
	char message[MTU];     /* message de l'application */
	int taille_msg;        /* taille du message */
	int service_liaison;   /* service de la couche liaison invoqué par appli */
	trame_t trame;         /* trame utilisée par le protocole liaison */
	//trame_t trame_ACK;	   /* trame d'acquittement reçue */	
	int numTimerTrame;	   /* numéro timer/trame */
	int nbRetrans=0;	   /* nombre de retransmission */
	int nbMax_Retrans=10;  /* nombre maximum de retransmission */
	int numero_sequence=0;

	init_physique(EMISSION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");
	
	de_application(&service_liaison, message, &taille_msg);
	
	do {	
		/* lecture primitive de service */
		if (service_liaison == L_UNIT_DATA_req) {
			
			trame.lg_info = taille_msg;
			strcpy(trame.info, message);
			trame.fcs = generer_controle(trame);
			trame.type = DATA;
			trame.num_seq = numero_sequence;
			numero_sequence++;
			
			/* remise à la couche physique pour emission */
			vers_canal(&trame);

		}
		else {
			printf("[DL] Service liaison inconnu !\n");
		}

		depart_compteur(1, 500);
		numTimerTrame = attendre();
		
		if(numTimerTrame == 0){
			attendre();
			arreter_compteur(1);	
			de_application(&service_liaison, message, &taille_msg);
		}
		
		else{
			nbRetrans ++;
		}

		if(nbRetrans == nbMax_Retrans) 
			printf("Nombre de retransmission maxmimum atteint.\n");

	} while(taille_msg > 0 && nbRetrans <= nbMax_Retrans);

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;
}

