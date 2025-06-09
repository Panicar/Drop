add_subdirectory(fmt)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt" FORCE)
add_subdirectory(spdlog)