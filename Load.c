#include <acknex.h> // Pure Mode

///////////////////////////////////////////////////////
// Draw a Mandelbrot fractal. 
double m_x = 0.344142,
  m_y = 0.075094,
  m_width = 0.017813;

long JetColor(double v)
{
   double d = 25.0;
   v = v/d + 0.5;
   int i = (int)v;
   int f = (int)(255*(v-i));
   int r=0, g=0, b=0;

   switch(i) {
     case 0: r=0; g=0; b=f; break;
     case 1: r=0; g=f; b=255; break;
     case 2: r=f; g=255; b=255-f; break;
     case 3: r= 255; g=255-f; b=0; break;
     case 4: r=255-f; g=0; b=0; break;
   }
   return (255<<24)|(r<<16)|(g<<8)|b;
}

void Draw(BMAP* bmap)
{
   var w = bmap_width(bmap);
   var h = bmap_height(bmap);
   var i,j;
   var times,inset;
   double x,y,zx,zy,zxs,zys;
   long detail=100;

   bmap_lock(bmap);
   for(i=0;i<w;i++)
   for(j=0;j<h;j++)
   {
     x = m_x+((double)i)*m_width/w;
     y = m_y+((double)(h-j))*m_width/w;
     zx = 0;
     zy = 0;
     inset = 1;
     times = 0;
     while(inset && times<detail)
     {
       times++;
       zxs = zx*zx;
       zys = zy*zy;
       zy = 2*zx*zy+y;
       zx = zxs-zys+x;
       if (zxs+zys >= 4.0) inset=0;
     }
     if(inset)
       pixel_to_bmap(bmap,i,j,255<<24);
     else
       pixel_to_bmap(bmap,i,j,JetColor(times));
   }
   bmap_unlock(bmap);
}

///////////////////////////////////////////////////////
// Define the panel that carries the image
PANEL* mandel_panel = { }

// Left mouse button clicked somewhere on the image.
// Zoom in by 0.5, calculate a new position, and draw.
void mandel_click()
{
   double a;
   a = mouse_cursor.x/screen_size.x; 
   m_x += m_width*(a-0.5);
   a = mouse_cursor.y/screen_size.y; 
   m_y += m_width*(0.5-a);
   m_width *= 0.5;
   Draw(mandel_panel->bmap);
}

void main()
{
// Create a bitmap, screen sized, for the background. 
   mandel_panel.bmap = 
    bmap_createblack(screen_size.x,screen_size.y,8888);
// make the panel visible on screen
   mandel_panel.flags |= SHOW;
// don't draw before the D3D device is created    
   wait(1); 
// Now we can draw 
   Draw(mandel_panel.bmap);
// Set a mouse event for redrawing the image
   on_mouse_left = mandel_click;
}
