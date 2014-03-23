/************************************************************
 *      Simulation d'une couche PHYSIQUE                    * 
 *                                                          *
 * (v 1.2)                                                  *
 * P. Torguet & E. Lavinal                                  *
 * Université de Toulouse III - Paul Sabatier               *
 ************************************************************/

#ifndef __PHYSIQUE_H__
#define __PHYSIQUE_H__

#include "couche_liaison.h" /* trame_t */

/* pour init_physique() */
#define EMISSION  0
#define RECEPTION 1

/* pour attendre() */
#define TRAME_RECUE 0

/* ============================== */
/* Initialisation couche physique */
/* ============================== */

/************************************************************ 
 * Initialisation de le couche physique. Le paramètre rôle  *
 * doit être égal à 1 pour la réception, 0 pour l'émission. * 
 ************************************************************/
void init_physique(int role);

/* Initialisation avec un taux de perte,
 *                        taux d'erreur et
 *                        des parametres reseau
 */
void initialisation(float proba_perte, float proba_erreur, unsigned short port_local, char* destination, unsigned short port_destination);


/* ============================================================= */
/* Primitives de service pour émission et réception sur le canal */
/* ============================================================= */

/**********************************************************
 * Remet une trame à la couche physique pour émission     *
 * sur le support de communication.                       *
 **********************************************************/
void vers_canal(trame_t *trame);

/*************************************************************
 * Prélève une trame de la couche physique (N.B. : fonction  *
 * bloquante tant qu'une trame n'est pas reçue).             *
 *************************************************************/
void de_canal(trame_t *trame);

/* =============================================== */
/* Fonctions utilitaires pour la gestion de timers */
/* =============================================== */

/***************************************************************
 * Démarre le timer numero n (0 < n < 100) qui s'arrête après  *
 * ms millisecondes (ms doit être un multiple de 100)          *
 ***************************************************************/
void depart_compteur(int n, int ms);

/*******************************************
 * Arrête le timer numéro n (0 < n < 100)  *
 *******************************************/
void arreter_compteur(int n);

/*********************************************************************
 * Fonction qui attend un évènement (trame reçue ou timeout).        *
 * (N.B. : fonction bloquante)                                       *
 * Retourne : 0 si une trame reçue est disponible,                   *
 *            un numéro de timer (1-100) si un timeout a été généré  *
 *********************************************************************/
int attendre();

#endif
