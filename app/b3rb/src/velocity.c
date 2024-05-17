/**
 * @file velocity.c
 * @brief Fichier pour la gestion de la conduite automatique.
 *
 * @author Antoine Guillermin
 * @date 2024-04-18
 * @version 1.501
 */
#include "velocity.h" 

// Fonction pour calculer le gain de direction en fonction de l'angle
double gain_steer(double x) {
    if (x > 100.0) { // Si x est supérieur à 100
        x = 100.0; // On fixe x à 100
    }
   int q = (int)((x-fmod(x,10.0)) / 10); // On calcule la partie entière de x/10
    return (steers[q + 1] - steers[q]) / 10.0 * (x - (double)q * 10.0) + steers[q]; // On retourne le gain de direction calculé
}

// Fonction pour calculer le gain de vitesse en fonction de l'angle
double gain_speed(double x) {
    if (x > 100.0) { // Si x est supérieur à 100
        x = 100.0; // On fixe x à 100
    }
    int q = (int)((x-fmod(x,10.0)) / 10); // On calcule la partie entière de x/10
    return (speeds[q + 1] - seeds[q]) / 10.0 * (x - (double)q * 10.0) + speeds[q]; // On retourne le gain de vitesse calculé
}

// Fonction pour afficher les informations de direction, vecteurs, nombre de vecteurs et omega_fwd
void print_info(DVecteur vecteurs, int nbVectors,double angle_observed,
				double steer,
				double speed,
				double steer_gain,
				double speed_gain)
{
    LOG_INF("                                                     \
            {                                                     \
            Timestamp: %lld,                                      \
            Nb_vectors:%d,                                        \
            m0:{                                                  \
				x0: %f,                                           \
				y0: %f,  										  \
				x1: %f,   										  \
				y1: %f                                            \
			},                                                    \
            m1:{                                                  \
				x0: %f,                                           \
				y0: %f,                                           \
				x1: %f,                                           \
				y1: %f                                            \
			},                                                    \
			angle_observed: %f,                                   \
			steer: %f,                                            \
			speed: %f,                                            \
			steer_gain: %f                                        \
            }",
                k_uptime_get(), // Récupère le temps écoulé depuis le démarrage du système
                nbVectors, // Affiche le nombre de vecteurs
                vecteurs.m[0].tail.x, // Affiche x0 du vecteur m0
                vecteurs.m[0].tail.y, // Affiche y0 du vecteur m0
                vecteurs.m[0].head.x, // Affiche x1 du vecteur m0
                vecteurs.m[0].head.y, // Affiche y1 du vecteur m0
                vecteurs.m[1].tail.x, // Affiche x0 du vecteur m1
                vecteurs.m[1].tail.y, // Affiche y0 du vecteur m1
                vecteurs.m[1].head.x, // Affiche x1 du vecteur m1
                vecteurs.m[1].head.y,
				angle_observed,
				steer,
				speed,
				steer_gain
				); // Affiche y1 du vecteur m1
}

// Fonction pour saturer la direction
double steer_saturation(double steer)
{
    if(steer>100){ // Si steer est supérieur à 100
        steer=100; // On fixe steer à 100
    }else if(steer < -100){ // Si steer est inférieur à -100
        steer=-100; // On fixe steer à -100
    }
    return steer; // On retourne steer
}

// Fonction pour saturer la vitesse
double speed_saturation(double speed)
{
    if(speed>100){ // Si speed est supérieur à 100
        speed=100; // On fixe speed à 100
    }else if(speed <=0){ // Si speed est inférieur ou égal à 0
        speed= 0; // On fixe speed à 0
    }
    return speed; // On retourne speed
}



static void follow_line(context* ctx)
{
    double steer = 0.0; // Initialise steer à 0
    double speed = 0.0; // Initialise speed à 0
    DVecteur vecteurs; // Déclare une variable vecteurs de type DVecteur
    pixyVecteursToDVecteurs(ctx->pixy_vector,&vecteurs); // Convertit les vecteurs Pixy en DVecteurs
    double angle_percent;
	double steer_gain;
	
    int num_vectors = getNbOfNonNullVecteurs(&vecteurs); // Obtient le nombre de vecteurs non nuls
    arrangeVecteurs(&vecteurs); // Arrange les vecteurs
    addVecteursToList(vecteurs,ctx,NB_OF_VECTS_TO_WAIT); // Ajoute les vecteurs à la liste
    if(ctx->nb_of_vects_stored == NB_OF_VECTS_TO_WAIT){ // Si le nombre de vecteurs stockés est égal à NB_OF_VECTS_TO_WAIT
        DVecteur average_vecteurs; // Déclare une variable average_vecteurs de type DVecteur
        setNullVecteur(&average_vecteurs); // Met à zéro average_vecteurs
        calculateAverageVecteursWithoutOutliers(ctx->oldVectors,&average_vecteurs,NB_OF_VECTS_TO_WAIT); // Calcule la moyenne des vecteurs sans les valeurs aberrantes
        removeSmallVecteurs(&average_vecteurs); // Supprime les petits vecteurs
        removeLargeAngleVecteur(&average_vecteurs); // Supprime les vecteurs avec un grand angle
       mergeSameVecteurs(&average_vecteurs); // Fusionne les vecteurs similaires dans average_vecteurs
        num_vectors = getNbOfNonNullVecteurs(&average_vecteurs); // Obtient le nombre de vecteurs non nuls dans average_vecteurs

        // Si aucun vecteur n'est détecté, on arrête la voiture
        if(num_vectors  == 0) {
            // Si c'est la première fois qu'aucun vecteur n'est détecté
            if(ctx->no_line_time == 0){
                ctx->no_line_time=k_uptime_get(); // Enregistre le temps actuel
                steer = ctx->prev_steer; // Utilise la dernière valeur de direction
                speed=NO_VECTOR_SPEED; // Définit la vitesse à NO_VECTOR_SPEED
                LOG_INF("\r\nFirst vector\r\n"); // Affiche un message d'information
            }
            // Si cela fait plus de NO_LINE_TIME ms qu'aucun vecteur n'a été détecté
            else if(k_uptime_get() - ctx->no_line_time >= NO_LINE_TIME) {
                speed = 0.0; // Arrête la voiture
                steer = 0.0; // Arrête de tourner
                LOG_INF("\r\n No vectors, stopping"); // Affiche un message d'information
            }
            else{
                LOG_INF("\r\n %lld ms without any vectors", k_uptime_get() - ctx->no_line_time); // Affiche le temps écoulé depuis la dernière détection de vecteur
                speed = NO_VECTOR_SPEED; // Continue à la même vitesse
                steer =ctx->prev_steer; // Continue à tourner dans la même direction
            }

            // Si la direction est supérieure à INCREASE_STEER_IF_NO_VECTORS_THRESHOLD
            if(steer > INCREASE_STEER_IF_NO_VECTORS_THRESHOLD){
                steer+=INCREASE_STEER_IF_NO_VECTORS; // Augmente la direction
            }
            // Si la direction est inférieure à -INCREASE_STEER_IF_NO_VECTORS_THRESHOLD
            else if(steer < -INCREASE_STEER_IF_NO_VECTORS_THRESHOLD){
                steer-=INCREASE_STEER_IF_NO_VECTORS; // Diminue la direction
            }
        }
        // Si deux vecteurs sont détectés et que le premier vecteur n'a pas encore été enregistré
        else if(num_vectors == 2 && ctx->first_vector_saved == 0){
            maximizeDVector(&average_vecteurs); // Maximise average_vecteurs
            saveReferenceVecteur(ctx,average_vecteurs); // Enregistre le vecteur de référence
        }
        else{
            // Si un seul vecteur est détecté et que le premier vecteur a été enregistré
            if(num_vectors == 1 && ctx->first_vector_saved == 1){
                buildSecondVecteur(&average_vecteurs,ctx); // Construit le second vecteur
                num_vectors++; // Incrémente le nombre de vecteurs
            }
            // Si deux vecteurs sont détectés
            if(num_vectors == 2){
                ctx->no_line_time = 0; // Réinitialise le temps sans détection de ligne
                Vecteur middle;
                getMiddleVecteur(average_vecteurs, &middle);
                double angle_antoine = (getMiddleAngle(middle) -ctx->angle_ref)*100/M_PI; // Calcule l'angle du vecteur central moins l'erreur statique du début et met cet angle en %
                 angle_percent = angle_antoine*100.0/15.0; // Applique un premier gain pour augmenter les petits angles

                 steer_gain = gain_steer(fabs(angle_percent) > 100.0):1.0; // Calcule le gain de direction
                steer =  steer_gain*angle_percent; // Calcule la nouvelle direction
				 speed = gain_speed(fabs(angle_percent)); // Calcule la vitesse
                LOG_INF("angle: %f, angle(percent): %f, gain: %f, steer : %f",angle_antoine,angle_percent,steer_gain,steer); // Affiche les informations de direction
            }
            steer= steer_saturation(steer); // Sature la direction
            ctx->prev_steer = steer; // Enregistre la direction actuelle
           
            speed = speed_saturation(speed); // Sature la vitesse
			if(num_vectors == 2){
			print_info(average_vecteurs, num_vectors,angle_percent,steer,speed, steer_gain);
			}
            steer = steer*STEER_FACTOR; // Applique le facteur de direction
            ctx->prev_speed = speed; // Enregistre la vitesse actuelle
            double omega_fwd=speed*SPEED_FACTOR; // Calcule la vitesse avant
            b3rb_set_actuators(&ctx->actuators, steer, omega_fwd); // Met à jour les actionneurs
        }
    }
}

// Plus utilisée - Fonction pour obtenir l'angle de référence d'un côté
double get_ref_angle_of_side(context* ctx, Vecteur vecteur)
{   
    return (getVecteurSide(vecteur, ctx->first_vector_middle) == VECTOR_ON_THE_LEFT) ? ctx->first_vector_angle_left : ctx->first_vector_angle_right;
}


static void stop(context* ctx)
{
    b3rb_set_actuators(&ctx->actuators, 0, 0);
}

static void b3rb_velocity_entry_point(void* p0, void* p1, void* p2)
{
    LOG_INF("init");
    context* ctx = p0;
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);

    init_b3rb_vel(ctx);

    while (true) {
        synapse_msgs_Status_Mode mode = ctx->status.mode;

        int rc = 0;
        if (mode == synapse_msgs_Status_Mode_MODE_MANUAL) {
            struct k_poll_event events[] = {
                *zros_sub_get_event(&ctx->sub_actuators_manual),
            };
            rc = k_poll(events, ARRAY_SIZE(events), K_MSEC(1000));
            if (rc != 0) {
                LOG_DBG("not receiving manual actuators");
            }
        } else if(mode == synapse_msgs_Status_Mode_MODE_CMD_VEL){
            struct k_poll_event events[] = {
                *zros_sub_get_event(&ctx->sub_cmd_vel),
            };
            rc = k_poll(events, ARRAY_SIZE(events), K_MSEC(1000));
            if (rc != 0) {
                LOG_DBG("not receiving cmd_vel");
            }
        } else {
            struct k_poll_event events[] = {
                *zros_sub_get_event(&ctx->sub_pixy_vector),
            };
            rc = k_poll(events, ARRAY_SIZE(events), K_MSEC(1000));
            if (rc != 0) {
                LOG_DBG("not receiving pixy_vector");
            }
        }

        if (zros_sub_update_available(&ctx->sub_status)) {
            zros_sub_update(&ctx->sub_status);
        }

        if (zros_sub_update_available(&ctx->sub_cmd_vel)) {
            zros_sub_update(&ctx->sub_cmd_vel);
        }

        if (zros_sub_update_available(&ctx->sub_actuators_manual)) {
            zros_sub_update(&ctx->sub_actuators_manual);
        }
        if (zros_sub_update_available(&ctx->sub_pixy_vector)) {
            zros_sub_update(&ctx->sub_pixy_vector);
        }
        #ifdef AUTO_START 
         #ifdef CONFIG_CEREBRI_SENSE_SAFETY
            if(ctx->status.safety == synapse_msgs_Status_Safety_SAFETY_UNSAFE && ctx->status.mode != synapse_msgs_Status_Mode_MODE_MANUAL){
            ctx->status.mode = synapse_msgs_Status_Mode_MODE_AUTO;
            ctx->status.arming = synapse_msgs_Status_Arming_ARMING_ARMED;
            }
            #endif
        #endif
        // handle modes
        if (rc < 0) {
            stop(ctx);
            LOG_DBG("no data, stopped");
        } else if (ctx->status.arming != synapse_msgs_Status_Arming_ARMING_ARMED) {
            stop(ctx);
            LOG_DBG("not armed, stopped");
        } else if (ctx->status.mode == synapse_msgs_Status_Mode_MODE_MANUAL) {
            LOG_DBG("manual mode");
            ctx->actuators = ctx->actuators_manual;
        } else if(ctx->status.mode == synapse_msgs_Status_Mode_MODE_AUTO){
            LOG_DBG("auto mode");
            follow_line(ctx);
        }else{
            LOG_DBG("cmd_vel mode");
            update_cmd_vel(ctx);
        }

        // publish
        zros_pub_update(&ctx->pub_actuators);
    }
}

K_THREAD_DEFINE(b3rb_velocity, MY_STACK_SIZE,
    b3rb_velocity_entry_point, &g_ctx, NULL, NULL,
    MY_PRIORITY, 0, 1000);

/* vi: ts=4 sw=4 et */