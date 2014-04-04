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
	int termine;			  /*retour de vers_application, permettant de déterminer la fin des échanges*/
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

		/*Vérification de la somme de controle*/
		if(verifier_controle(trame))
		{
			/*Vérification des numéros de séquences (precédent et actuel)*/
			if(trame.num_seq != numero_sequence)
			{
				/*Construction trame à envoyer*/
				taille_msg = trame.lg_info;

				for(i=0;i<taille_msg;i++){
					message[i] = trame.info[i]; 
				}

				/*Notification à l'application que des données on été reçues*/
				termine = vers_application(L_UNIT_DATA_ind, message, taille_msg);

				/*Mise à jour du numéro de séquence que l'on comparera lors de la prochaine réception*/
				numero_sequence = trame.num_seq;
			}
			
			/* remise des données à la couche application */
			vers_canal(&trame_ACK);
		}
		
	} while(termine == 0);

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;

}

