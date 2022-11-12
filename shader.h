#ifndef _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663
#define _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663

#include "statusType.h"

#include "math/linAlg.h"

#include <cinttypes>
#include <string>
#include <utility>
#include <vector>

#define GL_SHADER_UNDEFINED 0

struct Shader {
    enum class eShaderStage {
        VS,
        FS
    };

    Shader();
    ~Shader();

    using shaderProgramHandle_t = intptr_t;
    using shaderStageHandle_t   = intptr_t;
    using shaderStageDesc_t     = std::pair< eShaderStage, std::string >;
    using shaderStage_t         = std::pair< eShaderStage, shaderStageHandle_t >;

    void addShaderStage( const eShaderStage shaderStage, const std::string& shaderStr );

    Status_t build();

    void use( const bool shouldUse );
    const shaderProgramHandle_t programHandle() const { return mShaderProgram; }

    Status_t setInt( const std::string uniformVarName, const int val ) const;
    Status_t setFloat( const std::string uniformVarName, const float val ) const;

    Status_t setVec2( const std::string uniformVarName, const linAlg::vec2_t& val ) const;
    Status_t setVec3( const std::string uniformVarName, const linAlg::vec3_t& val ) const;
    Status_t setVec4( const std::string uniformVarName, const linAlg::vec4_t& val ) const;
    Status_t setVec4Array( const std::string uniformVarName, const linAlg::vec4_t *const pVec4Array, const size_t numVec4s ) const;

    Status_t setMat2( const std::string uniformVarName, const linAlg::mat2_t& matrix ) const;
    Status_t setMat3( const std::string uniformVarName, const linAlg::mat3_t& matrix ) const;
    Status_t setMat3x4( const std::string uniformVarName, const linAlg::mat3x4_t& matrix ) const;
    Status_t setMat4( const std::string uniformVarName, const linAlg::mat4_t& matrix ) const;

    private:
        std::vector< shaderStageDesc_t > mShaderStageDesc;
        std::vector< shaderStage_t >     mShaderStageObjects;
        shaderProgramHandle_t            mShaderProgram;
        bool                             mIsBound;
};

#endif // _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663
