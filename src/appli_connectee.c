/************************************************************
 *      APPLICATION de transfert de fichiers                * 
 *                                                          *
 *  --> L'application s'appuie sur les services de          *
 *      transmission de la couche liaison en mode           *
 *      connecté                                            * 
 *                                                          *
 * E. Lavinal - Univ. de Toulouse III - Paul Sabatier       *
 ************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include "services_liaison.h"
#include "config.h"

#define NON_CONNECTE 0
#define EN_COURS 1
#define CONNECTE 2

static int etat_connexion = NON_CONNECTE;
static FILE *fichier = NULL;

/*
 * Lecture de données émanant de la couche application. Ces données
 * doivent être traitées selon un certain service exigé par l'application
 * via l'argument service_liaison.
 * Paramètres (en sortie):
 *   - service_liaison : service demandé par l'application
 *                       (cf. primitives de services_liaison.h)
 *   - donnees : message devant être envoyé (issu de la couche application)
 *   - taille_msg : nombre d'octets de données à émettre (0 si pas de données)
 */
void de_application(int *service_liaison, char *donnees, int *taille_msg)
{

	switch (etat_connexion) {
	
		case NON_CONNECTE:
			/* demande de connexion */
			*service_liaison = L_CONNECT_req;
			*taille_msg = 0;
			printf("[APP] Demande de connexion liaison...\n");
			etat_connexion = EN_COURS;
			break;
			
		case CONNECTE:
			if ( !feof(fichier) ) {
				/* lecture du fichier, au max MTU données */
				/* fread(void *restrict ptr, size_t size, size_t nitems,
				* FILE *restrict stream); */
				*taille_msg = fread(donnees, 1, MTU, fichier);
				*service_liaison = L_DATA_req;
				printf("\n[APP] Lecture fichier.\n");
			}
			else {
				/* fin du fichier donc déconnexion */
				fclose(fichier);
				*service_liaison = L_DISCONNECT_req;
				*taille_msg = 0;
				printf("[APP] Fin du fichier, demande de déconnexion.\n");
				etat_connexion = NON_CONNECTE;
			}
			break;
			
		case EN_COURS:
			printf("[APP] Patientez, en cours de connexion...");
			break;
			
		default:
			printf("[APP] Problème état de la connexion !");	
			
	} /* switch */ 
}

/*
 * Remise de données à la couche application.
 * Paramètres (en entrée):
 *   - service_liaison : service fourni par la couche liaison
 *                       (cf. primitives de services_liaison.h)
 *   - donnees : données reçues par la liaison, remontées à l'application
 *   - taille_msg : taille des données reçues
 * Retourne :
 *   - En mode connecté, avec service_liaison = L_CONNECT_ind
 *      -> La réponse à la demande de connexion
 *         (L_CONNECT_resp_ACCEPT ou L_CONNECT_resp_REFUSE)
 */
int vers_application(int service_liaison, char *donnees, int taille_msg)
{
	char nom_fichier[30];
	
	switch (service_liaison) {
	
		case L_CONNECT_ind:
		
			if (etat_connexion == NON_CONNECTE) {
				/* demande de connexion reçue. Par défaut, on accepte. */
				printf("[APP] Demande de connexion recue (on accepte)\n");
				/* ouverture du fichier en ecriture */
				conf_fichier_reception(nom_fichier);
    			fichier = fopen(nom_fichier, "w");
    			if (fichier == NULL) {
					perror("[APP] Probleme ouverture fichier en écriture\n");
					return L_CONNECT_resp_REFUSE;
    			}
    			etat_connexion = CONNECTE;
    			return L_CONNECT_resp_ACCEPT; /* acceptation de connexion */
			}
			else {
				printf("[APP] ATTENTION, demande de connexion recue dans\
						un état incohérent !\n");
				return -1;
			}
			break;
			
		case L_CONNECT_conf_ACCEPT:
	
			if (etat_connexion == EN_COURS) {
				/* réponse à la demande de connexion -> OK */
				printf("[APP] Réponse demande de connexion : acceptée.\n");
    			/* ouverture du fichier en lecture */
				conf_fichier_emission(nom_fichier);
				fichier = fopen(nom_fichier, "r");
    			if (fichier == NULL) {
        			perror("[APP] Probleme ouverture fichier en lecture\n");
    	    		exit(1);
    			}
    			etat_connexion = CONNECTE;
    		}
    		else {
				printf("[APP] ATTENTION, réponse de connexion recue dans\
						un état incohérent !\n");
				return -1;
			}
			break;
			
		case L_CONNECT_conf_REFUSE:
		
			if (etat_connexion == EN_COURS) {
				/* réponse à la demande de connexion -> NOT OK */
    			printf("[APP] Réponse demande de connexion : refusée.\n");
    			exit(1);
    		}
    		else {
    			printf("[APP] ATTENTION, réponse de connexion recue dans\
						un état incohérent !\n");
				return -1;
    		}
			break;
			
		case L_DATA_ind:
		
			if (etat_connexion == CONNECTE) {
				/* données reçues */
				if (fichier == NULL) {
					perror("[APP] Données reçues mais fichier non ouvert !\n");
					exit(1);
				}
				/* ecriture des données dans le fichier */
				printf("[APP] Ecriture fichier.\n\n");
				fwrite(donnees, 1, taille_msg, fichier);
    			fflush(fichier);
    		}
    		else {
    			printf("[APP] ATTENTION, données recues hors connexion !\n");
				return -1;
    		}
			break;
			
		case L_DISCONNECT_ind:
		
			if (etat_connexion == CONNECTE) {
				/* deconnexion recue */
				fclose(fichier);
				printf("[APP] Déconnexion recue. Fichier fermé.\n");
				etat_connexion = NON_CONNECTE;
    		}
    		else {
    			printf("[APP] ATTENTION, déconnexion recue hors connexion !\n");
				return -1;
    		}
			break;
						
		default:
			/* service liaison inconnu */
			printf("[APP] vers_utilisateur: service liaison inconnu !\n");
	} /* switch */
	return 0;
}

