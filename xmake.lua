add_rules("mode.debug", "mode.release")

add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })
set_policy("build.sanitizer.address", true)

target("labster")
    set_kind("shared")
    add_includedirs("include", { public = true })
    add_files("src/**.c")

target("labsterc")
    set_kind("binary")
    add_deps("labster")
    add_files("labsterc/**.c")
