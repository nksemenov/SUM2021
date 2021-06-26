/* FILE NAME: rndtex.c
 * PROGRAMMER: ns6
 * DATE: 26.06.2021
 * PURPOSE: 3D animation rendering texture handle functions module.
 */

#include <string.h>

#include "../rnd.h"

/***
 * Textures stock functions
 ***/

/* Textures stock array and it size */
ns6TEXTURE NS6_RndTextures[NS6_MAX_TEXTURES];
INT NS6_RndTexturesSize;

/***
 * Base textures functions
 ***/

/* Texture stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndTexturesInit( VOID )
{
  NS6_RndTexturesSize = 0;
} /* End of 'NS6_RndTexturesInit' function */

/* Texture stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndTexturesClose( VOID )
{
  INT i;

  for (i = 0; i < NS6_RndTexturesSize; i++)
    glDeleteTextures(1, &NS6_RndTextures[i].TexId);
  NS6_RndTexturesSize = 0;
} /* End of 'NS6_RndTexturesInit' function */

/* Texture add to stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - texture bytes per pixel:
 *       INT C;
 *   - texture pixel colors pointer:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) shader number in stock.
 */
INT NS6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT MipLevels;

  if (NS6_RndTexturesSize >= NS6_MAX_SHADERS)
    return 0;
  strncpy(NS6_RndTextures[NS6_RndTexturesSize].Name, Name, NS6_STR_MAX - 1);
  NS6_RndTextures[NS6_RndTexturesSize].W = W;
  NS6_RndTextures[NS6_RndTexturesSize].H = H;
  
  /* Move image to GPU memory */
  glGenTextures(1, &NS6_RndTextures[NS6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, NS6_RndTextures[NS6_RndTexturesSize].TexId);

  MipLevels = log(W < H ? W : H) / log(2);
  MipLevels = MipLevels < 1 ? 1 : MipLevels;
  glTexStorage2D(GL_TEXTURE_2D, MipLevels, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
    C == 4 ? GL_RGBA : C == 3 ? GL_RGB : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return NS6_RndTexturesSize++;
} /* End of 'NS6_RndTexturesAdd' function */

/* END OF 'rndtex.c' FILE */
