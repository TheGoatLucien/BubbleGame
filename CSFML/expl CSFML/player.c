#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void update_attachment_status(bubble_t* grid[ROWS][COLS], int attach[ROWS][COLS]) {
    // Initialiser attach à -1
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            attach[i][j] = -1;
        }
    }

    // Pour chaque colonne de la première ligne, si une bulle existe, on la marque comme attachée (0)
    for (int j = 0; j < COLS; j++) {
        if (grid[0][j] != NULL) {
            attach[0][j] = 0;
        }
    }

    // Maintenant, en utilisant un flood fill 'iteratif' ou récursif, propage la marque 0 aux voisins.
    // On définit les offsets pour les voisins (selon la parité des lignes)
    int offsetsEven[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,-1}, {1,0}, {1,-1} };
    int offsetsOdd[6][2] = { {0,-1}, {0,1}, {-1,0}, {-1,1}, {1,0}, {1,1} };

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                // Si cette cellule est marquée (== 0) et a une bulle
                if (attach[i][j] == 0 && grid[i][j] != NULL) {
                    // Choisir offsets en fonction de la parité
                    int (*offsets)[2] = (i % 2 == 0) ? offsetsEven : offsetsOdd;

                    // Pour chaque voisin
                    for (int k = 0; k < 6; k++) {
                        int newRow = i + offsets[k][0];
                        int newCol = j + offsets[k][1];
                        if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS) {
                            // Si le voisin contient une bulle et n'est pas encore marqué
                            if (grid[newRow][newCol] != NULL && attach[newRow][newCol] != 0) {
                                attach[newRow][newCol] = 0;
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void apply_fall_logic_mathematically(player_t* player) {
    int attach[ROWS][COLS];
    update_attachment_status(player->grid, attach);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && attach[i][j] == -1) {
                // Marquer la bulle comme en chute
                player->grid[i][j]->falling = 1;
                player->grid[i][j]->next = player->falling_bubbles;
                player->falling_bubbles = player->grid[i][j];

                // Retirer la bulle de la grille
                player->grid[i][j] = NULL;
            }
        }
    }
}


void descend_bubbles(player_t* player) {
    float gridOriginY = 100.0f; // Position de départ de la grille

    for (int row = ROWS - 1; row > 0; row--) {
        for (int col = 0; col < COLS; col++) {
            if (player->grid[row - 1][col]) {
                player->grid[row][col] = player->grid[row - 1][col];
                player->grid[row - 1][col] = NULL;
                player->grid[row][col]->pos.y = gridOriginY + row * V_SPACING + BUBBLE_RADIUS;
            }
        }
    }
}


void add_random_bubble_line(player_t* player) {
    // Descendre les bulles existantes
    for (int row = ROWS - 1; row > 0; row--) {
        for (int col = 0; col < COLS; col++) {
            if (player->grid[row - 1][col]) {
                player->grid[row][col] = player->grid[row - 1][col];
                player->grid[row - 1][col] = NULL;
                player->grid[row][col]->pos.y += V_SPACING;
            }
        }
    }

    // Ajouter une nouvelle ligne en haut
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;
    float gridOriginY = 100.0f;

    for (int col = 0; col < COLS; col++) {
        bubble_t* new_bubble = malloc(sizeof(bubble_t));
        if (!new_bubble) continue;

        new_bubble->color = rand() % 4 + 1; // Couleur aléatoire
        new_bubble->active = 0;            // Assurez-vous que la bulle n'est pas active
        new_bubble->falling = 0;           // Assurez-vous que la bulle ne tombe pas
        new_bubble->next = NULL;

        new_bubble->pos.x = gridOriginX + col * H_SPACING + BUBBLE_RADIUS;
        new_bubble->pos.y = gridOriginY;

        player->grid[0][col] = new_bubble;
    }
}
void reset_bubble_states(player_t* player) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j]) {
                player->grid[i][j]->active = 0;
                player->grid[i][j]->falling = 0;
            }
        }
    }
}



player_t create_player(sfVector2f pos) {
    player_t player = { 0 };
    player.launcher_pos = pos;
    player.angle = -3.14f / 2;
    player.score = 0;
    player.defeat = 0;
    player.current = NULL;
    player.next_bubble = create_bubble(pos, 0); // Create the next bubble
    player.bonus_animation = NULL; // Initialisation du champ

    float gridOriginX = player.launcher_pos.x - (COLS * H_SPACING) / 2;
    float gridOriginY = 100.0f; // Start position for the top

    // Initialize rows
    for (int i = 0; i < INIT_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bubble_t* b = malloc(sizeof(bubble_t));
            if (!b) continue;
            b->color = rand() % 4 + 1; // Random playable color
            float offsetX = (i % 2 == 0) ? 0 : H_SPACING / 2; // Offset for odd rows
            b->pos.x = gridOriginX + offsetX + j * H_SPACING + BUBBLE_RADIUS;
            b->pos.y = gridOriginY + i * V_SPACING; // Align with grid origin
            b->falling = 0;
            b->active = 0;
            b->next = NULL;
            player.grid[i][j] = b;
        }
    }
    return player;
}



void update_player(player_t* player, sfEvent event, int left, int right, int shoot) {
    float delta = getDeltaTime();
    float rotationSpeed = 1.0f;

    if (sfKeyboard_isKeyPressed(left)) player->angle -= rotationSpeed * delta;
    if (sfKeyboard_isKeyPressed(right)) player->angle += rotationSpeed * delta;

    if (player->angle < -2.7f) player->angle = -2.7f;
    if (player->angle > -0.8f) player->angle = -0.8f;

    if (sfKeyboard_isKeyPressed(shoot) && player->current == NULL) {
        player->current = player->next_bubble;
        player->current->active = 1;
        player->next_bubble = create_bubble(player->launcher_pos, 0); // Générer une nouvelle bulle
    }

    if (player->current != NULL) {
        player->current->pos.x += cos(player->angle) * 800 * delta;
        player->current->pos.y += sin(player->angle) * 800 * delta;

        // Rebond mur gauche/droit
        if (player->current->pos.x <= 0 || player->current->pos.x >= WINDOWS_WIDHT) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
        }

        // Rebond mur central
        float centerX = WINDOWS_WIDHT / 2;
        float margin = 10;

        if ((player->launcher_pos.x < centerX && player->current->pos.x >= centerX - margin) ||
            (player->launcher_pos.x > centerX && player->current->pos.x <= centerX + margin)) {
            float dx = cos(player->angle);
            float dy = sin(player->angle);
            player->angle = atan2(dy, -dx);
            if (player->launcher_pos.x < centerX)
                player->current->pos.x = centerX - margin;
            else
                player->current->pos.x = centerX + margin;
        }

        // Plafond
        if (player->current->pos.y < 0) {
            destroy_bubble(player->current);
            player->current = NULL;
            player->next_bubble = create_bubble(player->launcher_pos, 0); // Nouvelle bulle après destruction
        }
    }
}

void draw_aim_line(sfVector2f origin, float angle, sfRenderWindow* window) {
    sfVector2f pos = origin;
    float dx = cos(angle), dy = sin(angle);
    int bounces = 0;
    const int max_bounces = 3;

    sfVertexArray* line = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(line, sfLinesStrip);
    sfVertexArray_append(line, (sfVertex) { pos, sfColor_fromRGBA(255, 255, 255, 80) });

    float centerX = WINDOWS_WIDHT / 2; // Position centrale du mur
    float margin = 5; // Marge pour détecter la collision avec le mur central

    while (bounces < max_bounces) {
        for (int i = 0; i < 100; i++) { // Avancer par petits pas
            pos.x += dx * 5;
            pos.y += dy * 5;

            // Vérifie rebond mur gauche/droit
            if (pos.x <= 0 || pos.x >= WINDOWS_WIDHT) {
                dx = -dx;
                bounces++;
                break;
            }

            // Vérifie rebond avec le plafond
            if (pos.y <= 0) {
                bounces = max_bounces;
                break;
            }

            // Vérifie rebond avec le mur central
            if ((origin.x < centerX && pos.x >= centerX - margin) || 
                (origin.x > centerX && pos.x <= centerX + margin)) {
                dx = -dx;
                bounces++;

                // Ajuste la position pour éviter de traverser le mur
                if (origin.x < centerX)
                    pos.x = centerX - margin;
                else
                    pos.x = centerX + margin;
                
                break;
            }
        }

        sfVertex v = { pos, sfColor_fromRGBA(255, 255, 255, 80) };
        sfVertexArray_append(line, v);
    }

    sfRenderWindow_drawVertexArray(window, line, NULL);
    sfVertexArray_destroy(line);
}


void update_falling_bubbles(player_t* player) {
    bubble_t** current = &player->falling_bubbles;
    float fallSpeed = 400.0f;

    while (*current) {
        (*current)->pos.y += fallSpeed * getDeltaTime();
        if ((*current)->pos.y > WINDOWS_HEIGHT + BUBBLE_RADIUS) {
            bubble_t* toDestroy = *current;
            *current = (*current)->next;
            destroy_bubble(toDestroy);
        }
        else {
            // Réinitialiser l'état de la bulle après la chute
            (*current)->falling = 0;
            current = &(*current)->next;
        }
    }
}


void draw_player(player_t* player, sfRenderWindow* window) {
    sfRectangleShape* midWall = sfRectangleShape_create();
    sfRectangleShape_setSize(midWall, (sfVector2f) { 10, WINDOWS_HEIGHT });
    sfRectangleShape_setFillColor(midWall, sfColor_fromRGBA(255, 255, 255, 60));
    sfRectangleShape_setPosition(midWall, (sfVector2f) { (WINDOWS_WIDHT / 2) - 5, 0 });
    sfRenderWindow_drawRectangleShape(window, midWall, NULL);
    sfRectangleShape_destroy(midWall);

    draw_aim_line(player->launcher_pos, player->angle, window);

    sfCircleShape* launcher = sfCircleShape_create();
    sfCircleShape_setRadius(launcher, 20);
    sfCircleShape_setOrigin(launcher, (sfVector2f) { 20, 20 });
    sfCircleShape_setFillColor(launcher, sfWhite);
    sfCircleShape_setPosition(launcher, player->launcher_pos);
    sfRenderWindow_drawCircleShape(window, launcher, NULL);
    sfCircleShape_destroy(launcher);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j]) {
                draw_bubble(player->grid[i][j], window);
            }
        }
    }

    if (player->current) {
        draw_bubble(player->current, window);
    }
    else {  
        sfCircleShape* preview = sfCircleShape_create();
        sfCircleShape_setRadius(preview, 16);
        sfCircleShape_setOrigin(preview, (sfVector2f) { 16, 16 });
        sfCircleShape_setPosition(preview, player->launcher_pos);

        // Utiliser la couleur de la prochaine bulle
        sfColor color;
        switch (player->next_bubble->color) {
        case 0: color = sfColor_fromRGB(128, 128, 128); break;
        case 1: color = sfRed; break;
        case 2: color = sfBlue; break;
        case 3: color = sfGreen; break;
        case 4: color = sfYellow; break;
        default: color = sfWhite; break;
        }
        sfCircleShape_setFillColor(preview, color);
        sfRenderWindow_drawCircleShape(window, preview, NULL);
        sfCircleShape_destroy(preview);
    }
    // Bulles en chute libre
    bubble_t* fall = player->falling_bubbles;
    while (fall) {
        draw_bubble(fall, window);
        fall = fall->next;
    }

}

int flood_fill(bubble_t* grid[ROWS][COLS], int row, int col, int color, int visited[ROWS][COLS], bubble_t** cluster, int* count) {
    // Vérification des limites et conditions de correspondance
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    if (visited[row][col] || !grid[row][col] || grid[row][col]->color != color)
        return 0;

    visited[row][col] = 1;
    cluster[(*count)++] = grid[row][col];

    // Dépendant de la parité de la ligne, les voisins diffèrent :
    // Pour les lignes paires (index 0, 2, 4, …)
    int offsetsEven[6][2] = {
        {0, -1},   // Gauche
        {0, 1},    // Droite
        {-1, 0},   // Haut
        {-1, -1},  // Haut gauche
        {1, 0},    // Bas
        {1, -1}    // Bas gauche
    };
    // Pour les lignes impaires (index 1, 3, 5, …)
    int offsetsOdd[6][2] = {
        {0, -1},   // Gauche
        {0, 1},    // Droite
        {-1, 0},   // Haut
        {-1, 1},   // Haut droite
        {1, 0},    // Bas
        {1, 1}     // Bas droite
    };

    int (*offsets)[2] = (row % 2 == 0) ? offsetsEven : offsetsOdd;

    for (int i = 0; i < 6; i++) {
        int newRow = row + offsets[i][0];
        int newCol = col + offsets[i][1];
        flood_fill(grid, newRow, newCol, color, visited, cluster, count);
    }

    return *count;
}

void update_bonus_animation(player_t* player, sfRenderWindow* window) {
    if (player->bonus_animation) {
        bonus_animation_t* anim = player->bonus_animation;

        // Réduire le temps restant
        anim->timer -= getDeltaTime();
        if (anim->timer > 0) {
            // Calculer l'alpha en fonction du temps restant
            anim->alpha = (int)(255 * (anim->timer / 2.0f)); // 2.0f = durée totale
            if (anim->alpha < 0) anim->alpha = 0;

            // Mettre à jour la couleur du texte avec le nouvel alpha
            sfColor color = sfText_getFillColor(anim->text);
            color.a = anim->alpha;
            sfText_setFillColor(anim->text, color);

            // Dessiner le texte
            sfRenderWindow_drawText(window, anim->text, NULL);
        }
        else {
            // Supprimer l'animation lorsque le temps est écoulé
            sfText_destroy(anim->text);
            free(anim);
            player->bonus_animation = NULL;
        }
    }
}
void start_bubble_animation(player_t* player, bubble_t* bubble) {
    bubble_animation_t* anim = malloc(sizeof(bubble_animation_t));
    if (!anim) return;

    anim->bubble = bubble;
    anim->scale = 1.0f; // Taille initiale
    anim->alpha = 255;  // Transparence initiale
    anim->timer = 0.5f; // Durée de l'animation (0.5 seconde)
    anim->next = player->animations;
    player->animations = anim;
}

void update_bubble_animations(player_t* player, float deltaTime) {
    bubble_animation_t** current = &player->animations;

    while (*current) {
        bubble_animation_t* anim = *current;

        // Réduire le temps restant
        anim->timer -= deltaTime;
        if (anim->timer <= 0) {
            // Supprimer l'animation une fois terminée
            *current = anim->next;
            free(anim);
            continue;
        }

        // Mettre à jour l'échelle et la transparence
        anim->scale += deltaTime * 2.0f; // Agrandissement rapide
        anim->alpha -= (int)(deltaTime * 500.0f); // Réduction de la transparence
        if (anim->alpha < 0) anim->alpha = 0;

        // Appliquer la transparence et l'échelle à la bulle associée
        if (anim->bubble) {
            anim->bubble->active = 1; // Marquer la bulle comme active
        }

        current = &(*current)->next;
    }
}


void draw_bubble_animations(player_t* player, sfRenderWindow* window) {
    bubble_animation_t* anim = player->animations;

    while (anim) {
        sfCircleShape* shape = sfCircleShape_create();
        sfCircleShape_setRadius(shape, BUBBLE_RADIUS * anim->scale);
        sfCircleShape_setOrigin(shape, (sfVector2f) { BUBBLE_RADIUS* anim->scale, BUBBLE_RADIUS* anim->scale });
        sfCircleShape_setPosition(shape, anim->bubble->pos);

        sfColor color = sfRed; // Couleur rouge pour les bulles rouges
        color.a = anim->alpha; // Appliquer la transparence
        sfCircleShape_setFillColor(shape, color);

        sfRenderWindow_drawCircleShape(window, shape, NULL);
        sfCircleShape_destroy(shape);

        anim = anim->next;
    }
}



void update_bubbles(player_t* player, float* chrono) {
    if (!player->current) return;

    // Collision avec plafond
    if (player->current->pos.y <= 0)
        goto attach;

    // Collision avec une autre bulle
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] &&
                Circle_Collision(player->current->pos, player->grid[i][j]->pos, BUBBLE_RADIUS, BUBBLE_RADIUS)) {
                goto attach;
            }
        }
    }

    return;

attach: {
    float gridOriginX = player->launcher_pos.x - (COLS * H_SPACING) / 2;
    int row = (int)round((player->current->pos.y - 100.0f) / V_SPACING);
    int col = (int)round((player->current->pos.x - gridOriginX - ((row % 2 == 0) ? 0 : H_SPACING / 2)) / H_SPACING);

    row = fClamp(row, 0, ROWS - 1);
    col = fClamp(col, 0, COLS - 1);

    int placed = 0;

    if (!player->grid[row][col]) {
        placed = 1;
    }
    else {
        int dx[6] = { -1, -1, 0, 0, 1, 1 };
        int dy_even[6] = { 0, 1, -1, 1, 0, 1 };
        int dy_odd[6] = { -1, 0, -1, 1, -1, 0 };
        int* dy = (row % 2 == 0) ? dy_even : dy_odd;

        for (int i = 0; i < 6; i++) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS &&
                !player->grid[newRow][newCol]) {

                float offsetX = (newRow % 2 == 0) ? 0 : H_SPACING / 2;
                player->current->pos.x = gridOriginX + offsetX + newCol * H_SPACING + BUBBLE_RADIUS;
                player->current->pos.y = 100.0f + newRow * V_SPACING + BUBBLE_RADIUS;

                row = newRow;
                col = newCol;
                placed = 1;
                break;
            }
        }
    }

    if (!placed) {
        destroy_bubble(player->current);
        player->current = NULL;
        return;
    }

    // Place dans la grille
    player->grid[row][col] = player->current;
    player->current = NULL;

    // Détection de grappe
    int visited[ROWS][COLS] = { 0 };
    bubble_t* cluster[ROWS * COLS] = { 0 };
    int count = 0;
    flood_fill(player->grid, row, col, player->grid[row][col]->color, visited, cluster, &count);

    if (count >= 3) {
        for (int i = 0; i < count; i++) {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    if (player->grid[r][c] == cluster[i]) {
                        cluster[i]->active = 0; // Marquer la bulle comme inactive
                        cluster[i]->falling = 1; // Marquer la bulle comme en chute
                        start_bubble_animation(player, player->grid[r][c]); //  animation
                        player->grid[r][c]->falling = 1;
                       
                        player->grid[r][c] = NULL;
                    }
                }
            }
        }

        player->score += count;        // score
        *chrono += 10.0f;              //  gain chrono
        if (*chrono > 60.0f) *chrono = 60.0f;

        apply_fall_logic_mathematically(player);  // suspension
    }
   
    }
// Vérification de la défaite (bulles trop proches du bas)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (player->grid[i][j] && player->grid[i][j]->pos.y >= WINDOWS_HEIGHT - 100) {
                player->defeat = 1;
                return;
            }
        }
    }
}
