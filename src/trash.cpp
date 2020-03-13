//#include <iostream>
//#include <SDL2/SDL.h>
//#include "entity.h"
//#include "trash.h"
//#include "graphics_context.h"
//
//Trash::Trash(GraphicsContext& context)
//{
//    _texture = context.getTexture("trash.png");
//    _rect.x = 320;
//    _rect.y = 320;
//    _rect.h = 32;
//    _rect.w = 32;
//}
//
//Trash::~Trash()
//{
//    SDL_DestroyTexture(_texture);
//}
//
//bool Trash::contains(int x, int y)
//{
//    return x > 320 &&
//        x < 336 &&
//        y > 320 &&
//        y < 336;
//}
//
//void Trash::interact()
//{
//    std::cout << "Trash" << std::endl;
//}
//
//void Trash::draw(SDL_Renderer* renderer)
//{
//    SDL_RenderCopy(renderer, _texture, NULL, &_rect);
//}
