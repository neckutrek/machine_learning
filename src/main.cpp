#include <iostream>
#include <memory>
#include <filesystem>

#include "window.h"
#include "surface.h"

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

int main(int argc, char* args[])
{
   try
   {
      InitSDL();

      std::unique_ptr<Window> window = std::make_unique<Window>();
      window->create("K-Means Clustering", 1024, 768);

      window->fillWithColor(0xa0, 0xa0, 0xa0);
      window->clear();
      window->present();

      std::string filename = "anthonyjoshua.png";
      std::filesystem::path imgPath = std::filesystem::current_path() / "../data/" / filename;

      std::unique_ptr<Surface> surface = std::make_unique<Surface>();
      surface->create(imgPath, window->getFormat());

      window->clear();
      window->draw(*surface, 1024/2-200, 768/2-200);
      window->present();

      SDL_Delay( 5000 );
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }

   QuitSDL();

   return 0;
}
