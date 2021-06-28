#ifndef __rndres_h_
#define __rndres_h_

#include "../../../def.h"

#define NS6_STR_MAX 300

typedef struct tagns6SHADER
{
  CHAR Name[NS6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} ns6SHADER;

#define NS6_MAX_SHADERS 30
extern ns6SHADER NS6_RndShaders[NS6_MAX_SHADERS];
extern INT NS6_RndShadersSize;


VOID NS6_RndShadersInit( VOID );
VOID NS6_RndShadersClose( VOID );
INT NS6_RndShaderAdd( CHAR *FileNamePrefix );
VOID NS6_RndShadersUpdate( VOID );

#define NS6_MTL_SIZE 100
#define NS6_MAX_MATERIALS 10

typedef struct tagns6MATERIAL
{
  CHAR Name[NS6_MTL_SIZE];
  VEC Ka, Kd, Ks;
  FLT Ph;
  FLT Trans;
  INT Tex[8];

  /* Shader string */
  CHAR ShaderStr[NS6_MTL_SIZE];
  INT ShdNo;
} ns6MATERIAL;

extern ns6MATERIAL NS6_RndMaterial[NS6_MAX_MATERIALS];
extern INT NS6_RndMaterialSize;

VOID NS6_RndMtlInit( VOID );
VOID NS6_RndMtlClose( VOID );
INT NS6_RndMtlAdd( ns6MATERIAL *mtl );
INT NS6_RndMtlApply( INT MtlNo );

/***
 * Textures stock functions
 ***/

#define NS6_MAX_TEXTURES 30

typedef struct tagns6TEXTURE
{
  CHAR Name[NS6_STR_MAX]; /* Texture name */
  INT TexId;              /* Texture Id */
  INT W, H;               /* Texture size */
} ns6TEXTURE;

/* Textures stock array and it size */
extern ns6TEXTURE NS6_RndTextures[NS6_MAX_TEXTURES];
extern INT NS6_RndTexturesSize;

/***
 * Base textures functions
 ***/

/* Texture stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndTexturesInit( VOID );

/* Texture stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndTexturesClose( VOID );

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
INT NS6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

#endif