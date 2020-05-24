#include <iostream>
#include <memory>
#include <filesystem>

#include "window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void InitSDL()
{
   if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
   {
      throw std::runtime_error(
         "SDL could not initialize! SDL_Error: "
         + std::string(SDL_GetError()));
   }
}

void QuitSDL()
{
   SDL_Quit();
}

void InitIMG()
{
   int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
   if( !( IMG_Init( flags ) & flags ) )
   {
      throw std::runtime_error(
         "SDL_image could not initialize: "
         + std::string(IMG_GetError()) );
   }
}

SDL_Surface* loadImage(
   const std::filesystem::path& filepath,
   const SDL_PixelFormat* format = nullptr)
{
   SDL_Surface* surface = IMG_Load( filepath.c_str() );
   if (!surface)
   {
      throw std::runtime_error("Error while loading image: " + std::string(IMG_GetError()));
   }

   SDL_Surface* optimizedSurface = surface;
   if (format)
   {
      optimizedSurface = SDL_ConvertSurface( surface, format, 0 );
      SDL_FreeSurface(surface);
      if (!optimizedSurface)
      {
         throw std::runtime_error("Error while loading image: " + std::string(SDL_GetError()));
      }
   }

   return optimizedSurface;
}

int main(int argc, char* args[])
{
   try
   {
      InitSDL();

      std::unique_ptr<Window> window = std::make_unique<Window>();
      window->create("K-Means Clustering", 1024, 768);

      window->fillWithColor(0xa0, 0xa0, 0xa0);
      window->update();

      std::string filename = "anthonyjoshua.png";
      std::filesystem::path imgPath = std::filesystem::current_path() / "../data/" / filename;
      SDL_Surface* surface = loadImage(imgPath, window->getFormat());
      SDL_Texture* texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
      SDL_Rect srcRect = {0, 0, 0, 0};
      SDL_GetClipRect(surface, &srcRect);
      SDL_FreeSurface(surface);

      SDL_Rect dstRect = srcRect;
      dstRect.x = 1024/2 - srcRect.w/2;
      dstRect.y = 768/2 - srcRect.h/2;
      SDL_RenderClear(window->getRenderer());
      SDL_RenderCopy(window->getRenderer(), texture, &srcRect, &dstRect);
      SDL_RenderPresent(window->getRenderer());

      SDL_Delay( 5000 );
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }

   QuitSDL();

   return 0;
}
