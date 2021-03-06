#pragma once

#include "vcpkg_cmd_arguments.h"
#include "vcpkg_paths.h"
#include "StatusParagraphs.h"
#include <array>
#include "version_t.h"

namespace vcpkg::Commands
{
    using command_type_a = void(*)(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    using command_type_b = void(*)(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    using command_type_c = void(*)(const vcpkg_cmd_arguments& args);

    namespace Build
    {
        enum class BuildResult
        {
            NULLVALUE = 0,
            SUCCEEDED,
            BUILD_FAILED,
            POST_BUILD_CHECKS_FAILED,
            CASCADED_DUE_TO_MISSING_DEPENDENCIES
        };

        static constexpr std::array<BuildResult, 4> BuildResult_values = { BuildResult::SUCCEEDED, BuildResult::BUILD_FAILED, BuildResult::POST_BUILD_CHECKS_FAILED, BuildResult::CASCADED_DUE_TO_MISSING_DEPENDENCIES };

        const std::string& to_string(const BuildResult build_result);
        std::string create_error_message(const BuildResult build_result, const package_spec& spec);
        std::string create_user_troubleshooting_message(const package_spec& spec);

        BuildResult build_package(const SourceParagraph& source_paragraph, const package_spec& spec, const vcpkg_paths& paths, const fs::path& port_dir, const StatusParagraphs& status_db);
        void perform_and_exit(const package_spec& spec, const fs::path& port_dir, const std::unordered_set<std::string>& options, const vcpkg_paths& paths);
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    }

    namespace BuildExternal
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    }

    namespace Install
    {
        void install_package(const vcpkg_paths& paths, const BinaryParagraph& binary_paragraph, StatusParagraphs* status_db);
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    }

    namespace CI
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    }

    namespace Remove
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths, const triplet& default_target_triplet);
    }

    namespace Update
    {
        struct outdated_package
        {
            static bool compare_by_name(const outdated_package& left, const outdated_package& right);

            package_spec spec;
            version_diff_t version_diff;
        };

        std::vector<outdated_package> find_outdated_packages(const vcpkg_paths& paths, const StatusParagraphs& status_db);
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Create
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Edit
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Search
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace List
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Owns
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Cache
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Import
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Integrate
    {
        extern const char*const INTEGRATE_COMMAND_HELPSTRING;

        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace PortsDiff
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);
    }

    namespace Help
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args, const vcpkg_paths& paths);

        void help_topic_valid_triplet(const vcpkg_paths& paths);

        void print_usage();

        void print_example(const std::string& command_and_arguments);

        std::string create_example_string(const std::string& command_and_arguments);
    }

    namespace Version
    {
        const std::string& version();
        void perform_and_exit(const vcpkg_cmd_arguments& args);
    }

    namespace Contact
    {
        const std::string& email();
        void perform_and_exit(const vcpkg_cmd_arguments& args);
    }

    namespace Hash
    {
        void perform_and_exit(const vcpkg_cmd_arguments& args);
    }

    template <class T>
    struct package_name_and_function
    {
        std::string name;
        T function;
    };

    const std::vector<package_name_and_function<command_type_a>>& get_available_commands_type_a();
    const std::vector<package_name_and_function<command_type_b>>& get_available_commands_type_b();
    const std::vector<package_name_and_function<command_type_c>>& get_available_commands_type_c();

    template <typename T>
    T find(const std::string& command_name, const std::vector<package_name_and_function<T>> available_commands)
    {
        for (const package_name_and_function<T>& cmd : available_commands)
        {
            if (cmd.name == command_name)
            {
                return cmd.function;
            }
        }

        // not found
        return nullptr;
    }
}
