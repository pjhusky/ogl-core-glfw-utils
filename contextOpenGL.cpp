#include "contextOpenGL.h"

#if defined( DEBUG )
    #include <iostream>
#endif

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <cassert>

//#include <imgui/backends/imgui_impl_win32.h>


#if 1
// https://stackoverflow.com/questions/68469954/how-to-choose-specific-gpu-when-create-opengl-context
#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) /*DWORD*/unsigned long NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif
#endif

using namespace GfxAPI;

namespace {
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
        printf( "framebufferSizeCallback: %d x %d\n", width, height ); fflush( stdout );
        glViewport( 0, 0, width, height );
    } 

// unfortunately, debug context not supported on Mac => 
//   https://learnopengl.com/In-Practice/Debugging
//   ... Note that OS X does not seem to support debug output functionality (as gathered online). ...

// FOLLOW-UP:
// http://renderingpipeline.com/2013/09/opengl-debugging-with-khr_debug/
// You can however use this trick to get the same debug functionality even on MacOS and other systems that do not support the debug extension natively ;-)
// ==> http://renderingpipeline.com/2013/09/simulating-khr_debug-on-macos-x/

// https://vallentin.dev/2015/02/23/debugging-opengl

#if defined( DEBUG ) //&& !defined( __APPLE__ ) 
    static void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam) {
        // ignore non-significant error/warning codes
        if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) { return; }

        std::cerr << "---------------" << std::endl;
        std::cerr << "Debug message (" << id << "): " <<  message << std::endl;

        switch ( source ) {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cerr << std::endl;

        switch ( type ) {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cerr << std::endl;
        
        switch ( severity ) {
            case GL_DEBUG_SEVERITY_HIGH:         
                std::cout << "Severity: high"; 
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:       
                std::cout << "Severity: medium"; 
                break;
            case GL_DEBUG_SEVERITY_LOW:          
                std::cout << "Severity: low"; 
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: 
                std::cout << "Severity: notification"; 
                break;
        } std::cerr << std::endl;
        std::cerr << std::endl << std::flush;
    }
#endif
} // namespace

ContextOpenGL::ContextOpenGL() 
    : mpWindow( nullptr ) 
{}

ContextOpenGL::~ContextOpenGL() {
    glfwTerminate();
}

const eRetVal ContextOpenGL::init( const ContextOpenGL::Settings_t& settings ) {

    //ImGui_ImplWin32_EnableDpiAwareness();

    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, settings.glMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, settings.glMinor );

#if defined( __APPLE__ )
    printf( "__APPLE__ is defined!\n" ); fflush( stdout );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE );
#endif

    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );

    glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
    
    glfwWindowHint( GLFW_RED_BITS, 8 );
    glfwWindowHint( GLFW_GREEN_BITS, 8 );
    glfwWindowHint( GLFW_BLUE_BITS, 8 );
    glfwWindowHint( GLFW_ALPHA_BITS, 8 );
    glfwWindowHint( GLFW_DEPTH_BITS, 24 );
    glfwWindowHint( GLFW_STENCIL_BITS, 8 );

    glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
    glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );

    // https://www.glfw.org/docs/latest/window_guide.html
    //GLFW_REFRESH_RATE specifies the desired refresh rate for full screen windows.
    //A value of GLFW_DONT_CARE means the highest available refresh rate will be used.This hint is ignored for windowed mode windows.
    glfwWindowHint( GLFW_REFRESH_RATE, GLFW_DONT_CARE );

#if defined( DEBUG ) // && !defined( __APPLE__ ) 
    printf( "DEBUG is defined!\n" ); fflush( stdout );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#else
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE );
#endif

    mpWindow = glfwCreateWindow( settings.windowW, settings.windowH, settings.windowTitle.c_str(), nullptr, nullptr);
    if ( mpWindow == nullptr ) {
        glfwTerminate();
        return eRetVal::ERROR;//( "Failed to create GLFW window" );
    }
    glfwMakeContextCurrent( reinterpret_cast< GLFWwindow* >( mpWindow ) );
    glfwSetFramebufferSizeCallback( reinterpret_cast< GLFWwindow* >( mpWindow ), framebufferSizeCallback );

    if ( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) ) {
        return eRetVal::ERROR;//( "Failed to initialize GLAD" );
    }

#if defined( DEBUG ) // && !defined( __APPLE__ ) 
    // if ( ogl_ext_KHR_debug ) {
    //     std::cout << "KHR_debug supported" << std::endl << std::flush;
    // }
    
    GLint flags; 
    glGetIntegerv( GL_CONTEXT_FLAGS, &flags ); 
    std::cout << " trying to hook up debug context " << std::endl << std::flush;
    if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT ) {
        std::cout << " have GL_CONTEXT_FLAG_DEBUG_BIT " << std::endl << std::flush;
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS ); 
        if (glDebugMessageCallback == nullptr) {
            std::cout << "ouch, glDebugMessageCallback is a nullptr!" << std::endl;
        } else {
            glDebugMessageCallback(glDebugOutput, nullptr);
        }
        if (glDebugMessageControl == nullptr) {
            std::cout << "ouch, glDebugMessageControl is a nullptr!" << std::endl;
        } else {
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    } else {
        std::cout << " *BUT* don't have GL_CONTEXT_FLAG_DEBUG_BIT " << std::endl << std::flush;
    #if defined( __APPLE__ ) 
        // http://renderingpipeline.com/2013/09/opengl-debugging-with-khr_debug/
        // Note that OpenGL 4.3 and later implementations don’t need to advertise the extension at all – MacOS X often does not claim to support the extensions that are in fact part of a core specification OS X sup
        if ( glDebugMessageCallback != nullptr ) { // nullptr on my machine unfortunately
            glDebugMessageCallback( glDebugOutput, NULL );
        }
    #endif
    }
#endif

    {
        int32_t w, h;
        glfwGetWindowSize( reinterpret_cast< GLFWwindow* >( mpWindow ), &w, &h );
        printf( "glfwGetWindowSize(): window created: %d x %d\n", w, h ); fflush( stdout );

        assert( w == settings.windowW && h == settings.windowH );

        float sx, sy;
        glfwGetWindowContentScale( reinterpret_cast< GLFWwindow* >( mpWindow ), &sx, &sy );
        printf( "glfwGetWindowContentScale(): window scale: %f x %f\n", sx, sy ); fflush( stdout );

        printf( "scaled window dimensions %f x %f\n", settings.windowW * sx, settings.windowH * sy );

        int32_t fbWidth, fbHeight;
        glfwGetFramebufferSize( reinterpret_cast< GLFWwindow* >( mpWindow ), &fbWidth, &fbHeight);
        printf( "glfwGetFramebufferSize(): %d x %d\n", fbWidth, fbHeight );

        glViewport( 0, 0, settings.windowW * static_cast<GLsizei>( sx + 0.5f ), settings.windowH * static_cast<GLsizei>( sy + 0.5f ) );
    }

    return eRetVal::OK;
}