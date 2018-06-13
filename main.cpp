// ********** George Terzakis 2016 ***************
//
//         University of Portsmouth 
//
// Code is based on PTAM by Klein and Murray (Copyright 2008 Isis Innovation Limited)
//
//
#include <stdlib.h>
#include <iostream>

#include "Persistence/instances.h"
#include "System.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
using namespace Persistence;

int main(int argc, char** argv)
{
  
  cout << "  Visual Tracking and Mapping with OpenCV for WebAssembly" << endl;
  cout << "  --------------- " << endl;
  cout <<endl<<endl<<" Université Paris 8 2018";
  cout << "  Based on GPTAM by George Terzakis (University of Portsmouth 2016)" << endl;
  cout << "itself based on PTAM by Klein and Murray (Isis Innovation Limited 2008)" << endl;
  cout << "   " << endl;
  cout << endl;

  cout << "  Parsing settings.cfg ...." << endl;
  GUI.LoadFile("settings.cfg");
  
  GUI.StartParserThread(); // Start parsing of the console input
  atexit(GUI.StopParserThread);

  try
  {
    System s;

#ifdef __EMSCRIPTEN__
    // void emscripten_set_main_loop(em_callback_func func, System* func_arg, int fps, int simulate_infinite_loop);
    emscripten_set_main_loop_arg(System::Run, &s, 0, 1); //setting 0 or a negative value as the fps will use the browser’s requestAnimationFrame mechanism to call the main loop function
#else
    while(!s.IsDone()) {
        s.Run();
    }
#endif
  }
  catch(cv::Exception e) {
    
      cout << endl;
      cout << "!! Failed to run System; got exception. " << endl;
      cout << "   Exception was: " << endl;
      //cout << e.what << endl;
      cout <<"At line : " << e.line << endl << e.msg << endl;
    }
    
}










