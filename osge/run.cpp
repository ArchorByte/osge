#include "config/engine.config.hpp"
#include "config/game.config.hpp"
#include "config/engine.version.hpp"
#include "logs/logs.popup.hpp"
#include "logs/logs.handler.hpp"
#include "sdl3/window/sdl3.instance.hpp"
#include "sdl3/window/window.handler.hpp"
#include "environment/env.console.hpp"
#include "environment/env.displays.hpp"
#include "environment/env.resolutions.hpp"
#include "environment/env.system.hpp"
#include "utils/tool.parser.hpp"
#include "utils/tool.integer.hpp"
#include "utils/tool.files.hpp"
#include "utils/tool.versioning.hpp"
#include "vulkan/vulkan.hpp"
#include "opengl/opengl.run.hpp"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

// Note: If the graphics API provided is not handled, we use Vulkan by default.
enum GraphicsAPI
{
    VULKAN = 0,
    OPENGL = 1
};

// Handle the exceptions.
void crash_handler
(
    const std::string &error
)
{
    error_log("The program crashed! Every object created by the engine has been destroyed to prevent any memory overload or memory leak.");
    open_crash_popup(error); // Open a message box to inform of the project crash.
    fatal_error_log(error);  // Finally trigger the crash.
}

// Code executed on start.
int main()
{
    try
    {
        // We "reset" the logs file by deleting it if it already exists.
        if constexpr (EngineConfig::ENABLE_LOGS_FILE)
        {
            std::string logs_file_name = std::string(EngineConfig::LOGS_FILE_NAME);

            if (std::filesystem::exists(logs_file_name))
                std::filesystem::remove(logs_file_name);

            create_new_empty_file(logs_file_name);
        }

        // We get rid of the console on Windows if we are running in release mode.
        if constexpr (EngineConfig::DEBUG_MODE)
        {
            #if defined(_WIN64)
                hide_console();
            #endif
        }
        else log("WARNING: Running in debug mode!");

        const int engine_version_variant = EngineVersion::ENGINE_VERSION_VARIANT;
        const int engine_version_major = EngineVersion::ENGINE_VERSION_MAJOR;
        const int engine_version_minor = EngineVersion::ENGINE_VERSION_MINOR;
        const int engine_version_patch = EngineVersion::ENGINE_VERSION_PATCH;

        const std::string engine_version = create_version(engine_version_variant, engine_version_major, engine_version_minor, engine_version_patch);
        log("Running on OSGE v" + engine_version + "!");

        start_sdl3_instance();
        check_operating_system_support(); // Check if we are running on a supported operating system.

        // Retrieve and list the index of all available monitors on this device.
        const std::vector<int> display_indexes = get_available_display_indexes();

        // We create a new default game config file if it's missing.
        if (!std::filesystem::exists("game.config"))
        {
            create_new_empty_file("game.config");
            write_file("game.config", "WINDOW_WIDTH=1920\nWINDOW_HEIGHT=1080\nWINDOW_MODE=2\nGRAPHICS_API=0\nMONITOR=1\nGPU=1", false);
        }

        // Read the data in the game.config file and parse its data into a map.
        std::map<std::string, std::string> config = parse_config_file("game.config");

        // Retrieve the data from the game.config file.
        std::string window_mode = config["WINDOW_MODE"];
        std::string graphics_api = config["GRAPHICS_API"];
        std::string monitor = config["MONITOR"];
        std::string gpu = config["GPU"];

        if (!is_an_integer(window_mode))
        {
            error_log("The window mode configured (" + window_mode + ") is not valid! Defaulted to full screen!");
            window_mode = "2";
        }

        if (!is_an_integer(graphics_api))
        {
            error_log("The graphics API configured (" + graphics_api + ") is not valid! Defaulted to Vulkan!");
            graphics_api = "0";
        }

        if (!is_an_integer(monitor))
        {
            error_log("The monitor configured (" + monitor + ") is not valid! Defaulted to the primary monitor!");
            monitor = "1";
        }

        int monitor_index = stoi(monitor);

        if (monitor_index < 1 || monitor_index > display_indexes.size())
        {
            error_log("The monitor configured (" + monitor + ") is out of bounds! Defaulted to the primary monitor!");
            monitor_index = 1;
        }

        if (!is_an_integer(gpu))
        {
            error_log("The GPU selected (" + gpu + ") is not valid! Defaulted to GPU #1!");
            gpu = 1;
        }

        int gpu_index = stoi(gpu);

        if (gpu_index < 1)
        {
            error_log("The GPU selected (" + gpu + ") is not valid as 1 is the lowest accepted index! Defaulted to GPU #1!");
            gpu = 1;
        }

        // Get the screen resolution.
        // Screen resolution stored as <width, height>.
        const std::pair<int, int> screen_resolution = get_display_resolution(display_indexes[monitor_index - 1]);

        // List the "allowed" game resolutions.
        // TODO: Add more screen ratio to the allowed game resolutions.
        const std::vector<std::string> allowed_game_resolutions = get_allowed_game_resolutions(screen_resolution);

        // Select the window resolution at start.
        // Selection priorities: game.config res, screen res and finally highest allowed res.
        const std::pair<int, int> game_resolution = select_game_resolution(config, screen_resolution, allowed_game_resolutions);

        const int window_width = game_resolution.first;
        const int window_height = game_resolution.second;
        const std::string window_title = std::string(GameConfig::GAME_TITLE);

        // We create the SDL3 window of the game.
        const SDL3_Window window(window_width, window_height, stoi(window_mode), window_title, stoi(graphics_api));

        // Select the graphics API that we are going to use.
        switch (stoi(graphics_api))
        {
            case VULKAN:
                run_using_vulkan(window.get(), gpu_index);
                break;

            case OPENGL:
                run_using_opengl();
                break;

            // If the graphics API provided by the game.config file is not handled, we default to Vulkan.
            default:
                run_using_vulkan(window.get(), gpu_index);
                break;
        }

        return 0;
    }
    catch (const std::exception &error)
    {
        crash_handler(error.what());
    }
    catch (const std::string &error)
    {
        crash_handler(error);
    }
    catch (const char &error)
    {
        crash_handler(std::to_string(error));
    }
    catch (const char* &error)
    {
        crash_handler(std::string(error));
    }
    catch (const int &error)
    {
        crash_handler(std::to_string(error));
    }
}
