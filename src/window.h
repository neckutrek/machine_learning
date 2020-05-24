#ifndef INCGRD_WINDOW_H
#define INCGRD_WINDOW_H

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

class Window
{
private:
   unsigned int      m_width;
   unsigned int      m_height;
   std::string       m_title;

   SDL_Window*       m_window;
   SDL_Renderer*     m_renderer;
   SDL_PixelFormat*  m_format;

public:
                     Window();
                     ~Window();

   void              create(
                        std::string title = "",
                        unsigned int width = 1024,
                        unsigned int height = 768);

   void              fillWithColor(
                        uint8_t red,
                        uint8_t green,
                        uint8_t blue,
                        uint8_t alpha = 0xff);

   const SDL_PixelFormat*
                     getFormat();

   ///
   /// Will return non-nullptr pointer or throw exception
   SDL_Renderer*     getRenderer();

   void              update();
};

#endif // INCGRD_WINDOW_H
