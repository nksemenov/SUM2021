/* FILE NAME: rndshd.c
 * PROGRAMMER: ns6
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
ns6SHADER NS6_RndShaders[NS6_MAX_SHADERS];
INT NS6_RndShadersSize;

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
VOID NS6_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/shd{30}nm4.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} /* End of 'NS6_RndLoadTextFromFile' function */

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
CHAR * NS6_RndLoadTextFromFile( CHAR *FileName )
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
} /* End of 'NS6_RndLoadTextFromFile' function */

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (INT) load shader program Id.
 */
INT NS6_RndShdLoad( CHAR *FileNamePrefix )
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
    txt = NS6_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      NS6_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }

    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      NS6_RndShdLog(FileNamePrefix, shd[i].Name, "Error create shader");
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
      NS6_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      NS6_RndShdLog(FileNamePrefix, "PROG", "Error create program");
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
        NS6_RndShdLog(FileNamePrefix, "PROG", Buf);
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
} /* End of 'NS6_RndShdLoad' function */

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETUNS: None.
 */
VOID NS6_RndShdFree( INT ProgId )
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
} /* End of 'NS6_RndShdLoad' function */

/***
 * Shaders stock functions
 ***/

/* Shader stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndShadersInit( VOID )
{
  NS6_RndShadersSize = 0;
  NS6_RndShaderAdd("DEFAULT");
} /* End of 'NS6_RndShadersInit' function */

/* Shader stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndShadersClose( VOID )
{
  INT i;

  for (i = 0; i < NS6_RndShadersSize; i++)
    NS6_RndShdFree(NS6_RndShaders[i].ProgId);
  NS6_RndShadersSize = 0;
} /* End of 'NS6_RndShadersInit' function */

/* Shader add to stock function.
 * ARGUMENTS:
 *   - shader folder prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader number in stock.
 */
INT NS6_RndShaderAdd( CHAR *FileNamePrefix )
{
  INT i;

  for (i = 0; i < NS6_RndShadersSize; i++)
    if (strcmp(FileNamePrefix, NS6_RndShaders[i].Name) == 0)
      return i;
  if (NS6_RndShadersSize >= NS6_MAX_SHADERS)
    return 0;
  strncpy(NS6_RndShaders[NS6_RndShadersSize].Name, FileNamePrefix, NS6_STR_MAX - 1);
  NS6_RndShaders[NS6_RndShadersSize].ProgId = NS6_RndShdLoad(FileNamePrefix);
  return NS6_RndShadersSize++;
} /* End of 'NS6_RndShadersAdd' function */

/* Shader stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NS6_RndShadersUpdate( VOID )
{
  INT i;

  for (i = 0; i < NS6_RndShadersSize; i++)
  {
    NS6_RndShdFree(NS6_RndShaders[i].ProgId);
    NS6_RndShaders[i].ProgId = NS6_RndShdLoad(NS6_RndShaders[i].Name);
  }
} /* End of 'NS6_RndShadersUpdate' function */

/* END OF 'rndshd.c' FILE */
