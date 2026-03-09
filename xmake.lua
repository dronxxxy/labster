add_rules("mode.debug", "mode.release")

add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })
set_policy("build.sanitizer.address", true)

target("labster")
    set_languages("c++23")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/**.cpp")

