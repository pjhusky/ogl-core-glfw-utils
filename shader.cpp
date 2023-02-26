#include "shader.h"

#include <glad/glad.h>

namespace {
    static GLuint toShaderStageGL( const GfxAPI::Shader::eShaderStage& stage ) {
        switch( stage ) {
            case GfxAPI::Shader::eShaderStage::VS: return GL_VERTEX_SHADER;
            case GfxAPI::Shader::eShaderStage::GS: return GL_GEOMETRY_SHADER;
            case GfxAPI::Shader::eShaderStage::FS: return GL_FRAGMENT_SHADER;
        }
        return GL_SHADER_UNDEFINED;
    }

    static GfxAPI::eRetVal checkCompileErrors( const GfxAPI::Shader::shaderStageHandle_t& shaderHandle ) {
        GLint compileError;
        glGetShaderiv( static_cast<GLuint>( shaderHandle ), GL_COMPILE_STATUS, &compileError );
        if ( compileError != GL_TRUE ) {
            GLint logLength;
            glGetShaderiv( static_cast<GLuint>( shaderHandle ), GL_INFO_LOG_LENGTH, &logLength );
            if ( logLength > 0 ) {
                std::vector< char > compileLog;
                compileLog.resize( logLength + 1 );
                GLsizei actualLogLength;
                glGetShaderInfoLog( static_cast<GLuint>( shaderHandle ), logLength, &actualLogLength, compileLog.data() );
                if ( actualLogLength > 0 ) {
                    compileLog[ logLength ] = '\0';
                    fprintf( stderr, "compile error log:\n'%s'\n", compileLog.data() );
                    return GfxAPI::eRetVal::ERROR;
                }
            }
        }
        return GfxAPI::eRetVal::OK;
    }

    static GfxAPI::eRetVal checkLinkErrors( const GfxAPI::Shader::shaderProgramHandle_t& programHandle ) {
        GLint compileError;
        glGetProgramiv( static_cast<GLuint>( programHandle ), GL_LINK_STATUS, &compileError );
        if ( compileError != GL_TRUE ) {
            GLint logLength;
            glGetProgramiv( static_cast<GLuint>( programHandle ), GL_INFO_LOG_LENGTH, &logLength );
            if ( logLength > 0 ) {
                std::vector< char > linkLog;
                linkLog.resize( logLength + 1 );
                GLsizei actualLogLength;
                glGetProgramInfoLog( static_cast<GLuint>( programHandle ), logLength, &actualLogLength, linkLog.data() );
                if ( actualLogLength > 0 ) {
                    linkLog[ logLength ] = '\0';
                    fprintf( stderr, "link error log:\n'%s'\n", linkLog.data() );
                    return GfxAPI::eRetVal::ERROR;
                }
            }
        }
        return GfxAPI::eRetVal::OK;
    }
} // namespace

GfxAPI::Shader::Shader()
    : mIsBound( false ) 
{}

GfxAPI::Shader::~Shader() {
    if (!mShaderStageObjects.empty()) {
        for (const auto& shaderStageObject : mShaderStageObjects) {
            glDeleteShader( static_cast<GLuint>(shaderStageObject.second) );
        }
        glDeleteProgram( static_cast<GLuint>(mShaderProgram) );
    }

    mShaderStageDesc.clear();
    mShaderStageObjects.clear();
    mShaderProgram = shaderProgramHandle_t{ -1 };
    mIsBound = false;
}

void GfxAPI::Shader::addShaderStage( const Shader::eShaderStage shaderStage, const std::string& shaderStr ) {
    mShaderStageDesc.push_back( std::make_pair( shaderStage, shaderStr ) );
}

GfxAPI::eRetVal GfxAPI::Shader::build() {

    mShaderProgram = glCreateProgram();

    for ( const auto& shaderStageDesc : mShaderStageDesc ) {
        const auto& shaderStage = shaderStageDesc.first;
        const auto& shaderStr = shaderStageDesc.second;
        const shaderStageHandle_t shaderStageHandle = glCreateShader( toShaderStageGL( shaderStage ) );
        mShaderStageObjects.push_back( std::make_pair( shaderStage, shaderStageHandle ) );

        const GLchar* shaderStringsForStage[]{ shaderStr.c_str() };
        glShaderSource( static_cast<GLuint>( shaderStageHandle ), 1, shaderStringsForStage, nullptr );
        glCompileShader( static_cast<GLuint>( shaderStageHandle ) );
        if ( checkCompileErrors( shaderStageHandle ) != eRetVal::OK ) { 
            return eRetVal::ERROR; 
        }

        glAttachShader( static_cast<GLuint>( mShaderProgram ), static_cast<GLuint>( shaderStageHandle ) );
    }

    glLinkProgram( static_cast<GLuint>( mShaderProgram ) );
    if (checkLinkErrors( mShaderProgram ) != eRetVal::OK ) {
        return eRetVal::ERROR;
    }

    return eRetVal::OK;
}

void GfxAPI::Shader::use( const bool shouldUse ) {
    if ( shouldUse ) {
        mIsBound = true;
        glUseProgram( static_cast<GLuint>( mShaderProgram ) );
    } else {
        mIsBound = false;
        glUseProgram( 0 );
    }
}

GfxAPI::eRetVal GfxAPI::Shader::setInt( const std::string& uniformVarName, const int val ) const {
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    glUniform1i( uniformLocation, val );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setFloat( const std::string& uniformVarName, const float val ) const {
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    glUniform1f( uniformLocation, val );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setVec2( const std::string& uniformVarName, const vec2_t& val ) const {
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    glUniform2fv( uniformLocation, 1, val.data() );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setVec3( const std::string& uniformVarName, const vec3_t& val ) const {
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    glUniform3fv( uniformLocation, 1, val.data() );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setVec4( const std::string& uniformVarName, const vec4_t& val ) const {
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }//( uniformVarName + " not found in shader!" ); }
    glUniform4fv( uniformLocation, 1, val.data() );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setVec4Array( const std::string& uniformVarName, const vec4_t* const pVec4Array, const size_t numVec4s ) const
{
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    glUniform4fv( uniformLocation, static_cast<GLsizei>( numVec4s ), pVec4Array->data() );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setMat2( const std::string& uniformVarName, const mat2_t& matrix ) const {
    const float *const pMatrix = getMatrixPtr( matrix );
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    constexpr int32_t performTranspose = 1;
    glUniformMatrix2fv( uniformLocation, 1, performTranspose, pMatrix );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setMat3( const std::string& uniformVarName, const mat3_t& matrix ) const {
    const float *const pMatrix = getMatrixPtr( matrix );
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    constexpr int32_t performTranspose = 1;
    glUniformMatrix3fv( uniformLocation, 1, performTranspose, pMatrix );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setMat3x4( const std::string& uniformVarName, const mat3x4_t& matrix ) const {
    const float *const pMatrix = getMatrixPtr( matrix );
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    constexpr int32_t performTranspose = 1;
    glUniformMatrix3x4fv( uniformLocation, 1, performTranspose, pMatrix );
    return eRetVal::OK;
}

GfxAPI::eRetVal GfxAPI::Shader::setMat4( const std::string& uniformVarName, const mat4_t& matrix ) const {
    const float *const pMatrix = getMatrixPtr( matrix );
    const int32_t uniformLocation = glGetUniformLocation( static_cast<GLuint>( mShaderProgram ), uniformVarName.c_str() );
    if (uniformLocation < 0) { return eRetVal::ERROR; }// (uniformVarName + " not found in shader!"); }
    constexpr int32_t performTranspose = 1;
    glUniformMatrix4fv( uniformLocation, 1, performTranspose, pMatrix );
    return eRetVal::OK;
}
