#define DecalFnNone 0
#define DecalFnDelete 1
#define DecalFnDestroy 2
#define DecalFnShade 3

typedef struct _Decal Decal;

struct _Decal {
  int x, y, width, height;
  short int b1_fn; /* any of the DecalFn* constants declared in this file */
  short int b2_fn; /* any of the DecalFn* constants declared in this file */
  short int b3_fn; /* any of the DecalFn* constants declared in this file */
  Imlib_Image image;
  Decal *next;
};
