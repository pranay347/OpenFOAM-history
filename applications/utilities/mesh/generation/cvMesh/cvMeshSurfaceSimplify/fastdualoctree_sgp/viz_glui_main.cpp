/**
 * @file    glui_main.h
 * @author  Thomas Lewiner <thomas.lewiner@polytechnique.org>
 * @author  Math Dept, PUC-Rio
 * @version 0.1
 * @date    30/05/2006
 *
 * @brief   Octree graphical interface: commandline parser and main pipeline
 */
//________________________________________________


#if !defined(WIN32) || defined(__CYGWIN__)
#pragma implementation "viz_glui_defs.h"
#endif // WIN32

#if WIN32 && _DEBUG
#include <vld.h>
#endif // WIN32 && _DEBUG

#include "viz_glui_defs.h"
#include "hash.h"


//_____________________________________________________________________________
// declarations of this file

// main window id
int  main_window  = -1 ;

// export movie switch
int export_movie  =  0 ;

//-----------------------------------------------------------------------------

// idle behaviour: redraw constantly
void idle() ;

//_____________________________________________________________________________





//_____________________________________________________________________________
// idel behaviour: redraw constantly
void idle()
//-----------------------------------------------------------------------------
{
  if ( ::glutGetWindow() != main_window ) ::glutSetWindow(main_window);


  if( auto_rotate )
  {
    objects_rot->iaction_mouse_down_handler( 0,0 ) ;
    objects_rot->iaction_mouse_held_down_handler( 0,0, 1 ) ;
    objects_rot->iaction_mouse_held_down_handler( 1,0, 1 ) ;
    objects_rot->iaction_mouse_up_handler( 1,0, 1 ) ;
  }

  if( auto_translate_x )
  {
    obj_pos[0] += auto_translate_x * 0.01f ;
    objects_mv->sync_live(0,1) ;
  }

  if( auto_translate_y )
  {
    obj_pos[1] += auto_translate_y * 0.01f ;
    objects_mv->sync_live(0,1) ;
  }

  if( auto_zoom )
  {
    obj_pos[2] += auto_zoom * 0.01f ;
    objects_zm->sync_live(0,1) ;
  }

  if( export_movie ) export_png() ;
  ::glutPostRedisplay();
}
//_____________________________________________________________________________




//_____________________________________________________________________________
// main
int main(int argc, char* argv[])
//-----------------------------------------------------------------------------
{
  printf( OCTREE_STRING " (hash %s pointers, %d bits) visualization with:\n\n", (USE_HASH_PTR ? "with" : "without"), HASH_BITS ) ;

  // create main window
  ::glutInit( &argc, argv ) ;
  ::glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  ::glutInitWindowPosition( 50, 50 );
  ::glutInitWindowSize( 800, 600 );
  main_window = ::glutCreateWindow( "Dual Octree" );



  //--------------------------------------------------//
  // Create the side and bottom subwindow
#ifdef WIN32
  create_bottom_panel() ;
  create_side_panel  () ;
#else  // WIN32
  create_side_panel  () ;
  create_bottom_panel() ;
#endif // WIN32

  //--------------------------------------------------//
  // set callback functions
  ::glutMouseFunc        ( mouse );
  ::glutMotionFunc       ( motion );
  ::glutDisplayFunc      ( display );
  ::glutKeyboardFunc     ( keyboard );
  GLUI_Master.set_glutIdleFunc   ( idle    );
  GLUI_Master.set_glutReshapeFunc( reshape );

  //--------------------------------------------------//
  // init trackball
  init_trackballs() ;


  //--------------------------------------------------//

  ::glEnable( GL_DEPTH_TEST );
  ::glEnable(GL_COLOR_MATERIAL);
  ::glDisable(GL_NORMALIZE);
  
  ::glPolygonOffset( 1.0f, -0.1f );
  ::glEnable(GL_POLYGON_OFFSET_FILL) ;
  /*
  ::glEnable(GL_ALPHA_TEST);
  ::glEnable(GL_BLEND ) ;
  ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;  
*/
  ::glClearColor( 1,1,1,1 );
  
  ::glEnable(GL_LIGHTING);
  const GLfloat light0_ambient[4] =  {0.1f, 0.1f, 0.3f, 1.0f};
  ::glEnable(GL_LIGHT0);
  ::glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  ::glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

  const GLfloat light1_ambient[4] =  {0.1f, 0.1f, 0.3f, 1.0f};
  ::glEnable(GL_LIGHT1);
  ::glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  ::glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

  ::glShadeModel(GL_SMOOTH);

  //--------------------------------------------------//
  // texture color map
  GLuint colormap_texture;
  
  // allocate a texture name
  ::glGenTextures( 1, &colormap_texture );
  
  // select our current texture
  ::glBindTexture( GL_TEXTURE_1D, colormap_texture );
  
  // select modulate to mix texture with color for shading
  ::glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
  
  // when texture area is small, bilinear filter the closest mipmap
  ::glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  // when texture area is large, bilinear filter the original
  ::glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
  // the texture wraps over at the edges (repeat)
  ::glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  
  GLfloat color_jet[3 * 256] = { 0.000000f, 0.000000f, 0.515625f, 0.000000f, 0.000000f, 0.531250f, 0.000000f, 0.000000f, 0.546875f, 0.000000f, 0.000000f, 0.562500f, 0.000000f, 0.000000f, 0.578125f, 0.000000f, 0.000000f, 0.593750f, 0.000000f, 0.000000f, 0.609375f, 0.000000f, 0.000000f, 0.625000f, 0.000000f, 0.000000f, 0.640625f, 0.000000f, 0.000000f, 0.656250f, 0.000000f, 0.000000f, 0.671875f, 0.000000f, 0.000000f, 0.687500f, 0.000000f, 0.000000f, 0.703125f, 0.000000f, 0.000000f, 0.718750f, 0.000000f, 0.000000f, 0.734375f, 0.000000f, 0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.765625f, 0.000000f, 0.000000f, 0.781250f, 0.000000f, 0.000000f, 0.796875f, 0.000000f, 0.000000f, 0.812500f, 0.000000f, 0.000000f, 0.828125f, 0.000000f, 0.000000f, 0.843750f, 0.000000f, 0.000000f, 0.859375f, 0.000000f, 0.000000f, 0.875000f, 0.000000f, 0.000000f, 0.890625f, 0.000000f, 0.000000f, 0.906250f, 0.000000f, 0.000000f, 0.921875f, 0.000000f, 0.000000f, 0.937500f, 0.000000f, 0.000000f, 0.953125f, 0.000000f, 0.000000f, 0.968750f, 0.000000f, 0.000000f, 0.984375f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.015625f, 1.000000f, 0.000000f, 0.031250f, 1.000000f, 0.000000f, 0.046875f, 1.000000f, 0.000000f, 0.062500f, 1.000000f, 0.000000f, 0.078125f, 1.000000f, 0.000000f, 0.093750f, 1.000000f, 0.000000f, 0.109375f, 1.000000f, 0.000000f, 0.125000f, 1.000000f, 0.000000f, 0.140625f, 1.000000f, 0.000000f, 0.156250f, 1.000000f, 0.000000f, 0.171875f, 1.000000f, 0.000000f, 0.187500f, 1.000000f, 0.000000f, 0.203125f, 1.000000f, 0.000000f, 0.218750f, 1.000000f, 0.000000f, 0.234375f, 1.000000f, 0.000000f, 0.250000f, 1.000000f, 0.000000f, 0.265625f, 1.000000f, 0.000000f, 0.281250f, 1.000000f, 0.000000f, 0.296875f, 1.000000f, 0.000000f, 0.312500f, 1.000000f, 0.000000f, 0.328125f, 1.000000f, 0.000000f, 0.343750f, 1.000000f, 0.000000f, 0.359375f, 1.000000f, 0.000000f, 0.375000f, 1.000000f, 0.000000f, 0.390625f, 1.000000f, 0.000000f, 0.406250f, 1.000000f, 0.000000f, 0.421875f, 1.000000f, 0.000000f, 0.437500f, 1.000000f, 0.000000f, 0.453125f, 1.000000f, 0.000000f, 0.468750f, 1.000000f, 0.000000f, 0.484375f, 1.000000f, 0.000000f, 0.500000f, 1.000000f, 0.000000f, 0.515625f, 1.000000f, 0.000000f, 0.531250f, 1.000000f, 0.000000f, 0.546875f, 1.000000f, 0.000000f, 0.562500f, 1.000000f, 0.000000f, 0.578125f, 1.000000f, 0.000000f, 0.593750f, 1.000000f, 0.000000f, 0.609375f, 1.000000f, 0.000000f, 0.625000f, 1.000000f, 0.000000f, 0.640625f, 1.000000f, 0.000000f, 0.656250f, 1.000000f, 0.000000f, 0.671875f, 1.000000f, 0.000000f, 0.687500f, 1.000000f, 0.000000f, 0.703125f, 1.000000f, 0.000000f, 0.718750f, 1.000000f, 0.000000f, 0.734375f, 1.000000f, 0.000000f, 0.750000f, 1.000000f, 0.000000f, 0.765625f, 1.000000f, 0.000000f, 0.781250f, 1.000000f, 0.000000f, 0.796875f, 1.000000f, 0.000000f, 0.812500f, 1.000000f, 0.000000f, 0.828125f, 1.000000f, 0.000000f, 0.843750f, 1.000000f, 0.000000f, 0.859375f, 1.000000f, 0.000000f, 0.875000f, 1.000000f, 0.000000f, 0.890625f, 1.000000f, 0.000000f, 0.906250f, 1.000000f, 0.000000f, 0.921875f, 1.000000f, 0.000000f, 0.937500f, 1.000000f, 0.000000f, 0.953125f, 1.000000f, 0.000000f, 0.968750f, 1.000000f, 0.000000f, 0.984375f, 1.000000f, 0.000000f, 1.000000f, 1.000000f, 0.015625f, 1.000000f, 1.000000f, 0.031250f, 1.000000f, 0.984375f, 0.046875f, 1.000000f, 0.968750f, 0.062500f, 1.000000f, 0.953125f, 0.078125f, 1.000000f, 0.937500f, 0.093750f, 1.000000f, 0.921875f, 0.109375f, 1.000000f, 0.906250f, 0.125000f, 1.000000f, 0.890625f, 0.140625f, 1.000000f, 0.875000f, 0.156250f, 1.000000f, 0.859375f, 0.171875f, 1.000000f, 0.843750f, 0.187500f, 1.000000f, 0.828125f, 0.203125f, 1.000000f, 0.812500f, 0.218750f, 1.000000f, 0.796875f, 0.234375f, 1.000000f, 0.781250f, 0.250000f, 1.000000f, 0.765625f, 0.265625f, 1.000000f, 0.750000f, 0.281250f, 1.000000f, 0.734375f, 0.296875f, 1.000000f, 0.718750f, 0.312500f, 1.000000f, 0.703125f, 0.328125f, 1.000000f, 0.687500f, 0.343750f, 1.000000f, 0.671875f, 0.359375f, 1.000000f, 0.656250f, 0.375000f, 1.000000f, 0.640625f, 0.390625f, 1.000000f, 0.625000f, 0.406250f, 1.000000f, 0.609375f, 0.421875f, 1.000000f, 0.593750f, 0.437500f, 1.000000f, 0.578125f, 0.453125f, 1.000000f, 0.562500f, 0.468750f, 1.000000f, 0.546875f, 0.484375f, 1.000000f, 0.531250f, 0.500000f, 1.000000f, 0.515625f, 0.515625f, 1.000000f, 0.500000f, 0.531250f, 1.000000f, 0.484375f, 0.546875f, 1.000000f, 0.468750f, 0.562500f, 1.000000f, 0.453125f, 0.578125f, 1.000000f, 0.437500f, 0.593750f, 1.000000f, 0.421875f, 0.609375f, 1.000000f, 0.406250f, 0.625000f, 1.000000f, 0.390625f, 0.640625f, 1.000000f, 0.375000f, 0.656250f, 1.000000f, 0.359375f, 0.671875f, 1.000000f, 0.343750f, 0.687500f, 1.000000f, 0.328125f, 0.703125f, 1.000000f, 0.312500f, 0.718750f, 1.000000f, 0.296875f, 0.734375f, 1.000000f, 0.281250f, 0.750000f, 1.000000f, 0.265625f, 0.765625f, 1.000000f, 0.250000f, 0.781250f, 1.000000f, 0.234375f, 0.796875f, 1.000000f, 0.218750f, 0.812500f, 1.000000f, 0.203125f, 0.828125f, 1.000000f, 0.187500f, 0.843750f, 1.000000f, 0.171875f, 0.859375f, 1.000000f, 0.156250f, 0.875000f, 1.000000f, 0.140625f, 0.890625f, 1.000000f, 0.125000f, 0.906250f, 1.000000f, 0.109375f, 0.921875f, 1.000000f, 0.093750f, 0.937500f, 1.000000f, 0.078125f, 0.953125f, 1.000000f, 0.062500f, 0.968750f, 1.000000f, 0.046875f, 0.984375f, 1.000000f, 0.031250f, 1.000000f, 1.000000f, 0.015625f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.984375f, 0.000000f, 1.000000f, 0.968750f, 0.000000f, 1.000000f, 0.953125f, 0.000000f, 1.000000f, 0.937500f, 0.000000f, 1.000000f, 0.921875f, 0.000000f, 1.000000f, 0.906250f, 0.000000f, 1.000000f, 0.890625f, 0.000000f, 1.000000f, 0.875000f, 0.000000f, 1.000000f, 0.859375f, 0.000000f, 1.000000f, 0.843750f, 0.000000f, 1.000000f, 0.828125f, 0.000000f, 1.000000f, 0.812500f, 0.000000f, 1.000000f, 0.796875f, 0.000000f, 1.000000f, 0.781250f, 0.000000f, 1.000000f, 0.765625f, 0.000000f, 1.000000f, 0.750000f, 0.000000f, 1.000000f, 0.734375f, 0.000000f, 1.000000f, 0.718750f, 0.000000f, 1.000000f, 0.703125f, 0.000000f, 1.000000f, 0.687500f, 0.000000f, 1.000000f, 0.671875f, 0.000000f, 1.000000f, 0.656250f, 0.000000f, 1.000000f, 0.640625f, 0.000000f, 1.000000f, 0.625000f, 0.000000f, 1.000000f, 0.609375f, 0.000000f, 1.000000f, 0.593750f, 0.000000f, 1.000000f, 0.578125f, 0.000000f, 1.000000f, 0.562500f, 0.000000f, 1.000000f, 0.546875f, 0.000000f, 1.000000f, 0.531250f, 0.000000f, 1.000000f, 0.515625f, 0.000000f, 1.000000f, 0.500000f, 0.000000f, 1.000000f, 0.484375f, 0.000000f, 1.000000f, 0.468750f, 0.000000f, 1.000000f, 0.453125f, 0.000000f, 1.000000f, 0.437500f, 0.000000f, 1.000000f, 0.421875f, 0.000000f, 1.000000f, 0.406250f, 0.000000f, 1.000000f, 0.390625f, 0.000000f, 1.000000f, 0.375000f, 0.000000f, 1.000000f, 0.359375f, 0.000000f, 1.000000f, 0.343750f, 0.000000f, 1.000000f, 0.328125f, 0.000000f, 1.000000f, 0.312500f, 0.000000f, 1.000000f, 0.296875f, 0.000000f, 1.000000f, 0.281250f, 0.000000f, 1.000000f, 0.265625f, 0.000000f, 1.000000f, 0.250000f, 0.000000f, 1.000000f, 0.234375f, 0.000000f, 1.000000f, 0.218750f, 0.000000f, 1.000000f, 0.203125f, 0.000000f, 1.000000f, 0.187500f, 0.000000f, 1.000000f, 0.171875f, 0.000000f, 1.000000f, 0.156250f, 0.000000f, 1.000000f, 0.140625f, 0.000000f, 1.000000f, 0.125000f, 0.000000f, 1.000000f, 0.109375f, 0.000000f, 1.000000f, 0.093750f, 0.000000f, 1.000000f, 0.078125f, 0.000000f, 1.000000f, 0.062500f, 0.000000f, 1.000000f, 0.046875f, 0.000000f, 1.000000f, 0.031250f, 0.000000f, 1.000000f, 0.015625f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.984375f, 0.000000f, 0.000000f, 0.968750f, 0.000000f, 0.000000f, 0.953125f, 0.000000f, 0.000000f, 0.937500f, 0.000000f, 0.000000f, 0.921875f, 0.000000f, 0.000000f, 0.906250f, 0.000000f, 0.000000f, 0.890625f, 0.000000f, 0.000000f, 0.875000f, 0.000000f, 0.000000f, 0.859375f, 0.000000f, 0.000000f, 0.843750f, 0.000000f, 0.000000f, 0.828125f, 0.000000f, 0.000000f, 0.812500f, 0.000000f, 0.000000f, 0.796875f, 0.000000f, 0.000000f, 0.781250f, 0.000000f, 0.000000f, 0.765625f, 0.000000f, 0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.734375f, 0.000000f, 0.000000f, 0.718750f, 0.000000f, 0.000000f, 0.703125f, 0.000000f, 0.000000f, 0.687500f, 0.000000f, 0.000000f, 0.671875f, 0.000000f, 0.000000f, 0.656250f, 0.000000f, 0.000000f, 0.640625f, 0.000000f, 0.000000f, 0.625000f, 0.000000f, 0.000000f, 0.609375f, 0.000000f, 0.000000f, 0.593750f, 0.000000f, 0.000000f, 0.578125f, 0.000000f, 0.000000f, 0.562500f, 0.000000f, 0.000000f, 0.546875f, 0.000000f, 0.000000f, 0.531250f, 0.000000f, 0.000000f, 0.515625f, 0.000000f, 0.000000f };
  ::glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_FLOAT, color_jet ) ;

  //--------------------------------------------------//
  // Parse command line
  bool quit = parse_command_line(argc, argv) ;

  //--------------------------------------------------//
  // GLUT main loop
  if( !quit ) ::glutMainLoop();

  return 0 ;
}
//_____________________________________________________________________________
