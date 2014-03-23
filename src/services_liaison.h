#ifndef __SERVICES_LIAISON_H__
#define __SERVICES_LIAISON_H__

/**********************************************************
 * MTU (Maximum Transmission Unit) de la couche liaison   *
 * (taille connue de la couche application)               *
 **********************************************************/
#define MTU 96

/***************************************************
 * Primitives de service pour le mode non connecté *
 ***************************************************/
/* Remarque : dans le mode non connecté ces primitives
   sont implicites via "de" et "vers" application */

/* transfert d'une unité de données */
#define L_UNIT_DATA_req  1
/* notification de la réception d'une unité de données */
#define L_UNIT_DATA_ind  2

/***********************************************
 * Primitives de service pour le mode connecté *
 ***********************************************/

/* demande d'établissement de connexion */
#define L_CONNECT_req          10
/* notification d'une demande d'établissement de connexion */  
#define L_CONNECT_ind          11 
/* réponse à la demande de connexion : acceptation */
#define L_CONNECT_resp_ACCEPT  12 
/* réponse à la demande de connexion : refus */
#define L_CONNECT_resp_REFUSE  13
/* notifier la réponse à la demande de connexion : acceptation */
#define L_CONNECT_conf_ACCEPT  14  
/* notifier la réponse à la demande de connexion : refus */
#define L_CONNECT_conf_REFUSE  15
/* transfert d'une unité de données au sein d'une connexion */
#define L_DATA_req             16
/* notification de la réception d'une unité de données au sein d'une connex. */
#define L_DATA_ind             17
/* fermeture de connexion */
#define L_DISCONNECT_req       18
/* notification de fermeture de connexion */
#define L_DISCONNECT_ind       19


#endif
