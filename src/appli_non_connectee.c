/************************************************************
 *      APPLICATION de transfert de fichiers                * 
 *                                                          *
 *  --> L'application s'appuie sur les services de          *
 *      transmission de la couche liaison en mode           *
 *      non connecté                                        * 
 *                                                          *
 * E. Lavinal - Univ. de Toulouse III - Paul Sabatier       *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "services_liaison.h"
#include "config.h"

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
void de_application(int *service_liaison, char *message, int *taille_msg)
{	
	/* test état fichier */
	if (fichier == NULL) {
		/* fichier non ouvert, ouverture en lecture */
		char nom_fichier[30];
		conf_fichier_emission(nom_fichier);
	    fichier = fopen(nom_fichier, "r");
    	if (fichier == NULL) {
        	perror("[APP] Problème ouverture fichier en lecture !\n");
        	exit(1);
    	}
    }
	
	if ( !feof(fichier) ) {
		/* lecture du fichier, au max MTU données */
		/* fread(void *restrict ptr, size_t size, size_t nitems,
		 *       FILE *restrict stream); */
		*taille_msg = fread(message, 1, MTU, fichier);
		*service_liaison = L_UNIT_DATA_req;
		printf("\n[APP] Lecture fichier.\n");
	}
	else {
		*taille_msg = 0;
		printf("[APP] Fin du fichier.\n");
		fclose(fichier);
	}
}

/*
 * Remise de données à la couche application.
 * Paramètres (en entrée):
 *   - service_liaison : service fourni par la couche liaison
 *                       (cf. primitives de services_liaison.h)
 *   - donnees : données reçues par la liaison, remontées à l'application
 *   - taille_msg : taille des données reçues
 * Retourne :
 *   - En mode non connecté, avec sercice_liaison = L_UNIT_DATA_ind
 *      -> 1 si le récepteur n'a plus rien à écrire (fichier refermé)
 *      -> 0 sinon
 */
int vers_application(int service_liaison, char *message, int taille_msg)
{

	/* test état fichier */
	if (fichier == NULL) {
		/* fichier non ouvert, ouverture en écriture */
		char nom_fichier[30];
		conf_fichier_reception(nom_fichier);
	    fichier = fopen(nom_fichier, "w");
    	if (fichier == NULL) {
        	perror("[APP] Problème ouverture fichier en écriture !\n");
        	exit(1);
    	}
    }

	/* écriture des données dans le fichier */
	printf("[APP] Ecriture fichier.\n\n");
	fwrite(message, 1, taille_msg, fichier);
    fflush(fichier);
    
    if (taille_msg < MTU) {
    	/* c'etait la derniere partie du fichier car taille message < MTU */
    	/* ATTENTION HYPOTHESE FORTE...
    	   --> problème si la MTU est un multiple de la taille du message */
    	fclose(fichier);
    	printf("[APP] Fichier fermé.\n");
    	return 1;
    }
    return 0;
}

