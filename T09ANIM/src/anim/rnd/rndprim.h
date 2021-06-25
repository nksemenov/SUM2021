/***
 * Primitive collection support
 ***/

/* Primitive collection data type */
typedef struct tagns6PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  ns6PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} ns6PRIMS;

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL NS6_RndPrimsCreate( ns6PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 * RETURNS: None.
 */
VOID NS6_RndPrimsFree( ns6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID NS6_RndPrimsDraw( ns6PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL NS6_RndPrimsLoad( ns6PRIMS *Prs, CHAR *FileName );

