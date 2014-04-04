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
	char message[MTU];     			/* message de l'application */
	int taille_msg;        			/* taille du message */
	int service_liaison;  			/* service de la couche liaison invoqué par appli */
	trame_t trame;         			/* trame utilisée par le protocole liaison */
	trame_t trame_recepteur;	    /* trame venant du recepteur */	
	trame_t trame_deconnexion;		/*trame de demande de déconnexion à envoyer au recepteur*/
	int numTimerTrame=0;	   		/* numéro timer/trame */
	int nbRetrans=0;	   			/* nombre de retransmission */
	int nbMax_Retrans=10;  			/* nombre maximum de retransmission */
	int numero_sequence=0;			/*num_seq que l'on incrémentera pour toute nouvelle trame à envoyer*/

	init_physique(EMISSION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");
	
	de_application(&service_liaison, message, &taille_msg);
	//printf("Service liaison : %d.\n",service_liaison);
	
	while(service_liaison != L_DISCONNECT_req && nbRetrans <= nbMax_Retrans){	

		switch(service_liaison)
			{
				/*Connexion*/
				case L_CONNECT_req: 
					if(numTimerTrame ==0) trame.type = CON_REQ;
					
					vers_canal(&trame);
					printf("Envoi trame demande connexion.\n");

					nbRetrans++;
				break;
				
				/*Transfert d'informations*/
				case L_DATA_req: 
					trame.type = DATA;
					trame.lg_info = taille_msg;
					strcpy(trame.info, message);
					trame.num_seq = numero_sequence;
					trame.fcs = generer_controle(trame);

					/* remise à la couche physique pour emission */
					vers_canal(&trame);
					printf("Envoi de données.\n");

					nbRetrans++;
					numero_sequence++;
				break;

				default:
				printf("[DL] Service liaison inconnu !\n");
				
			}


		/*Gestion de la reception de trame*/

		depart_compteur(1, 200);
		numTimerTrame = attendre();
		
		/*Si trame reçue on la récupère*/
		if(numTimerTrame == 0){
			arreter_compteur(1);	
			de_canal(&trame_recepteur);

			/*Analyse du type de trame reçue*/
			switch(trame_recepteur.type)
			{
				/*Demande de conexion acceptée*/
				case CON_ACCEPT:
					vers_application(L_CONNECT_conf_ACCEPT, trame_recepteur.info, trame_recepteur.lg_info);
				break;

				/*Acquittement*/
				case ACK:
					nbRetrans = 0;
			}

			de_application(&service_liaison, message, &taille_msg);
		}

		/*Si tramne npn reçue on décrémente le numéro de séquence pour renvoyer la même trame*/
		else numero_sequence--;

	}


	if(nbRetrans == nbMax_Retrans) 
		printf("Nombre de retransmission maxmimum atteint.\n");


/*Déconnexion*/

	/*Préparation et envoi trame déconnexion*/
	trame_deconnexion.type = CON_CLOSE;
	vers_canal(&trame_deconnexion);

	/*Attente de l'acquittement du récepteur*/
	depart_compteur(1, 200);
	numTimerTrame = attendre();

	/*Tant qu'on a pas reçu de trame et que le nombre de retransmission max n'est pas atteint*/
	while(numTimerTrame != 0 && nbRetrans < nbMax_Retrans){
		vers_canal(&trame_deconnexion);
		depart_compteur(1, 200);
		numTimerTrame = attendre();
		nbRetrans++;
	}

	/*Si trame reçu on vérifie que c'est un acquittement de déconnexion*/
	if(numTimerTrame == 0)
	{
		arreter_compteur(1);
		de_canal(&trame_recepteur);

		if(trame_recepteur.type != CON_CLOSE_ACK){
			printf("Problème déconnexion côté récepteur, mauvaise trame renvoyée.\n");
		}

		else
		{
			printf("Déconnexion réussie.\n");
		}
	}

	else if(nbRetrans == nbMax_Retrans){
		printf("Nombre de retransmission maxmimum atteint.\n");
	}
	

	printf("[DL] Fin execution protocole liaison.\n");
	return 0;
}

