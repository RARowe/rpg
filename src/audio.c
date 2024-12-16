//static void
//audio_queue_sound(Audio* a, int soundId) {
//    // TODO: This does nothing interesting
//    a->queue.size += 1;
//}
//
//static void
//audio_request_stop_music(Audio* a) {
//    a->stopRequested = true;
//}
//
///* Internal Definitions */
//static int
//beginning_audio(Audio* a) {
//
//    // TODO: These should go somewhere else
//    a->music = Mix_LoadMUS("resources/welcome.wav");
//    a->sounds = Mix_LoadWAV("resources/start.ogg");
//    a->stopRequested = false;
//    a->queue.size = 0;
//
//    Mix_PlayMusic(a->music, -1);
//
//    return 0;
//}
//
//static void
//audio_process(Audio* a) {
//    if (a->stopRequested) {
//        Mix_HaltMusic();
//    }
//
//    for (int i = 0; i < a->queue.size; i++) {
//        Mix_PlayChannel(-1, a->sounds, 0);
//    }
//
//    a->queue.size = 0;
//}
//
//static void
//audio_shutdown(Audio* a) {
//    Mix_FreeChunk(a->sounds);
//    Mix_FreeMusic(a->music);
//    Mix_CloseAudio();
//}
//
