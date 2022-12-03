#ifndef _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663
#define _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663

#include <cinttypes>
#include <string>
#include <utility>
#include <vector>
#include <array>

#include "eRetVal_GfxAPI.h"
#include "apiAbstractions.h"

#define GL_SHADER_UNDEFINED 0

namespace GfxAPI {
    struct Shader {

        using vec2_t = std::array<float, 2>;
        using vec3_t = std::array<float, 3>;
        using vec4_t = std::array<float, 4>;

        static constexpr size_t numRows_mat2x2 = 2;
        using mat2_t = std::array< vec2_t, numRows_mat2x2 >; // row-major storage, 2 rows, 2 columns

        static constexpr size_t numRows_mat3x3 = 3;
        using mat3_t = std::array< vec3_t, numRows_mat3x3 >; // row-major storage, 3 rows, 3 columns

        static constexpr size_t numRows_mat3x4 = 3;
        using mat3x4_t = std::array< vec4_t, numRows_mat3x4 >; // row-major storage, 3 rows, 4 columns

        static constexpr size_t numRows_mat4x4 = 4;
        using mat4_t = std::array< vec4_t, numRows_mat4x4 >; // row-major storage, 4 rows, 4 columns

        template< typename mat_T >
        static const float* const getMatrixPtr( const mat_T& matrix ) {
            return matrix[0].data();
        }


        enum class eShaderStage {
            VS,
            GS,
            FS
        };

        Shader();
        ~Shader();

        using shaderProgramHandle_t = handle_t;
        using shaderStageHandle_t = handle_t;
        using shaderStageDesc_t = std::pair< eShaderStage, std::string >;
        using shaderStage_t = std::pair< eShaderStage, shaderStageHandle_t >;

        void addShaderStage( const eShaderStage shaderStage, const std::string& shaderStr );

        eRetVal build();

        void use( const bool shouldUse );
        const shaderProgramHandle_t programHandle() const { return mShaderProgram; }

        eRetVal setInt( const std::string& uniformVarName, const int val ) const;
        eRetVal setFloat( const std::string& uniformVarName, const float val ) const;

        eRetVal setVec2( const std::string& uniformVarName, const vec2_t& val ) const;
        eRetVal setVec3( const std::string& uniformVarName, const vec3_t& val ) const;
        eRetVal setVec4( const std::string& uniformVarName, const vec4_t& val ) const;
        eRetVal setVec4Array( const std::string& uniformVarName, const vec4_t* const pVec4Array, const size_t numVec4s ) const;

        eRetVal setMat2( const std::string& uniformVarName, const mat2_t& matrix ) const;
        eRetVal setMat3( const std::string& uniformVarName, const mat3_t& matrix ) const;
        eRetVal setMat3x4( const std::string& uniformVarName, const mat3x4_t& matrix ) const;
        eRetVal setMat4( const std::string& uniformVarName, const mat4_t& matrix ) const;

    private:
        std::vector< shaderStageDesc_t > mShaderStageDesc;
        std::vector< shaderStage_t >     mShaderStageObjects;
        shaderProgramHandle_t            mShaderProgram;
        bool                             mIsBound;
    };
}
#endif // _SHADER_H_25f1bdce_2824_4834_8467_4ebc2e2bd663
