//void scene_process_interaction(GameData* d, SceneData* s, const Input* i) {
//	if (!input_is_pressed(i, GAME_INPUT_SELECT)) { return; }
//	Entity* player = entities_get(s, 0);
//	Entity interaction;
//
//	if (entities_get_by_point(s, player->body.x, player->body.y - 5)) {
//		if (entities_text_interaction_get(s, b, d->interactionData.text)) {
//			game_request_open_text_box(d, 2, d->interactionData.text);
//		} else if (entities_item_get(s, b, &d->interactionData.item)) {
//			d->foundItem = 5.0f;
//		}
//	}
//}
//
//void scene_save(const SceneData* s) {
//	// TODO: Non-C++ way
//	//char path[256];
//	//strcat(path, "resources/");
//	//strcat(path, s->name);
//
//	//FILE* f = fopen(path, "w");
//
//	///* Write next entity_id */
//	//fprintf(f, "next_id=%d\n", s->nextEntityId);
//
//	//// Write tiles
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->background[i]);
//	//}
//	//fputs("\n", f);
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->midground[i]);
//	//}
//	//fputs("\n", f);
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->foreground[i]);
//	//}
//	//fputs("\n", f);
//
//	///* Write spawn point */
//	//fprintf(f, "%f,%f\n", s->spawnPoint.x, s->spawnPoint.y);
//
//	//// Write Bodies
//	//fprintf(f, "%lu ", s->bodies.size() - 1);
//	//for (auto&& pair : s->bodies) {
//	//	int id = pair.first;
//	//	Body b = pair.second;
//	//	if (id == 0) { continue; }
//	//	fprintf(f, "%d:%f,%f,%hd,%hd$", id, b.x, b.y, b.w, b.h);
//	//}
//	//fputs("\n", f);
//
//	///* Write Solid entities */
//	//fprintf(f, "%lu ", s->solidEntities.size());
//	//for (int id : s->solidEntities) {
//	//	fprintf(f, "%d,", id);
//	//}
//	//fputs("\n", f);
//
//	///* Write text interactions */
//	//fprintf(f, "%lu ", s->textInteractions.size());
//	//for (auto&& pair : s->textInteractions) {
//	//	int id = pair.first;
//	//	std::string s = pair.second;
//	//	if (id == 0) { continue; }
//	//	fprintf(f, "%d:%s$$", id, s.c_str());
//	//}
//	//fputs("\n", f);
//
//	//fclose(f);
//}
//
//void scene_load(SceneData* s) {
//	FILE* f = fopen("resources/test.level", "r");
//
//	fscanf(f, "next_id=%d\n", &(s->nextEntityId));
//
//
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->background[i]));
//	}
//	// Skip newline
//	fseek(f, 1L, SEEK_CUR);
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->midground[i]));
//	}
//	fseek(f, 1L, SEEK_CUR);
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->foreground[i]));
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	/* Read spawn point*/
//	fscanf(f, "%f,%f", &s->spawnPoint.x, &s->spawnPoint.y);
//	fseek(f, 1L, SEEK_CUR);
//
//	// Read bodies
//	int numberOfBodies = 0;
//	fscanf(f, "%d ", &numberOfBodies);
//	int id;
//	Body b;
//	for (int i = 0; i < numberOfBodies; i++) {
//		fscanf(f, "%d:%f,%f,%hd,%hd$", &id, &b.x, &b.y, &b.w, &b.h);
//		s->bodies[id] = b;
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	/* Read Solid entities */
//	int numberOfSolidEntities = 0;
//	fscanf(f, "%d ", &numberOfSolidEntities);
//	for (int i = 0; i < numberOfSolidEntities; i++) {
//		fscanf(f, "%d,", &id);
//		s->solidEntities.insert(id);
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	/* Read text interactions */
//	int numberOfTextInteractions = 0;
//	char textBuffer[1024];
//	fscanf(f, "%d ", &numberOfTextInteractions);
//	for (int i = 0; i < numberOfTextInteractions; i++) {
//		fscanf(f, "%d:%[^$\n]$$", &id, textBuffer);
//		s->textInteractions[id] = textBuffer;
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	fclose(f);
//}
//
