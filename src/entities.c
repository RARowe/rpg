#include "types.h"

void entities_wall_add(SceneData* s, float x, float y, unsigned short w, unsigned short h) {
    Body b = { x, y, w, h };

    int entityId = s->bodies.size();
    s->solidEntities.insert(entityId);
    s->bodies[entityId] = b;
}

// TODO: This could probably be easily generalized
void entities_wall_remove(SceneData* s, void* entity) {
    for (unsigned int i = 0; i < s->bodies.size(); i++) {
        Body* b = &(s->bodies[i]);

        if (b == (Body*)entity) {
            s->bodies.erase(i);
            s->solidEntities.erase(i);
            break;
        }
    }
}

void entities_text_interaction_add(SceneData* s, void* entity, const char* text) {
    for (unsigned int i = 0; i < s->bodies.size(); i++) {
        Body* b = &(s->bodies[i]);

        if (b == (Body*)entity) {
            s->textInteractions[i] = text;
            break;
        }
    }
}
