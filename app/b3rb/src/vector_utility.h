/**
 * @file vector_utility.h
 * @brief Fichier contenant les fonctions relatives à la manipulation des vecteurs
 *
 * @author Antoine Guillermin
 * @date 2024-04-18
 * @version 1.501
 */
#ifndef VECTOR_UTILITY
#define VECTOR_UTILITY

#define VECTEUR_FALSE 0
#define VECTEUR_TRUE 1

double epsilon = 1e-9;  // définir une tolérance

/**
 * @brief Convertit des vecteurs Pixy en vecteurs D.
 *
 * @param pixy_vector Le vecteur Pixy.
 * @param vecteurs Les vecteurs D résultants.
 */
void pixyVecteursToDVecteurs(synapse_msgs_PixyVector pixy_vector,
                             DVecteur* vecteurs);

/**
 * @brief Initialise un vecteur à zéro.
 *
 * @param vecteurs Le vecteur à initialiser.
 */
void setNullVecteur(DVecteur* vecteurs);

/**
 * @brief Convertit des vecteurs D en vecteurs Pixy.
 *
 * @param vecteurs Les vecteurs D.
 * @param pixy_vector Le vecteur Pixy résultant.
 */
void DVecteursToPixyVecteurs(DVecteur vecteurs,
                             synapse_msgs_PixyVector* pixy_vector);

/**
 * @brief Vérifie si un vecteur est nul.
 *
 * @param vecteur Le vecteur à vérifier.
 * @return int Retourne VECTEUR_FALSE si le vecteur est non nul, sinon VECTEUR_TRUE.
 */
int isNullVecteur(Vecteur* vecteur);

/**
 * @brief Obtient le nombre de vecteurs non nuls dans un ensemble de vecteurs D.
 *
 * @param vecteurs Les vecteurs D.
 * @return int Le nombre de vecteurs non nuls.
 */
int getNbOfNonNullVecteurs(DVecteur* vecteurs);

/**
 * @brief Arrange les vecteurs pour que y0 soit toujours inférieur à y1.
 *
 * @param vecteurs Les vecteurs à arranger.
 */
void arrangeVecteurs(DVecteur* vecteurs);

/**
 * @brief Ajoute des vecteurs à une liste dans un contexte donné.
 *
 * @param vecteurs Les vecteurs à ajouter.
 * @param ctx Le contexte dans lequel ajouter les vecteurs.
 * @param nbVecteurs Le nombre de vecteurs à ajouter.
 */
void addVecteursToList(DVecteur vecteurs, context* ctx, int nbVecteurs);

/**
 * @brief Calcule le vecteur moyen à partir d'un ensemble de vecteurs.
 *
 * @param vecteurs Les vecteurs à utiliser.
 * @param size La taille de l'ensemble de vecteurs.
 * @param average_vecteur Le vecteur moyen résultant.
 */
void calculateAverageVecteur(DVecteur* vecteurs, int size,
                             DVecteur* average_vecteur);

/**
 * @brief Calcule la distance entre deux vecteurs D.
 *
 * @param v1 Le premier vecteur.
 * @param v2 Le deuxième vecteur.
 * @return double La distance entre les deux vecteurs.
 */
double calculateVecteursDistance(DVecteur v1, DVecteur v2);

/**
 * @brief Supprime les vecteurs aberrants d'un ensemble de vecteurs.
 *
 * @param vecteurs Les vecteurs à traiter.
 * @param size La taille de l'ensemble de vecteurs.
 * @param avg_vector Le vecteur moyen.
 * @param threshold Le seuil pour déterminer les vecteurs aberrants.
 */
void removeOutliersVecteurs(DVecteur* vecteurs, int* size, DVecteur* avg_vector,
                            double threshold);

/**
 * @brief Calcule le vecteur moyen sans les vecteurs aberrants.
 *
 * @param vecteurs Les vecteurs à utiliser.
 * @param average_vecteurs Le vecteur moyen résultant.
 * @param nbVecteurs Le nombre de vecteurs à utiliser.
 */
void calculateAverageVecteursWithoutOutliers(DVecteur* vecteurs,
                                             DVecteur* average_vecteurs,
                                             int nbVecteurs);

/**
 * @brief Supprime les vecteurs avec une longueur inférieure à une certaine
 * valeur.
 *
 * @param vecteurs Les vecteurs à traiter.
 */
void removeSmallVecteurs(DVecteur* vecteurs);

/**
 * @brief Supprime les vecteurs avec un angle supérieur à une certaine limite.
 *
 * @param vecteurs Les vecteurs à traiter.
 */
void removeLargeAngleVecteur(DVecteur* vecteurs);

/**
 * @brief Fusionne deux vecteurs s'ils sont suffisamment proches.
 *
 * @param vecteurs Les vecteurs à fusionner.
 */
void mergeSameVecteurs(DVecteur* vecteurs);

/**
 * @brief Obtient l'angle d'un vecteur par rapport à l'axe des y.
 *
 * @param vecteur Le vecteur.
 * @return double L'angle du vecteur.
 */
double getVecteurAngle(Vecteur vecteur);

/**
 * @brief Obtient le côté sur lequel se trouve un vecteur par rapport à une
 * position donnée.
 *
 * @param vecteur Le vecteur.
 * @param middle La position de référence.
 * @return int Le côté du vecteur.
 */
int getVecteurSide(Vecteur vecteur, double middle);

/**
 * @brief Calcule l'erreur relative entre une valeur approchée et une valeur
 * exacte.
 *
 * @param valeur_approchee La valeur approchée.
 * @param valeur_exacte La valeur exacte.
 * @return double L'erreur relative.
 */
double erreur_relative(double valeur_approchee, double valeur_exacte);

/**
 * @brief Enregistre un vecteur de référence dans un contexte donné.
 *
 * @param ctx Le contexte dans lequel enregistrer le vecteur de référence.
 * @param vecteurs Le vecteur de référence.
 */
void saveReferenceVecteur(context* ctx, DVecteur vecteurs);

/**
 * @brief Affiche les coordonnées des vecteurs.
 *
 * @param vecteurs Les vecteurs à afficher.
 */
void printVecteur(DVecteur* vecteurs);

/**
 * @brief Obtient la coordonnée x correspondante à une coordonnée y dans
 * l'équation d'un vecteur.
 *
 * @param v Le vecteur.
 * @param y La coordonnée y.
 * @return int La coordonnée x.
 */
int getXWithYInEquationOfVecteur(Vecteur v, double y);

/**
 * @brief Maximise la longueur des vecteurs jusqu'au bord de l'image.
 *
 * @param vecteurs Les vecteurs à traiter.
 */
void maximizeDVector(DVecteur* vecteurs);

/**
 * @brief Calcule la distance entre deux points.
 *
 * @param a Le premier point.
 * @param b Le deuxième point.
 * @return double La distance entre les deux points.
 */
double getPointDistance(Point a, Point b);

/**
 * @brief Construit le deuxième vecteur à partir du premier vecteur et du
 * contexte.
 *
 * @param vecteurs Les vecteurs à utiliser.
 * @param ctx Le contexte.
 */
void buildSecondVecteur(DVecteur* vecteurs, context* ctx);

/**
 * @brief Calcule l'angle moyen entre deux vecteurs.
 *
 * @param vecteurs Les vecteurs à utiliser.
 * @return double L'angle moyen.
 */
double getMiddleAngle(DVecteur vecteurs);

void printVecteur(DVecteur* vecteurs) {
    LOG_INF(
        "m0 : (x0 : %f, y0 : %f), (x1 : %f, y1 : %f)\r\nm1 : (x0 : %f, y0 : "
        "%f), (x1 : %f, y1 : %f)",
        vecteurs->m[0].tail.x, vecteurs->m[0].tail.y, vecteurs->m[0].head.x,
        vecteurs->m[0].head.y, vecteurs->m[1].tail.x, vecteurs->m[1].tail.y,
        vecteurs->m[1].head.x, vecteurs->m[1].head.y);
}

void setNullVecteur(DVecteur* vecteurs) {
    for (int i = 0; i < 2; i++) {
        vecteurs->m[i].tail.x = 0;
        vecteurs->m[i].tail.y = 0;

        vecteurs->m[i].head.x = 0;
        vecteurs->m[i].head.y = 0;
    }
}
void pixyVecteursToDVecteurs(synapse_msgs_PixyVector pixy_vector,
                             DVecteur* vecteurs) {
    vecteurs->m[0].tail.x = pixy_vector.m0_x0;
    vecteurs->m[0].tail.y = pixy_vector.m0_y0;

    vecteurs->m[0].head.x = pixy_vector.m0_x1;
    vecteurs->m[0].head.y = pixy_vector.m0_y1;

    vecteurs->m[1].tail.x = pixy_vector.m1_x0;
    vecteurs->m[1].tail.y = pixy_vector.m1_y0;

    vecteurs->m[1].head.x = pixy_vector.m1_x1;
    vecteurs->m[1].head.y = pixy_vector.m1_y1;
}
void DVecteursToPixyVecteurs(DVecteur vecteurs,
                             synapse_msgs_PixyVector* pixy_vector) {
    pixy_vector->m0_x0 = vecteurs.m[0].tail.x;
    pixy_vector->m0_y0 = vecteurs.m[0].tail.y;

    pixy_vector->m0_x1 = vecteurs.m[0].head.x;
    pixy_vector->m0_y1 = vecteurs.m[0].head.y;

    pixy_vector->m1_x0 = vecteurs.m[1].tail.x;
    pixy_vector->m1_y0 = vecteurs.m[1].tail.y;

    pixy_vector->m1_x1 = vecteurs.m[1].head.x;
    pixy_vector->m1_y1 = vecteurs.m[1].head.y;
}
/* Return VECTEUR_FALSE if vector is non null*/
int isNullVecteur(Vecteur* vecteur) {
    if (!(vecteur->head.x < epsilon && vecteur->head.y < epsilon &&
          vecteur->tail.x < epsilon && vecteur->tail.y < epsilon)) {
        return VECTEUR_FALSE;
    }
    return VECTEUR_TRUE;
}

int getNbOfNonNullVecteurs(DVecteur* vecteurs) {
    int n = 0;
    int i = 0;
    for (i = 0; i < 2; i++) {
        n += (isNullVecteur(&vecteurs->m[i]) == VECTEUR_FALSE) ? 1 : 0;
    }
    return n;
}

void arrangeVecteurs(
    DVecteur* vecteurs) {  // fonction qui Ã©change (x0,y0) avec (x1,y1) si y0 >
                           // y1 pour avoir tjrs y0 < y1
    uint32_t tmp;
    int n = getNbOfNonNullVecteurs(vecteurs);
    int i = 0;
    for (i = 0; i < n; i++) {
        if (vecteurs->m[i].tail.y > vecteurs->m[i].head.y) {
            tmp = vecteurs->m[i].head.y;
            vecteurs->m[i].head.y = vecteurs->m[i].tail.y;
            vecteurs->m[i].tail.y = tmp;
            tmp = vecteurs->m[i].head.x;
            vecteurs->m[i].head.x = vecteurs->m[i].tail.x;
            vecteurs->m[i].tail.x = tmp;
        }
    }
}

void addVecteursToList(DVecteur vecteurs, context* ctx, int nbVecteurs) {
    ctx->nb_of_vects_stored =
        (ctx->nb_of_vects_stored == nbVecteurs) ? 0 : ctx->nb_of_vects_stored;
    ctx->oldVectors[ctx->nb_of_vects_stored] = vecteurs;
    ctx->nb_of_vects_stored++;
}

void calculateAverageVecteur(DVecteur* vecteurs, int size,
                             DVecteur* average_vecteur) {
    for (int i = 0; i < size; i++) {
        DVecteur* vecteur = &vecteurs[i];
        for (int j = 0; j < 2; j++) {
            average_vecteur->m[j].tail.x =
                average_vecteur->m[j].tail.x + vecteur->m[j].tail.x / size;
            average_vecteur->m[j].tail.y =
                average_vecteur->m[j].tail.y + vecteur->m[j].tail.y / size;
            average_vecteur->m[j].head.x =
                average_vecteur->m[j].head.x + vecteur->m[j].head.x / size;
            average_vecteur->m[j].head.y =
                average_vecteur->m[j].head.y + vecteur->m[j].head.y / size;
        }
    }
}

double calculateVecteursDistance(DVecteur v1, DVecteur v2) {
    int i;
    double somme = 0;
    for (i = 0; i < 2; i++) {
        somme += sqrt(pow((double)(v1.m[i].tail.x - v2.m[i].tail.x), 2) +
                      pow((double)(v1.m[i].tail.y - v2.m[i].tail.x), 2) +
                      pow((double)(v1.m[i].head.x - v2.m[i].head.x), 2) +
                      pow((double)(v1.m[i].head.y - v2.m[i].head.y), 2));
    }
    return somme;
}

void removeOutliersVecteurs(DVecteur* vecteurs, int* size, DVecteur* avg_vector,
                            double threshold) {
    calculateAverageVecteur(vecteurs, *size, avg_vector);
    int j = 0;
    for (int i = 0; i < *size; i++) {
        if (calculateVecteursDistance(vecteurs[i], *avg_vector) <= threshold) {
            vecteurs[j++] = vecteurs[i];
        }
    }
    *size = j;
}

void calculateAverageVecteursWithoutOutliers(DVecteur* vecteurs,
                                             DVecteur* average_vecteurs,
                                             int nbVecteurs) {
    int size = nbVecteurs;

    removeOutliersVecteurs(vecteurs, &size, average_vecteurs,
                           MAX_DISTANCE_FOR_SAME_VECT);
    calculateAverageVecteur(vecteurs, size, average_vecteurs);
}
void removeLargeAngleVecteur(DVecteur* vecteurs) {
    int m1_empty = 0;
    if (fabs(getVecteurAngle(vecteurs->m[1])) / M_PI * 180.0 >
        MAX_VECTOR_ANGLE_THRESHOLD) {
        m1_empty++;
    }
    if (fabs(getVecteurAngle(vecteurs->m[0])) / M_PI * 180.0 >
        MAX_VECTOR_ANGLE_THRESHOLD) {
        if (m1_empty == 1) {
            vecteurs->m[0].tail.x = 0;
            vecteurs->m[0].tail.y = 0;
            vecteurs->m[0].head.x = 0;
            vecteurs->m[0].head.y = 0;
        } else {
            vecteurs->m[0].tail.x = vecteurs->m[1].tail.x;
            vecteurs->m[0].tail.y = vecteurs->m[1].tail.y;
            vecteurs->m[0].head.x = vecteurs->m[1].head.x;
            vecteurs->m[0].head.y = vecteurs->m[1].head.y;
        }
    }
    if (m1_empty == 1) {
        vecteurs->m[1].tail.x = 0;
        vecteurs->m[1].tail.y = 0;
        vecteurs->m[1].head.x = 0;
        vecteurs->m[1].head.y = 0;
    }
}
void removeSmallVecteurs(DVecteur* vecteurs) {
    int m1_empty = 0;
    if ((int)(vecteurs->m[1].head.y - vecteurs->m[1].tail.y) <
        MIN_VECTOR_LENGTH) {
        m1_empty = 1;
    }
    if ((int)(vecteurs->m[0].head.y - vecteurs->m[0].tail.y) <
        MIN_VECTOR_LENGTH) {
        if (m1_empty == 1) {
            vecteurs->m[0].head.y = 0;
            vecteurs->m[0].tail.y = 0;
            vecteurs->m[0].head.x = 0;
            vecteurs->m[0].tail.x = 0;
        } else {
            vecteurs->m[0].head.y = vecteurs->m[1].head.y;
            vecteurs->m[0].tail.y = vecteurs->m[1].tail.y;
            vecteurs->m[0].head.x = vecteurs->m[1].head.x;
            vecteurs->m[0].tail.x = vecteurs->m[1].tail.x;
        }
    }
    if (m1_empty == 1) {
        vecteurs->m[1].head.y = 0;
        vecteurs->m[1].tail.y = 0;
        vecteurs->m[1].head.x = 0;
        vecteurs->m[1].tail.x = 0;
    }
}

void mergeSameVecteurs(DVecteur* vecteurs) {
    if (abs((int)(vecteurs->m[1].tail.x - vecteurs->m[0].tail.x)) <
            SAME_VECTOR_THRESHOLD &&
        abs((int)(vecteurs->m[1].head.x - vecteurs->m[0].head.x)) <
            SAME_VECTOR_THRESHOLD) {
        vecteurs->m[0].tail.x =
            (vecteurs->m[1].tail.x + vecteurs->m[0].tail.x) / 2;
        vecteurs->m[0].tail.y =
            (vecteurs->m[1].tail.y + vecteurs->m[0].tail.y) / 2;
        vecteurs->m[0].head.x =
            (vecteurs->m[1].head.x + vecteurs->m[0].head.x) / 2;
        vecteurs->m[0].head.y =
            (vecteurs->m[1].head.y + vecteurs->m[0].head.y) / 2;

        vecteurs->m[1].tail.x = 0;
        vecteurs->m[1].tail.y = 0;
        vecteurs->m[1].head.x = 0;
        vecteurs->m[1].head.y = 0;
    }
}

double getVecteurAngle(Vecteur vecteur) {
    int32_t dx;
    int32_t dy;
    dx = vecteur.head.x - vecteur.tail.x;
    dy = vecteur.head.y - vecteur.tail.y;
    return atan2(dx, dy);
}

int getVecteurSide(Vecteur vecteur, double middle) {
    if (vecteur.head.x < middle) {
        return VECTOR_ON_THE_LEFT;
    }
    return VECTOR_ON_THE_RIGHT;
}

double erreur_relative(double valeur_approchee, double valeur_exacte) {
    return (valeur_exacte - valeur_approchee) / fabs(valeur_exacte);
}

void saveReferenceVecteur(context* ctx, DVecteur vecteurs) {
    LOG_INF("=========");
    printVecteur(&vecteurs);
    LOG_INF("=========");
    Vecteur middle;
    getMiddleVecteur(vecteurs, &middle);
    ctx->first_vector_saved = 1;
    ctx->first_vector = vecteurs;
    ctx->first_vector_angle_left = getVecteurAngle(vecteurs.m[0]);
    ctx->first_vector_angle_right = getVecteurAngle(vecteurs.m[1]);

    ctx->first_vector_middle = (double)middle.head.x;
    ctx->beta_ref = atan2(vecteurs.m[0].head.x - vecteurs.m[0].tail.x,
                          vecteurs.m[0].head.y - vecteurs.m[0].tail.y);
    ctx->sigma_ref = atan2(vecteurs.m[1].head.x - vecteurs.m[1].tail.x,
                           vecteurs.m[1].head.y - vecteurs.m[1].tail.y);s
    ctx->head_distance_ref =
        getPointDistance(vecteurs.m[0].head, vecteurs.m[1].head);
    ctx->tail_distance_ref =
        getPointDistance(vecteurs.m[0].tail, vecteurs.m[1].tail);
    ctx->angle_ref = getMiddleAngle(middle);
}
int getXWithYInEquationOfVecteur(Vecteur v, double y) {
    double a = (double)(v.head.y - v.tail.y) / (double)(v.head.x - v.tail.x);
    double b = v.head.y - a * v.head.x;
    return (y - b) / a;
}
void maximizeDVector(DVecteur* vecteurs) {
    vecteurs->m[0].head.x =
        getXWithYInEquationOfVecteur(vecteurs->m[0], IMAGE_HEIGHT);
    vecteurs->m[1].head.x =
        getXWithYInEquationOfVecteur(vecteurs->m[1], IMAGE_HEIGHT);
    vecteurs->m[0].head.y = IMAGE_HEIGHT;
    vecteurs->m[1].head.y = IMAGE_HEIGHT;

    vecteurs->m[0].tail.x = getXWithYInEquationOfVecteur(vecteurs->m[0], 0.0);
    vecteurs->m[1].tail.x = getXWithYInEquationOfVecteur(vecteurs->m[1], 0.0);
    vecteurs->m[0].tail.y = 0.0;
    vecteurs->m[1].tail.y = 0.0;
}

double getPointDistance(Point a, Point b) {
    return sqrt(powf((a.x - b.x), 2.0) + powf((a.y - b.y), 2.0));
}
void buildSecondVecteur(DVecteur* vecteurs, context* ctx) {
    Vecteur working_vecteur = vecteurs->m[0];

    double l = 1.0;
    double ref_angle = ctx->beta_ref;
    Vecteur ref_vecteur = ctx->first_vector.m[0];

    if (getVecteurSide(working_vecteur, IMAGE_WIDTH / 2) !=
        VECTOR_ON_THE_LEFT) {
        l = -1.0;
        ref_angle = ctx->sigma_ref;
        ref_vecteur = ctx->first_vector.m[1];
    }

    double omega =
        getPointDistance(working_vecteur.head, working_vecteur.tail) /
        getPointDistance(ref_vecteur.head, ref_vecteur.tail);
    double dist_head = omega * ctx->head_distance_ref;
    double beta = atan2(working_vecteur.head.x - working_vecteur.tail.x,
                        working_vecteur.head.y - working_vecteur.tail.y);
    double angle1 = M_PI + l * beta - fabs(ref_angle);
    vecteurs->m[1].head.y = working_vecteur.head.y + cos(angle1) * dist_head;
    vecteurs->m[1].head.x =
        working_vecteur.head.x + l * sin(angle1) * dist_head;

    double dist_tail = omega * ctx->tail_distance_ref;
    double angle2 = M_PI / 2 - angle1;

    vecteurs->m[1].tail.y = working_vecteur.tail.y + dist_tail * sin(angle2);
    vecteurs->m[1].tail.x =
        working_vecteur.tail.x + l * dist_tail * cos(angle2);
}
double getMiddleAngle(Vecteur middle) {
    return atan2(middle.head.x - middle.tail.x, middle.head.y - middle.tail.y);
}
void getMiddleVecteur(DVecteur vecteurs, Vecteur * middle)
{
    double middle_head =
        getPointDistance(vecteurs.m[0].head, vecteurs.m[1].head) / 2.0;
    double middle_tail =
        getPointDistance(vecteurs.m[0].tail, vecteurs.m[1].tail) / 2.0;
    double beta = atan2(vecteurs.m[1].head.y - vecteurs.m[0].head.y,
                        vecteurs.m[1].head.x - vecteurs.m[0].head.x);
    middle->head.x = vecteurs.m[0].head.x + cos(fabs(beta)) * middle_head;
    middle->head.y = vecteurs.m[0].head.y + sin(beta) * middle_head;
    double beta2 = atan2(vecteurs.m[1].tail.y - vecteurs.m[0].tail.y,
                         vecteurs.m[1].tail.x - vecteurs.m[0].tail.x);
    middle->tail.x = vecteurs.m[0].tail.x + cos(fabs(beta2)) * middle_tail;
    middle->tail.y = vecteurs.m[0].tail.y + sin(beta2) * middle_tail;
}
#endif