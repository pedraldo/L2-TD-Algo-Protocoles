#ifndef __COUCHE_LIAISON_H__
#define __COUCHE_LIAISON_H__

#include <stdint.h> /* uint8 */
#include "services_liaison.h" /* pour constante MTU */

/*************************
 * Structure d'une trame *
 *************************/

typedef struct trame_s {
  uint8_t type;      /* type de trame, cf. ci-dessous */
  uint8_t num_seq;   /* numéro de séquence */
  uint8_t lg_info;   /* longueur du champ info */
  char info[MTU];    /* données utiles de la trame */
  char fcs;          /* somme de contrôle */
} trame_t;

/******************
 * Types de trame *
 ******************/
#define CON_REQ       0  /* demande d'établissement de connexion */
#define CON_ACCEPT    1  /* acceptation de connexion */
#define CON_REFUSE    2  /* refus d'établissement de connexion */
#define CON_CLOSE     3  /* notification de déconnexion */
#define CON_CLOSE_ACK 4  /* accusé de réception de la déconnexion */
#define DATA          5  /* données de l'application */
#define ACK           6  /* accusé de réception des données */
#define OTHER         7  /* extensions */




#endif


char generer_controle(trame_t trame);

int verifier_controle(trame_t trame);
