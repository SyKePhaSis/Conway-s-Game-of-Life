#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<cmath>
#include<iostream>
#include<string>

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT  1000
#define ARRAY_LEN 500


void run(SDL_Renderer* renderer, int blobs[ARRAY_LEN][ARRAY_LEN]);
void draw(SDL_Renderer* renderer, int blobs[ARRAY_LEN][ARRAY_LEN], bool info);
void update(int blobs_prev_gen[ARRAY_LEN][ARRAY_LEN], int blobs_next_gen[ARRAY_LEN][ARRAY_LEN]);
void generate(int percent, int blobs[ARRAY_LEN][ARRAY_LEN]);

int main(int argc, char* args[])
{

  // CREATING DISPLAY ARRAY
  int blobs[ARRAY_LEN][ARRAY_LEN];
  generate(50,blobs);

  //INITIALIZING SDL VARIABLES
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  SDL_Renderer* renderer = NULL;
  
  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
  else
    {
      //Create window
      window = SDL_CreateWindow( "Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
       if( window == NULL )
        {
	  printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
      else
        {
	  
	  //GET RENDERER 
	  renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED);

	  //Get window surface
	  screenSurface = SDL_GetWindowSurface( window ); 
	      
	  //Fill the surface white
	  SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ));
	  
	  //Update the surface
 	  SDL_UpdateWindowSurface( window );
	  
	  run(renderer,blobs);
        }
    }
  return 0;
}

void run(SDL_Renderer* renderer, int blobs_prev_gen[ARRAY_LEN][ARRAY_LEN])
{
  int close = 0;
  int blobs_next_gen[ARRAY_LEN][ARRAY_LEN];
  int speed = 1;
  int percentage = 50;
  bool PAUSE = false;
  bool display_info = true;
  
  while(!close){
    SDL_Event event;
    
    while(SDL_PollEvent(&event))
      {
	switch(event.type)
	  {
	    case SDL_QUIT:
	      close = 1;
	      break;
	      
	  case SDL_KEYDOWN:
	    switch(event.key.keysym.scancode)
	      {
	      case SDL_SCANCODE_ESCAPE:
		close = 1;
		break;
		
	      case SDL_SCANCODE_R:
		generate(percentage,blobs_next_gen);
		break;
		
	      case SDL_SCANCODE_W:
		speed++;
		break;
		
	      case SDL_SCANCODE_S:
		if(speed > 1)
		  {
		    speed--;
		  }
		break;

	      case SDL_SCANCODE_P:
		PAUSE = !PAUSE;
		break;
		
	      case SDL_SCANCODE_A:
		if(percentage > 10)
		  {
		    percentage = percentage - 10;
		  }
		break;
		
	      case SDL_SCANCODE_Q:
		if(percentage < 100)
		  {
		    percentage = percentage + 10;
		  }
		break;

	      case SDL_SCANCODE_U:
		display_info = !display_info;
		break;
	      }
	  }
      }
    if(!PAUSE)
      { 
	update(blobs_prev_gen, blobs_next_gen);
      }
    draw(renderer, blobs_next_gen, display_info);
    blobs_prev_gen = blobs_next_gen;
    SDL_Delay(100*speed);
  }
  SDL_Quit();
}

void draw_info(SDL_Renderer* renderer)
{
  // INFO BOX
  SDL_Rect info_rect;
  info_rect.w = 450;
  info_rect.h = 220;
  info_rect.x = 50;
  info_rect.y = 50;
  SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0,0,0,180);
  SDL_RenderFillRect(renderer, &info_rect);

  //TEXT

  TTF_Init();
  TTF_Font* font = TTF_OpenFont("./Assets/OpenSans-Semibold.ttf",18);
  if(!font) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  SDL_Color color = {255,160,0};
  std::string titles[8] = {"[ESC] Press button to quit.", "[R] Press R to generate new map.", "[W]Slow Down process", "[S]Speed up process", "[P]Pause the process", "[Q]Increase the percentage of cells in the board", "[A]Decrease the percentage of cells in the board", "[U] Dispaly key-bindings"}; 
  for(int i = 0; i < 8; i++)
    {
  SDL_Texture* text_texture;
  SDL_Surface* text;
  text = TTF_RenderText_Solid(font,titles[i].c_str(), color);
  text_texture = SDL_CreateTextureFromSurface(renderer, text);
  SDL_Rect dest = {60,60 + i*text->h,text->w,text->h};
  SDL_RenderCopy(renderer,text_texture, NULL,&dest);
    }
  TTF_CloseFont(font);
}

void draw(SDL_Renderer* renderer,int blobs[ARRAY_LEN][ARRAY_LEN], bool info)
{
  SDL_RenderClear(renderer);
  for(int i = 0; i < ARRAY_LEN; i++)
    {
      for(int j = 0; j < ARRAY_LEN; j++)
	{ 
	  SDL_Rect rect;
	  rect.w = SCREEN_WIDTH/ARRAY_LEN;
	  rect.h = SCREEN_HEIGHT/ARRAY_LEN;
	  rect.x = (SCREEN_WIDTH/ARRAY_LEN)*j;
	  rect.y = (SCREEN_HEIGHT/ARRAY_LEN)*i;

	  if(blobs[i][j] == 1)
	    {
	      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	      
	    }
	  else if(blobs[i][j] == 0)
	    {
	      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	    }
	  else
	    {
	      SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	    }
	    
	  SDL_RenderFillRect(renderer, &rect);
	}
    }
  if(info)
    {
      draw_info(renderer);
    }
  // PRESENTING RENDERER
  SDL_RenderPresent(renderer);
}

void update(int blobs_prev_gen[ARRAY_LEN][ARRAY_LEN], int blobs_next_gen[ARRAY_LEN][ARRAY_LEN])
{
  for(int i = 0; i < ARRAY_LEN; i++)
    {
      for(int j = 0; j < ARRAY_LEN; j++)
	{
	  int COUNT = 0;
	  for(int k = -1; k < 2; k++)
	    {
	      if(i+k >= 0 && j-1 >= 0 && i+k <= ARRAY_LEN - 1 && *(*(blobs_prev_gen  + i + k) + j - 1) == 1){COUNT++;}
	      if(i+k <= ARRAY_LEN-1 && i+k >= 0 && j+1 <= ARRAY_LEN-1 && *(*(blobs_prev_gen + i + k) + j + 1) == 1){COUNT++;}
	    }
	  if(i - 1 >= 0 && *(*(blobs_prev_gen + i - 1) + j) == 1){COUNT++;}
	  if(i + 1<= ARRAY_LEN-1 && *(*(blobs_prev_gen + i + 1) + j) == 1){COUNT++;}
	  if(COUNT == 2 || COUNT == 3 )
	    {
	      if(*(*(blobs_prev_gen + i) + j) == 1)
		{
		  *(*(blobs_next_gen + i) + j) = 1;
		}
	      if(*(*(blobs_prev_gen + i) + j) == 0  && COUNT == 3)
	      	{
		  *(*(blobs_next_gen + i) + j) = 1;
		}
	      }
	  else
	    {
	      *(*(blobs_next_gen + i) + j) = 0;
	    } 
	}
    }
}

void generate(int percent, int blobs[ARRAY_LEN][ARRAY_LEN])
{
  if(percent >= 0 && percent <= 100)
    {
      for(int i = 0; i < ARRAY_LEN; i++)
	{
	  for(int j = 0; j < ARRAY_LEN; j++)
	    {
	      int num = rand() % 100;
	      if(num >= percent){num = 1;}
	      else{num = 0;}
	      blobs[i][j] = num;
	    }
	}
    }
}
