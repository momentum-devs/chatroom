include(CMakeParseArguments)

find_program(ODB_EXECUTABLE NAMES odb DOC "ODB executable")
mark_as_advanced(ODB_EXECUTABLE)

function(odb_compile)
    if (NOT ODB_EXECUTABLE)
        message(SEND_ERROR "ODB compiler not found")
    endif ()

    # Parse arguments passed
    set(options GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA)
    set(oneValueParams DB SCHEMA_FORMAT SCHEMA_NAME TABLE_PREFIX
            STANDARD OUTPUT_DIRECTORY SLOC_LIMIT
            HEADER_SUFFIX INLINE_SUFFIX SOURCE_SUFFIX FILE_SUFFIX
            HEADER_PROLOGUE INLINE_PROLOGUE SOURCE_PROLOGUE
            HEADER_EPILOGUE INLINE_EPILOGUE SOURCE_EPILOGUE
            PROFILE DEFAULT_POINTER)
    set(multiValueParams HEADERS INCLUDES)

    cmake_parse_arguments(PARAM "${options}"
            "${oneValueParams}" "${multiValueParams}" ${ARGN})

    if (PARAM_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to ODB_COMPILE(): \"${PARAM_UNPARSED_ARGUMENTS}\"")
    endif ()

    if (NOT PARAM_HEADERS)
        message(FATAL_ERROR "HEADERS parameter missing")
    endif ()

    # We need some defaults for this to work in a nice way.
    set(odb_output_directory "${CMAKE_CURRENT_BINARY_DIR}/odb")
    set(odb_header_suffix ".hxx")
    set(odb_inline_suffix ".ixx")
    set(odb_source_suffix ".cxx")
    set(odb_file_suffix "_odb")

    # This variable contains the command line arguments
    # passed to odb.
    set(ODB_ARGS)

    if (PARAM_GENERATE_QUERY)
        list(APPEND ODB_ARGS --generate-query)
    endif ()

    if (PARAM_GENERATE_SESSION)
        list(APPEND ODB_ARGS --generate-session)
    endif ()

    if (PARAM_GENERATE_SCHEMA)
        list(APPEND ODB_ARGS --generate-schema)
    endif ()

    if (PARAM_DB)
        list(APPEND ODB_ARGS --database ${PARAM_DB})
    endif ()

    if (PARAM_SCHEMA_FORMAT)
        list(APPEND ODB_ARGS --schema-format ${PARAM_SCHEMA_FORMAT})
    endif ()

    if (PARAM_SCHEMA_NAME)
        list(APPEND ODB_ARGS --schema-name ${PARAM_SCHEMA_NAME})
    endif ()

    if (PARAM_TABLE_PREFIX)
        list(APPEND ODB_ARGS --table-prefix ${PARAM_TABLE_PREFIX})
    endif ()

    if (PARAM_STANDARD)
        list(APPEND ODB_ARGS --std ${PARAM_STANDARD})
    endif ()

    if (PARAM_OUTPUT_DIRECTORY)
        set(odb_output_directory ${PARAM_OUTPUT_DIRECTORY})
    endif ()

    if (PARAM_SLOC_LIMIT)
        list(APPEND ODB_ARGS --sloc-limit {PARAM_SLOC_LIMIT})
    endif ()

    if (PARAM_HEADER_SUFFIX)
        set(odb_header_suffix ${PARAM_HEADER_SUFFIX})
    endif ()

    if (PARAM_INLINE_SUFFIX)
        set(odb_inline_suffix ${PARAM_INLINE_SUFFIX})
    endif ()

    if (PARAM_SOURCE_SUFFIX)
        set(odb_source_suffix ${PARAM_SOURCE_SUFFIX})
    endif ()

    if (PARAM_FILE_SUFFIX)
        set(odb_file_suffix ${PARAM_FILE_SUFFIX})
    endif ()

    if (PARAM_HEADER_PROLOGUE)
        list(APPEND ODB_ARGS --hxx-prologue-file ${PARAM_HEADER_PROLOGUE})
    endif ()
    if (PARAM_INLINE_PROLOGUE)
        list(APPEND ODB_ARGS --ixx-prologue-file ${PARAM_INLINE_PROLOGUE})
    endif ()
    if (PARAM_SOURCE_PROLOGUE)
        list(APPEND ODB_ARGS --cxx-prologue-file ${PARAM_SOURCE_PROLOGUE})
    endif ()
    if (PARAM_HEADER_EPILOGUE)
        list(APPEND ODB_ARGS --hxx-epilogue-file ${PARAM_HEADER_EPILOGUE})
    endif ()
    if (PARAM_INLINE_EPILOGUE)
        list(APPEND ODB_ARGS --ixx-epilogue-file ${PARAM_INLINE_EPILOGUE})
    endif ()
    if (PARAM_SOURCE_EPILOGUE)
        list(APPEND ODB_ARGS --cxx-epilogue-file ${PARAM_SOURCE_EPILOGUE})
    endif ()

    if (PARAM_PROFILE)
        list(APPEND ODB_ARGS --profile ${PARAM_PROFILE})
    endif ()

    if (PARAM_DEFAULT_POINTER)
        list(APPEND ODB_ARGS --default-pointer ${PARAM_DEFAULT_POINTER})
    endif ()

    list(APPEND ODB_ARGS --output-dir ${odb_output_directory})
    list(APPEND ODB_ARGS --hxx-suffix ${odb_header_suffix})
    list(APPEND ODB_ARGS --ixx-suffix ${odb_inline_suffix})
    list(APPEND ODB_ARGS --cxx-suffix ${odb_source_suffix})
    list(APPEND ODB_ARGS --odb-file-suffix ${odb_file_suffix})

    foreach (dir ${PARAM_INCLUDES})
        list(APPEND ODB_ARGS -I${dir})
    endforeach ()

    file(MAKE_DIRECTORY "${odb_output_directory}")
    include_directories("${odb_output_directory}")

    # Enable IMPLICIT_DEPENDS for all generators that do not ignore it
    if (${CMAKE_GENERATOR} MATCHES "^.*Makefiles.*")
        set(use_implicit_depends TRUE)
        message(STATUS "Enabling \"IMPLICIT_DEPENDS\" for \"${CMAKE_GENERATOR}\" to track ODB dependencies")
    else ()
        set(use_implicit_depends FALSE)
        message(STATUS "NOT Enabling \"IMPLICIT_DEPENDS\" for \"${CMAKE_GENERATOR}\" to track ODB dependencies")
    endif ()

    foreach (header ${PARAM_HEADERS})
        file(READ ${header} headercontent)
        # Don't generate ODB sources for headers without ODB pragmas
        if ("${headercontent}" MATCHES "#pragma[ \t]+db")
            get_filename_component(output_base ${header} NAME_WE)
            set(output_base "${odb_output_directory}/${output_base}${odb_file_suffix}")
            set(odb_source "${output_base}${odb_source_suffix}")
            set(odb_inline "${output_base}${odb_inline_suffix}")
            set(odb_header "${output_base}${odb_header_suffix}")
            list(APPEND ODB_GENERATED_SOURCES ${odb_source})
            list(APPEND ODB_GENERATED_INLINES ${odb_inline})
            list(APPEND ODB_GENERATED_HEADERS ${odb_header})
            if (use_implicit_depends)
                # Use IMPLICIT_DEPENDS to check further dependencies among headers
                # IMPLICIT_DEPENDS seems to rely on absolute paths
                get_filename_component(absolute_header ${header} ABSOLUTE)
                add_custom_command(
                        OUTPUT ${odb_source} ${odb_inline} ${odb_header}
                        COMMAND ${ODB_EXECUTABLE} ${ODB_ARGS} ${header}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        DEPENDS ${header}
                        IMPLICIT_DEPENDS CXX ${absolute_header}
                        COMMENT "Generating ODB files for ${header}"
                )
            else ()
                # Without IMPLICIT_DEPENDS - a change in a header causes a complete regeneration
                add_custom_command(
                        OUTPUT ${odb_source} ${odb_inline} ${odb_header}
                        COMMAND ${ODB_EXECUTABLE} ${ODB_ARGS} ${header}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        DEPENDS ${PARAM_HEADERS}
                        COMMENT "Generating ODB files for ${header}"
                )
            endif ()
        endif ()
    endforeach ()

    # Make these variables visible to the user
    set(ODB_GENERATED_SOURCES "${ODB_GENERATED_SOURCES}" PARENT_SCOPE)
    set(ODB_GENERATED_INLINES "${ODB_GENERATED_INLINES}" PARENT_SCOPE)
    set(ODB_GENERATED_HEADERS "${ODB_GENERATED_HEADERS}" PARENT_SCOPE)
endfunction()

# OCB.cmake - ODB Support for CMake
