#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <stdint.h>

/****************************************
 * Interface avec la couche application *
 ****************************************/

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
void de_application(int *service_liaison, char *donnees, int *taille_msg);


/*
 * Remise de données à la couche application.
 * Paramètres (en entrée):
 *   - service_liaison : service fourni par la couche liaison
 *                       (cf. primitives de services_liaison.h)
 *   - donnees : données reçues par la liaison, remontées à l'application
 *   - taille_msg : taille des données reçues
 * Retourne :
 *   - En mode non connecté, avec sercice_liaison = L_UNIT_DATA_ind
 *      -> 1 si le récepteur n'a plus rien à écrire (fichier terminé)
 *      -> 0 sinon
 *   - En mode connecté, avec service_liaison = L_CONNECT_ind
 *      -> La réponse à la demande de connexion
 *         (L_CONNECT_resp_ACCEPT ou L_CONNECT_resp_REFUSE)
 */
int vers_application(int service_liaison, char *donnees, int taille_msg);

#endif
