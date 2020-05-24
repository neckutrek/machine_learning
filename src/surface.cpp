#include "surface.h"

#include <stdexcept>
#include <string>

static bool IMG_INIT_DONE = false;

void TryInitIMG()
{
   if (IMG_INIT_DONE)
   {
      return;
   }

   if ( !(SDL_WasInit(SDL_INIT_EVERYTHING) & SDL_INIT_VIDEO) )
   {
      throw std::runtime_error(
         "SDL_image could not initialize: SDL has not been initialized!");
   }

   int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
   if( !( IMG_Init( flags ) & flags ) )
   {
      throw std::runtime_error(
         "SDL_image could not initialize: "
         + std::string(IMG_GetError()) );
   }

   IMG_INIT_DONE = true;
}

Surface::Surface()
: m_surface(nullptr), m_format(nullptr),
  m_clipRect{0, 0, 0, 0}
{
   TryInitIMG();
}

Surface::~Surface()
{
   if (m_surface)
   {
      SDL_FreeSurface(m_surface);
   }
}

void Surface::create(
   const std::string& filepath,
   const SDL_PixelFormat* format)
{
   if (m_surface)
   {
      throw std::runtime_error("Error while creating surface: Object has already loaded a surface!");
   }

   SDL_Surface* surface = IMG_Load( filepath.c_str() );
   if (!surface)
   {
      throw std::runtime_error("Error while creating surface: " + std::string(IMG_GetError()));
   }

   SDL_Surface* optimizedSurface = surface;
   if (format)
   {
      optimizedSurface = SDL_ConvertSurface( surface, format, 0 );
      SDL_FreeSurface(surface);
      if (!optimizedSurface)
      {
         throw std::runtime_error("Error while creating surface: " + std::string(SDL_GetError()));
      }
   }

   m_surface = optimizedSurface;
   m_format = m_surface->format;
   SDL_GetClipRect(m_surface, &m_clipRect);
}

const SDL_Surface* Surface::getSDLSurface() const
{
   return m_surface;
}

const SDL_Rect* Surface::getSDLRect() const
{
   return &m_clipRect;
}