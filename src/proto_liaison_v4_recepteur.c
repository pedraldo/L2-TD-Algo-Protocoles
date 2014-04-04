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
	char message[MTU];      			/* message de l'application */
	int taille_msg=0;        		    /* taille du message */
	trame_t trame; 			  			/* trame utilisée par le protocole liaison */
	trame_t trame_vers_emetteur;	    /* trame d'acquittement */
	int reponse_connexion;				/*réponse du recepteur à la demande de connexion*/
	int numero_sequence = -1;			/*numéro séquence trame reçue*/
	int type_trame_vers_app;			/*type trame à envoyer à l'app*/
	int type_trame_vers_canal;			/*type trame à envoyer au canal*/
	int i;


	init_physique(RECEPTION);

	printf("[DL] Initialisation physique : OK.\n");
	printf("[DL] Debut execution protocole liaison.\n");

	 while(trame.type != CON_CLOSE){	
		/* récupération d'une trame de la couche physique */
		attendre();
		de_canal(&trame);

			/*Traitement en fonction du type de trame reçue*/
			switch(trame.type)
			{
				/*Réception d'une demande de connexion*/
				case CON_REQ: 
					type_trame_vers_app = L_CONNECT_ind;
					printf("Reception demande de connexion.\n");
					reponse_connexion = vers_application(type_trame_vers_app, message, taille_msg);

					/*Traitement de la réponse de la demande de connexion venant de la couche app*/
					if(reponse_connexion == L_CONNECT_resp_ACCEPT){
						type_trame_vers_canal = CON_ACCEPT;
						printf("Envoi acceptation demande de connexion.\n");
					}
					else{
						type_trame_vers_canal = CON_REFUSE;
						printf("Envoi refus demande de connexion.\n");
					}
					trame_vers_emetteur.type = type_trame_vers_canal;
					
					/*Envoi de la réponse à l'émetteur*/
					vers_canal(&trame_vers_emetteur);
				break;

				/*Réception de données*/
				case DATA: 

					/*Vérification somme de contrôle*/
					if(verifier_controle(trame))
					{
						type_trame_vers_canal = ACK;
						trame_vers_emetteur.type = type_trame_vers_canal;

						/*Comparaison numéro de séquence avec la trame précédemment reçue*/
						if(trame.num_seq != numero_sequence)
						{
							printf("Numéros de sequence différents.\n");
							taille_msg = trame.lg_info;

							for(i=0;i<taille_msg;i++){
								message[i] = trame.info[i]; 
							}
						}
					}

					type_trame_vers_app = L_DATA_ind;

					/*Envoi acquittement à l'émetteur*/
					vers_canal(&trame_vers_emetteur);

					/*Notification de réception de données à la couche app*/
					vers_application(type_trame_vers_app, message, taille_msg);

					/*Stockage du numéro de séquence reçu pour futur comparaison*/
					numero_sequence = trame.num_seq;
				break;

				default:
				printf("Type de trame reçu inconnu.\n");
				
			}

		}


/*Déconnexion*/
		type_trame_vers_app = L_DISCONNECT_ind;
		type_trame_vers_canal = CON_CLOSE_ACK;
		printf("Reception demande de déconnexion.\n");
		printf("Acquittement demande de déconnexion.\n");

		vers_application(type_trame_vers_app, trame.info, trame.lg_info);
		trame_vers_emetteur.type = type_trame_vers_canal;
		vers_canal(&trame_vers_emetteur);
	

		printf("[DL] Fin execution protocole liaison.\n");
		return 0;

}

