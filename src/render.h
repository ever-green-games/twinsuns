
#ifndef RENDER_HEAD // yep, another const overlap <3
#define RENDER_HEAD

// TODO: implement 
#define RTYPE_PP 0
#define RTYPE_43 1

extern RenderTexture2D rtex;
extern Rectangle rtex_drect;

void init_render();
void update_rtex_drect();
void render();
void render_and_wait(int frames);

#endif
