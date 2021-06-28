/* FILE NAME: rndshd.c
 * PROGRAMMER: NM6
 * DATE: 23.06.2021
 * PURPOSE: 3D animation rendering shader handle functions module.
 */

#include <stdio.h>
#include <string.h>

#include "../rnd.h"

/***
 * Base shaders functions
 ***/


/* Shadre stock array and it size */
nm6SHADER NM6_RndShaders[NM6_MAX_SHADERS];
INT NM6_RndShadersSize;

/* Save log to file function.
 * ARGUMENTS:
 *   - shader prefix:
 *       CHAR *FileNamePrefix;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - error text:
 *       CHAR *Text;
 * RETURNS: None.
 */
VOID NM6_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/shd{30}nm4.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} /* End of 'NM6_RndLoadTextFromFile' function */

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
CHAR * NM6_RndLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  /* Load text */
  rewind(F);
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
} /* End of 'NM6_RndLoadTextFromFile' function */

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (INT) load shader program Id.
 */
INT NM6_RndShdLoad( CHAR *FileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader name (e.g. "VERT") */
    INT Type;   /* Shader type (e.g. GL_VERTEX_SHADER) */
    INT Id;     /* Obtained shader Id from OpenGL */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER, 0},
    {"FRAG", GL_FRAGMENT_SHADER, 0},
  };
  INT NoofS = sizeof(shd) / sizeof(shd[0]), i, prg, res;
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[1000];

  /* Load shader */
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader name */
    sprintf(Buf, "BIN/SHADERS/%s/%s.GLSL", FileNamePrefix, shd[i].Name);

    /* Load shader text from file */
    txt = NM6_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      NM6_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }

    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      NM6_RndShdLog(FileNamePrefix, shd[i].Name, "Error create shader");
      is_ok = FALSE;
      break;
    }

    /* Attach text to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);

    /* Handle errors */
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      NM6_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      NM6_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        NM6_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }

  /* Handle errors */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'NM6_RndShdLoad' function */

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETUNS: None.
 */
VOID NM6_RndShdFree( INT ProgId )
{
  INT shds[5], n, i;

  if (ProgId == 0 || !glIsProgram(ProgId))
    return;
  glGetAttachedShaders(ProgId, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(ProgId, shds[i]);
      glDeleteShader(shds[i]);
    }
    glDeleteProgram(ProgId);
} /* End of 'NM6_RndShdLoad' function */

/***
 * Shaders stock functions
 ***/

/* Shader stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NM6_RndShadersInit( VOID )
{
  NM6_RndShadersSize = 0;
  NM6_RndShaderAdd("DEFAULT");
} /* End of 'NM6_RndShadersInit' function */

/* Shader stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NM6_RndShadersClose( VOID )
{
  INT i;

  for (i = 0; i < NM6_RndShadersSize; i++)
    NM6_RndShdFree(NM6_RndShaders[i].ProgId);
  NM6_RndShadersSize = 0;
} /* End of 'NM6_RndShadersInit' function */

/* Shader add to stock function.
 * ARGUMENTS:
 *   - shader folder prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader number in stock.
 */
INT NM6_RndShaderAdd( CHAR *FileNamePrefix )
{
  INT i;

  for (i = 0; i < NM6_RndShadersSize; i++)
    if (strcmp(FileNamePrefix, NM6_RndShaders[i].Name) == 0)
      return i;
  if (NM6_RndShadersSize >= NM6_MAX_SHADERS)
    return 0;
  strncpy(NM6_RndShaders[NM6_RndShadersSize].Name, FileNamePrefix, NM6_STR_MAX - 1);
  NM6_RndShaders[NM6_RndShadersSize].ProgId = NM6_RndShdLoad(FileNamePrefix);
  return NM6_RndShadersSize++;
} /* End of 'NM6_RndShadersAdd' function */

/* Shader stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NM6_RndShadersUpdate( VOID )
{
  INT i;

  for (i = 0; i < NM6_RndShadersSize; i++)
  {
    NM6_RndShdFree(NM6_RndShaders[i].ProgId);
    NM6_RndShaders[i].ProgId = NM6_RndShdLoad(NM6_RndShaders[i].Name);
  }
} /* End of 'NM6_RndShadersUpdate' function */

/* END OF 'rndshd.c' FILE */
