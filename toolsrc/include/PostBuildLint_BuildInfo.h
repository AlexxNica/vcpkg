#pragma once

#include "filesystem_fs.h"
#include "PostBuildLint_BuildPolicies.h"
#include "opt_bool.h"
#include "PostBuildLint_LinkageType.h"

namespace vcpkg::PostBuildLint
{
    struct BuildInfo
    {
        static BuildInfo create(std::unordered_map<std::string, std::string> pgh);

        LinkageType::type crt_linkage;
        LinkageType::type library_linkage;

        std::map<BuildPolicies::type, opt_bool_t> policies;
    };

    BuildInfo read_build_info(const fs::path& filepath);
}
