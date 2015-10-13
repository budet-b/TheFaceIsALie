# include "pixel_operations.h"
 
static inline
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
  int bpp = surf->format->BytesPerPixel;
  return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}
 
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    return *p;
  case 2:
    return *(Uint16 *)p;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
  case 4:
    return *(Uint32 *)p;
  }
  return 0;
}
 
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(surface, x, y);
  switch(surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}


void ToGray(SDL_Surface *surface) {
    SDL_LockSurface(surface);
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint32 graypixel;
    for(int i = 0; i < surface->w; i++) { //Column route
        for(int j = 0; j < surface->h; j++){
            Uint32 pixel =  getpixel(surface,i,j);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            r = r * 0.3 + g * 0.53 + b * 0.11;
            graypixel = SDL_MapRGB(surface->format, r, r, r);
            putpixel(surface, i, j, graypixel);
        }
    }
    SDL_UnlockSurface(surface);
}

