/*****************************************************************************
*
* Copyright (c) 2000 - 2012, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                            GLEWInitializer.C                              //
// ************************************************************************* //
#include <algorithm>
#include <iterator>
#include "avtGLEWInitializer.h"
#include <avtCallback.h>
#include <DebugStream.h>
#include <LibraryNotFoundException.h>
#include <RuntimeSetting.h>
#include <StringHelpers.h>

namespace avt { namespace glew {

// Won't call init again if true.
static bool initialized = false;

// ****************************************************************************
//  Function: avt::glew::initialize
//
//  Purpose: Initializes GLEW using GL library from RuntimeSetting.
//           Maintains state to avoid duplicate initialization.
//
//  Arguments:
//    force : force initialization even if we've previously initialized.
//            Needed if, for example, contexts change behind our back.
//
//  Programmer: Tom Fogal
//
//  Modifications:
//
//    Tom Fogal, Tue Nov 24 10:48:00 MST 2009
//    Update for GLEW API change.
//
//    Tom Fogal, Tue Dec  8 14:07:38 MST 2009
//    Allow colon-separated list of libraries, trying them in sequence until
//    one works.
//
//    Tom Fogal, Mon Feb  8 12:52:06 MST 2010
//    Change separator to ";" to be more windows-friendly.
//
//    Kathleen Bonnell, Tue Feb 16 13:37:52 MST 2010
//    Don't use mesa on windows.
//
//    Brad Whitlock, Fri Oct 21 13:08:02 PDT 2011
//    Make it work with static linking.
//
//    Eric Brugger, Wed Feb 22 10:19:22 PST 2012
//    I added support for building without mesa.
//
// ****************************************************************************

bool initialize(bool force)
{
    if(initialized && !force) // Bail early if we've already been here.
    {
        return true;
    }

    typedef std::vector<std::string> stringvec;
    stringvec gl_errors;
    enum GL_Name_Convention convention;
    enum GL_Library_Type libtype;
#ifdef VISIT_USE_MANGLED_MESA
#ifndef WIN32
    const bool use_mesa = avtCallback::GetSoftwareRendering();
#else
    const bool use_mesa = false;
#endif
#else
    const bool use_mesa = false;
#endif

#ifdef VISIT_STATIC
    // VisIt is being built statically so we pass a NULL gl_lib to GLEW so
    // it will cause dlopen to open the program instead of an external library.
    // This lets it pick up the GL and MGL functions we've linked.
    const char *gl_lib = NULL;
    if(use_mesa)
    {
        convention = GLEW_NAME_MANGLED;
        libtype = GLEW_LIB_TYPE_OSMESA;
    }
    else
    {
        convention = GLEW_NAME_STANDARD;
        libtype = GLEW_LIB_TYPE_NATIVE;
    }
    GLenum err = glewInitLibrary(gl_lib, libtype, convention);
    if(GLEW_OK == err)
    {
        initialized = true;
    }
    else
    {
        std::ostringstream errmsg;
        errmsg << "GLEW init with library 'NULL' "
               << "failed: " << glewGetErrorString(err);
        debug4 << errmsg.str() << std::endl;
        gl_errors.push_back(errmsg.str());
    }
#else
    std::string gl_lib;
    if(use_mesa)
    {
        gl_lib = RuntimeSetting::lookups("mesa-lib");
        convention = GLEW_NAME_MANGLED;
        libtype = GLEW_LIB_TYPE_OSMESA;
    }
    else
    {
        gl_lib = RuntimeSetting::lookups("system-gl");
        convention = GLEW_NAME_STANDARD;
        libtype = GLEW_LIB_TYPE_NATIVE;
    }

    stringvec gl_libs = StringHelpers::split(gl_lib, ';');
    for(stringvec::const_iterator lib = gl_libs.begin(); lib != gl_libs.end();
        ++lib)
    {
        debug4 << "Initializing GLEW using library: " << *lib << std::endl;
        GLenum err = glewInitLibrary(lib->c_str(), libtype, convention);
        if(GLEW_OK == err)
        {
            initialized = true;
            break;
        }
        else
        {
            std::ostringstream errmsg;
            errmsg << "GLEW init with library '" << *lib << "' "
                   << "failed: " << glewGetErrorString(err);
            debug4 << errmsg.str() << std::endl;
            gl_errors.push_back(errmsg.str());
        }
    }
#endif

    if(!initialized)
    {
        std::ostringstream noGL;
        noGL << "GLEW initialization FAILED.\n";
        std::copy(gl_errors.begin(), gl_errors.end(),
                  std::ostream_iterator<std::string>(noGL, "\n"));
        std::string env_var = use_mesa ? "VISIT_MESA_LIB" : "VISIT_GL_LIB";
        std::string cmd_line = use_mesa ? "--mesa-lib" : "--system-gl-lib";
        noGL << "\nTry setting the " << env_var << " environment variable, "
             << "or using the " << cmd_line << " command line option.";
        EXCEPTION1(LibraryNotFoundException, noGL.str());
    }

    return initialized;
}

}; /* glew */ }; /* avt */
