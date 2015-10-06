#include "image.h"


SDL_Surface* load_image(char *path) {
     SDL_Surface  *image;
  image = IMG_Load(path);
  if (!image) {
    errx(3, "Unable to load image at : %s : %s", path,IMG_GetError());
  }
    return image;
}

  static
Uint8* pixelref(SDL_Surface *image, unsigned x, unsigned y) 
{
  int pix = image->format->BytesPerPixel;
  return (Uint8*)image->pixels + y * image->pitch + x * pix;
}


void putpixel(SDL_Surface *image, unsigned x, unsigned y, Uint32 pixel) {
  Uint8 *p = pixelref(image, x, y);
  switch(image->format->BytesPerPixel) {
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

Uint32 getpixel(SDL_Surface *image, unsigned x, unsigned y) {
    Uint8 *p = pixelref(image, x, y);
    if(p==NULL)
        return 0;
  switch(image->format->BytesPerPixel) {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3: //Compatibility for processor types (BIG_ENDIAN for PowerPC/ARM ... | LIL_ENDIAN for x86/x64 ...)
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
  }
  return 0;
}


void ToGray(SDL_Surface *image) {
    SDL_LockSurface(image);
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint32 graypixel;
    for(int i = 0; i < image->w; i++) { //Column route
        for(int j = 0; j < image->h; j++){
            Uint32 pixel =  getpixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            r = r * 0.3 + g * 0.53 + b * 0.11;
            graypixel = SDL_MapRGB(image->format, r, r, r);
            putpixel(image, i, j, graypixel);
        }
    }
    SDL_UnlockSurface(image);
}

void Binarize(SDL_Surface *surface)
{
  SDL_LockSurface(surface);
  Uint8 r=0;
  Uint8 g=0;
  Uint8 b=0;
  Uint32 pink;
  for(int i =0; i < surface->w; i++)
  {
    for (int j=0; j < surface->h; j++)
    {
      Uint32 pixel = getpixel(surface,i,j);
      SDL_GetRGB(pixel, surface->format,&r,&g,&b);
      if (r > 125)
      {
        r =255;
      }
      else
      {
        r = 0;
      }
      pink = SDL_MapRGB(surface->format,r,r,r);
      putpixel(surface,i,j,pink);
    }
  }
  SDL_UnlockSurface(surface);
}

SDL_Surface integralImage(SDL_Surface *image) {
    SDL_Surface *outImage = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    ToGray(outImage);
    Binarize(outImage);
    Uint32 pixel = getpixel(image,0,0);
    putpixel(outImage,0,0,pixel);
    /*for(int i = 0; i < image->w; i++){
        pixel = getpixel(image,i,1)+getpixel(outImage,i-1,1);
        putpixel(outImage,i,1,pixel);
    }
    for(int i = 0; i < image->h; i++){
        pixel = getpixel(image,1,i);
        putpixel(outImage,1,i,pixel);
    }*/ 
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
    	{
           pixel = getpixel(image,i,j) + getpixel(outImage,i,j-1) + getpixel(image,i-1,j) - getpixel(outImage,i-1,j-1);
           putpixel(outImage,i,j,pixel);
        }
    }
    return *outImage;
}
