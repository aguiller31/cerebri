/**
 * @file velocity.h
 * @brief Fichier d'en-tête pour la gestion de la conduite automatique.
 *
 * @author Antoine Guillermin
 * @date 2024-01-20
 * @version 1.501
 */
#ifndef VELOCITY
#define VELOCITY

#include "casadi/gen/b3rb.h" ///< Inclut le fichier d'en-tête pour le générateur de b3rb.
#include "math.h" ///< Inclut le fichier d'en-tête pour les fonctions mathématiques.
#include <zephyr/logging/log.h> ///< Inclut le fichier d'en-tête pour la journalisation de Zephyr.

#include <zephyr/kernel.h> ///< Inclut le fichier d'en-tête pour récupérer le temps.

// Définitions des constantes
#define VECTOR_ON_THE_LEFT 1 ///< Définit le côté gauche pour un vecteur.
#define VECTOR_ON_THE_RIGHT 0 ///< Définit le côté droit pour un vecteur.
#define M_PI 3.14159265358979323846 ///< Définit la valeur de Pi.
#define IMAGE_WIDTH 78 ///< Définit la largeur de l'image.
#define IMAGE_HEIGHT 51 ///< Définit la hauteur de l'image.
#define STEER_MAX 0.6 ///< Définit la direction maximale.
#define SPEED_MAX 36.0 ///< Définit la vitesse maximale.
#define WHEEL_RADIUS 34 ///< Définit le rayon de la roue.
#define LEFT_OR_RIGHT_COEFF -1 ///< Définit le coefficient pour le côté gauche ou droit.
#define AUTO_START ///< Définit le démarrage automatique.

// Paramètres pour éliminer les vecteurs "mauvais"
#define MIN_VECTOR_LENGTH 20 ///< Utilisé pour éliminer les vecteurs inférieurs à cette taille (y1-y0), plus la valeur est basse moins il y aura de fusion (-1 <=> pas de fusion)
#define NO_LINE_TIME 3500 ///< Définit le temps sans ligne.
#define NB_OF_VECTS_TO_WAIT 1 ///< Définit le nombre de vecteurs à attendre.
#define MAX_DISTANCE_FOR_SAME_VECT 30 ///< Utilisé pour éliminer les vecteurs "aberrants" lorsqu'on calcule les moyennes de m0 et m1, plus la valeur est grande moins il y aura de vecteurs éliminés.
#define SAME_VECTOR_THRESHOLD 20 ///< Utilisé pour fusionner m0 et m1 s'ils sont "proches", plus la valeur est basse moins il y aura de fusion (-1 <=> pas de fusion).
#define IMAGE_WIDTH_THRESHOLD 0 ///< Définit le seuil de largeur de l'image.
#define MAX_VECTOR_ANGLE_THRESHOLD 60 ///< Utilisé pour supprimer les vecteurs qui ont un angle supérieur à ceci (en degré).

// Paramètres de vitesse en %
#define NO_VECTOR_SPEED 50.0 ///< Définit la vitesse sans vecteur.

// Paramètres de rotation en %
#define INCREASE_STEER_IF_NO_VECTORS 5.0 ///< Définit l'augmentation de la direction si aucun vecteur.
#define INCREASE_STEER_IF_NO_VECTORS_THRESHOLD 60.0 ///< Définit le seuil d'augmentation de la direction si aucun vecteur.

// Non modifiable
#define SPEED_FACTOR SPEED_MAX/100.0 ///< Facteur de vitesse, max speed = 36 et on divise par 100.0 car on a un coefficient en %.
#define STEER_FACTOR STEER_MAX/100.0 ///< Facteur de direction, max steer = 0.6 et on divise par 100.0 car on a un coefficient en %.

#include <zros/private/zros_node_struct.h> ///< Inclut le fichier d'en-tête pour la structure de noeud ZROS.
#include <zros/private/zros_pub_struct.h> ///< Inclut le fichier d'en-tête pour la structure de publication ZROS.
#include <zros/private/zros_sub_struct.h> ///< Inclut le fichier d'en-tête pour la structure de souscription ZROS.
#include <zros/zros_node.h> ///< Inclut le fichier d'en-tête pour le noeud ZROS.
#include <zros/zros_pub.h> ///< Inclut le fichier d'en-tête pour la publication ZROS.
#include <zros/zros_sub.h> ///< Inclut le fichier d'en-tête pour la souscription ZROS.

#include <cerebri/core/casadi.h> ///< Inclut le fichier d'en-tête pour le noyau Cerebri.
#include <stdio.h> ///< Inclut le fichier d'en-tête pour les fonctions d'entrée/sortie standard.
#include <math.h> ///< Inclut le fichier d'en-tête pour les fonctions mathématiques.
#include <stdlib.h> ///< Inclut le fichier d'en-tête pour les fonctions de la bibliothèque standard.
#include "mixing.h" ///< Inclut le fichier d'en-tête pour les fonctions de mixage.

#define MY_STACK_SIZE 3072 ///< Définit la taille de la pile.
#define MY_PRIORITY 4 ///< Définit la priorité.

LOG_MODULE_REGISTER(b3rb_velocity, CONFIG_CEREBRI_B3RB_LOG_LEVEL); ///< Enregistre le module de journalisation pour la vitesse b3rb.

// Direction à (%)        0    10    20    30    40    50    60    70    80    90   100   100
const steers[] = {0.30, 0.30, 0.30, 0.30, 0.30, 0.35, 0.40, 0.70, 0.80, 0.85, 0.90, 0.90}; ///< Tableau des valeurs de gain direction.
const speeds[] = {100.0,100.0,100.0,90.0, 80.0, 80.0, 70.0, 70.0, 60.0, 60.0, 60.0, 60.0}; ///< Tableau des valeurs de vitesse.

/**
 * @brief Structure pour un point.
 */
typedef struct _Point {
    double x; ///< Coordonnée x du point.
    double y; ///< Coordonnée y du point.
} Point;

/**
 * @brief Structure pour un vecteur.
 */
typedef struct _Vecteur {
    Point tail; ///< Queue du vecteur.
    Point head; ///< Tête du vecteur.
} Vecteur;

/**
 * @brief Structure pour un DVecteur.
 */
typedef struct _DVecteur {
    Vecteur m[2]; ///< Tableau de deux vecteurs.
} DVecteur;

/**
 * @brief Structure pour le contexte.
 */
typedef struct _context {
    struct zros_node node; ///< Noeud ZROS.
    synapse_msgs_Twist cmd_vel; ///< Commande de vitesse.
    synapse_msgs_Status status; ///< Statut.
    synapse_msgs_Actuators actuators; ///< Actionneurs.
    synapse_msgs_Actuators actuators_manual; ///< Actionneurs manuels.
    synapse_msgs_PixyVector pixy_vector; ///< Vecteur Pixy.
    struct zros_sub sub_status, sub_cmd_vel, sub_actuators_manual, sub_pixy_vector; ///< Souscriptions.
    struct zros_pub pub_actuators; ///< Publications.
    const double wheel_radius; ///< Rayon de la roue.
    const double wheel_base; ///< Base de la roue.
    int64_t no_line_time; ///< Temps sans ligne.
    double prev_steer; ///< Direction précédente.
    double prev_speed; ///< Vitesse précédente.
    int nb_of_vects_stored; ///< Nombre de vecteurs stockés.
    DVecteur oldVectors[NB_OF_VECTS_TO_WAIT]; ///< Anciens vecteurs.
    DVecteur first_vector; ///< Premier vecteur.
    int first_vector_saved; ///< Premier vecteur sauvegardé.
    double first_vector_angle_left; ///< Angle du premier vecteur à gauche.
    double first_vector_angle_right; ///< Angle du premier vecteur à droite.
    double first_vector_middle; ///< Milieu du premier vecteur.
    double beta_ref; ///< Référence beta.
    double sigma_ref; ///< Référence sigma.
    double head_distance_ref; ///< Référence de distance de tête.
    double tail_distance_ref; ///< Référence de distance de queue.
    double angle_ref; ///< Référence d'angle.
} context;

/**
 * @brief Initialisation du contexte.
 */
static context g_ctx = {
    .node = {},
    .cmd_vel = synapse_msgs_Twist_init_default,
    .status = synapse_msgs_Status_init_default,
    .actuators = synapse_msgs_Actuators_init_default,
    .actuators_manual = synapse_msgs_Actuators_init_default,
    .pixy_vector=synapse_msgs_PixyVector_init_default,
    .sub_status = {},
    .sub_cmd_vel = {},
    .sub_actuators_manual = {},
    .pub_actuators = {},
    .wheel_radius = CONFIG_CEREBRI_B3RB_WHEEL_RADIUS_MM / 1000.0, // Il va falloir qu'on remesure le rayon de nos roues
    .wheel_base = CONFIG_CEREBRI_B3RB_WHEEL_BASE_MM / 1000.0,
    .no_line_time = 0, // Ajout Antoine
    .prev_speed=0,
    .prev_steer=0,
    .nb_of_vects_stored=0,
    .first_vector_middle=0.0,
    .beta_ref=0.0,
    .sigma_ref=0.0,
    .head_distance_ref=0.0,
    .tail_distance_ref=0.0,
    .angle_ref=0.0,
};

#include "./vector_utility.h" ///< Inclut le fichier d'en-tête pour les utilitaires de vecteur.

/**
 * @brief Construit les coefficients.
 */
void build_coefficients(void);

/**
 * @brief Calcule le gain de direction en fonction de l'angle (en %).
 * @param x Valeur de l'angle pour laquelle calculer le gain.
 * @return Le gain de direction.
 */
double gain_steer(double x);

/**
 * @brief Calcule la vitesse en fonction de l'angle (en %).
 * @param x Valeur de l'angle pour laquelle calculer le gain.
 * @return Le gain de vitesse.
 */
double gain_speed(double x);

/**
 * @brief Affiche les informations.
 * @param steer Direction.
 * @param vecteurs Vecteurs.
 * @param nbVectors Nombre de vecteurs.
 * @param omega_fwd Vitesse avant.
 */
void print_info(double steer,  DVecteur vecteurs, int nbVectors,double omega_fwd);

/**
 * @brief Sature la vitesse.
 * @param speed Vitesse à saturer.
 * @return La vitesse saturée.
 */
double speed_saturation(double speed);

/**
 * @brief Sature la direction.
 * @param steer Direction à saturer.
 * @return La direction saturée.
 */
double steer_saturation(double steer);

/**
 * @brief Obtient l'angle de référence du côté.
 * @param ctx Contexte.
 * @param vecteur Vecteur.
 * @return L'angle de référence du côté.
 */
double get_ref_angle_of_side(context* ctx, Vecteur vecteur);
#endif