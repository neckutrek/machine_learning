#ifndef INCGRD_SURFACE_H
#define INCGRD_SURFACE_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Surface
{
private:
   SDL_Surface*         m_surface;
   SDL_PixelFormat*     m_format;
   SDL_Rect             m_clipRect;

public:
   Surface();
   ~Surface();

   Surface(const Surface& other);
   Surface(const Surface&& other) noexcept;

   Surface& operator=(const Surface& other);
   Surface& operator=(const Surface&& other) noexcept;

   void                 create(
                           const std::string& filepath,
                           const SDL_PixelFormat* format = nullptr);

   const SDL_Surface*   getSDLSurface() const;

   const SDL_Rect*      getSDLRect() const;

};

#endif // !INCGRD_SURFACE_H