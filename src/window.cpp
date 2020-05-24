#include "window.h"

#include <stdexcept>

Window::Window()
: m_width(1024), m_height(768), m_title(""),
  m_window(nullptr), m_renderer(nullptr), m_format(nullptr)
{
   // empty ctor
}

Window::~Window()
{
   if (m_window)
   {
      SDL_DestroyWindow( m_window );
   }

   if (m_format)
   {
      SDL_FreeFormat(m_format);
   }
}

void Window::create(
   std::string title,
   unsigned int width,
   unsigned int height)
{
   if ( !(SDL_WasInit(SDL_INIT_EVERYTHING) & SDL_INIT_VIDEO) )
   {
      throw std::runtime_error("Error when trying to create Window: SDL Video hasn't been initialized!");
   }

   SDL_Window* window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      width,
      height,
      SDL_WINDOW_SHOWN);

   if (!window)
   {
      throw std::runtime_error(
         "Error when trying to create Window: "
         + std::string( SDL_GetError() ) );
   }

   SDL_Renderer *renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

   if (!renderer)
   {
      SDL_DestroyWindow(window);
      throw std::runtime_error(
         "Error when trying to create Window: "
         + std::string(SDL_GetError()));
   }

   m_title = title;
   m_width = width;
   m_height = height;
   m_window = window;
   m_renderer = renderer;
}

void Window::draw(
   const Surface& surface,
   int x,
   int y)
{
   if (!m_window)
   {
      throw std::runtime_error("Error when drawing surface on window: Window hasn't been initialized!");
   }

   if (!m_renderer)
   {
      throw std::runtime_error("Error when drawing surface on window: Renderer hasn't been initialized!");
   }

   const SDL_Surface* sdlSurface = surface.getSDLSurface();
   if (!sdlSurface)
   {
      throw std::runtime_error("Error when drawing surface on window: Surface hasn't been initialized!");
   }

   const SDL_Rect* srcRect = surface.getSDLRect();
   if (!srcRect)
   {
      throw std::runtime_error("Error when drawing surface on window: Surface doesn't have a proper clip rect!");
   }

   // SDL has a louse API that's why...
   SDL_Surface* sf = const_cast<SDL_Surface*>(sdlSurface);
   SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, sf);

   SDL_Rect dstRect = {
      x, y, srcRect->w, srcRect->h};

   SDL_RenderCopy(m_renderer, texture, srcRect, &dstRect);
}

void Window::fillWithColor(
   uint8_t red,
   uint8_t green,
   uint8_t blue,
   uint8_t alpha)
{
   if (!m_renderer)
   {
      throw std::runtime_error(
         "Error when trying to fill window with color:"
         + std::string(SDL_GetError()));
   }

   SDL_SetRenderDrawColor(m_renderer, red, green, blue, alpha);
   SDL_RenderDrawRect(m_renderer, nullptr);
}

const SDL_PixelFormat* Window::getFormat()
{
   if (!m_format)
   {
      if (!m_window)
      {
         throw std::runtime_error(
            "Error when trying to get windows format: m_window is nullptr!");
      }

      uint32_t fmt = SDL_GetWindowPixelFormat(m_window);
      if (fmt == SDL_PIXELFORMAT_UNKNOWN)
      {
         throw std::runtime_error(
            "Error when trying to get windows format:"
            + std::string(SDL_GetError()));
      }

      m_format = SDL_AllocFormat(fmt);
      if (!m_format)
      {
         throw std::runtime_error(
            "Error when trying to get windows format:"
            + std::string(SDL_GetError()));
      }
   }

   return m_format;
}

SDL_Renderer* Window::getRenderer()
{
   if (!m_renderer)
   {
      throw std::runtime_error("Trying to get uninitilized window renderer!");
   }
   return m_renderer;
}

void Window::clear()
{
   if (m_renderer)
   {
      SDL_RenderClear(m_renderer);
   }
}

void Window::present()
{
   if (m_renderer)
   {
      SDL_RenderPresent(m_renderer);
   }
}
