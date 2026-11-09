#!/usr/bin/env bash
#
# Build check harness for TahakomAVRLib.
#
# (a) compiles every non-empty lib/{io,core,component,utils}/*.cpp with avr-g++
#     using the project's global flags and fails on any error;
# (b) compiles each applications/*/main.cpp and links it with all lib objects,
#     with -lm after the objects;
# (c) prints avr-size totals per app and warning counts per file;
# (d) compares warning counts to scripts/warnings.baseline and fails on
#     increases.
#
# Usage:
#   scripts/check_build.sh             # check the tree against the baselines
#   scripts/check_build.sh --baseline  # (re)generate both baselines
#
# The disassembly baseline (scripts/objdump.baseline) is saved by
# --baseline as the avr-objdump -d output of every lib object, in a
# deterministic order, so changes in code generation can be diffed.

set -u

cd "$(dirname "$0")/.." || exit 1

MCU=atmega328p
CXX=avr-g++
OBJDUMP=avr-objdump
SIZE=avr-size

GLOBAL_FLAGS="-mmcu=${MCU} -DF_CPU=16000000UL -DBAUD=9600 -D__AVR_ATmega328P__ -Os -std=c++11 -funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wall -Wextra"

# the library and applications include all headers by flat file name
INCLUDE_FLAGS="-Ilib/io -Ilib/core -Ilib/component -Ilib/utils"

LIB_GROUPS="io core component utils"
APPS="applications/BlinkLed applications/LightMeter applications/MusicBox applications/Organ applications/tests/WeakIsr"

BUILD_DIR="scripts/.build"
WARNINGS_BASELINE="scripts/warnings.baseline"
OBJDUMP_BASELINE="scripts/objdump.baseline"

BASELINE_MODE=0
if [ "${1:-}" = "--baseline" ]; then
    BASELINE_MODE=1
fi

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

FAILED=0
WARNING_REPORT=""
TOTAL_WARNINGS=0

# ---------------------------------------------------------------------------
# (a) compile all non-empty library sources
# ---------------------------------------------------------------------------
for group in ${LIB_GROUPS}; do
    mkdir -p "${BUILD_DIR}/lib/${group}"
    for src in "lib/${group}"/*.cpp; do
        [ -e "${src}" ] || continue
        # skip empty files: they carry no code
        if [ ! -s "${src}" ]; then
            echo "SKIP (empty): ${src}"
            continue
        fi
        obj="${BUILD_DIR}/lib/${group}/$(basename "${src}" .cpp).o"
        log="${BUILD_DIR}/lib/${group}/$(basename "${src}" .cpp).log"
        if ! "${CXX}" ${GLOBAL_FLAGS} ${INCLUDE_FLAGS} -c "${src}" -o "${obj}" 2> "${log}"; then
            echo "ERROR: compilation of ${src} failed:"
            cat "${log}"
            FAILED=1
            continue
        fi
        count=$(grep -c '^.*warning:' "${log}" || true)
        count=$(printf '%s' "${count}" | tr -d '[:space:]')
        [ -z "${count}" ] && count=0
        if [ "${count}" -gt 0 ]; then
            WARNING_REPORT="${WARNING_REPORT}${src} ${count}
"
            TOTAL_WARNINGS=$((TOTAL_WARNINGS + count))
        else
            WARNING_REPORT="${WARNING_REPORT}${src} 0
"
        fi
    done
done

# ---------------------------------------------------------------------------
# disassembly of every lib object (baseline material)
# ---------------------------------------------------------------------------
OBJDUMP_OUT=""
for group in ${LIB_GROUPS}; do
    for obj in "${BUILD_DIR}/lib/${group}"/*.o; do
        [ -e "${obj}" ] || continue
        OBJDUMP_OUT="${OBJDUMP_OUT}$(printf '********** %s **********\n' "${obj}")
$("${OBJDUMP}" -d "${obj}")
"
    done
done

# ---------------------------------------------------------------------------
# (b) compile and link each application with all lib objects, -lm last
# ---------------------------------------------------------------------------
for app in ${APPS}; do
    app_name=$(basename "${app}")
    mkdir -p "${BUILD_DIR}/${app_name}"
    main_obj="${BUILD_DIR}/${app_name}/main.o"
    log="${BUILD_DIR}/${app_name}/main.log"
    if ! "${CXX}" ${GLOBAL_FLAGS} ${INCLUDE_FLAGS} -c "${app}/main.cpp" -o "${main_obj}" 2> "${log}"; then
        echo "ERROR: compilation of ${app}/main.cpp failed:"
        cat "${log}"
        FAILED=1
        continue
    fi
    count=$(grep -c '^.*warning:' "${log}" || true)
    count=$(printf '%s' "${count}" | tr -d '[:space:]')
    [ -z "${count}" ] && count=0
    WARNING_REPORT="${WARNING_REPORT}${app}/main.cpp ${count}
"
    TOTAL_WARNINGS=$((TOTAL_WARNINGS + count))

    elf="${BUILD_DIR}/${app_name}/${app_name}.elf"
    link_log="${BUILD_DIR}/${app_name}/link.log"
    LIB_OBJECTS=$(find "${BUILD_DIR}/lib" -name '*.o' | LC_ALL=C sort | tr '\n' ' ')
# The libraries must be linked before the CRT startup object: the startup
# code defines every vector weakly (aliased to __bad_interrupt), and the
# first weak definition wins, so the library's weak default handlers only
# take effect if the CRT comes after them.  -nostartfiles suppresses the
# driver's automatic (first) placement; the CRT is passed explicitly here.
CRT="$("${CXX}" -mmcu=${MCU} -print-file-name=crt${MCU}.o)"
if [ ! -e "${CRT}" ]; then
    echo "ERROR: startup object ${CRT} not found"
    FAILED=1
    continue
fi
if ! "${CXX}" -mmcu=${MCU} -nostartfiles -o "${elf}" "${main_obj}" ${LIB_OBJECTS} "${CRT}" -lm 2> "${link_log}"; then
        echo "ERROR: linking ${app_name} failed:"
        cat "${link_log}"
        FAILED=1
        continue
    fi

    # (c) avr-size totals per app
    echo "--- avr-size ${app_name} ---"
    "${SIZE}" "${elf}" | tail -1
done

# ---------------------------------------------------------------------------
# (c) warning counts per file
# ---------------------------------------------------------------------------
echo "--- warning counts per file ---"
if [ -n "${WARNING_REPORT}" ]; then
    printf '%s' "${WARNING_REPORT}" | LC_ALL=C sort
fi
echo "total warnings: ${TOTAL_WARNINGS}"

# ---------------------------------------------------------------------------
# (d) compare to baseline / write baselines
# ---------------------------------------------------------------------------
if [ "${BASELINE_MODE}" -eq 1 ]; then
    printf '%s' "${WARNING_REPORT}" | LC_ALL=C sort > "${WARNINGS_BASELINE}"
    printf '%s' "${OBJDUMP_OUT}" > "${OBJDUMP_BASELINE}"
    echo "baselines written: ${WARNINGS_BASELINE}, ${OBJDUMP_BASELINE}"
else
    if [ ! -f "${WARNINGS_BASELINE}" ]; then
        echo "ERROR: ${WARNINGS_BASELINE} missing (run: scripts/check_build.sh --baseline)"
        exit 1
    fi
    INCREASE=0
    while IFS= read -r line; do
        file=${line%% *}
        base_count=${line##* }
        cur_count=$(printf '%s' "${WARNING_REPORT}" | LC_ALL=C sort | grep -F "${file} " | head -1 | awk '{print $2}')
        cur_count=$(printf '%s' "${cur_count}" | tr -d '[:space:]')
        [ -z "${cur_count}" ] && cur_count=0
        if [ "${cur_count}" -gt "${base_count}" ]; then
            echo "WARNING INCREASE: ${file}: ${base_count} -> ${cur_count}"
            INCREASE=1
        fi
    done < "${WARNINGS_BASELINE}"
    if [ "${INCREASE}" -eq 1 ]; then
        echo "FAILED: warning counts increased over ${WARNINGS_BASELINE}"
        exit 1
    fi
    if [ "${FAILED}" -eq 1 ]; then
        echo "FAILED: compile/link errors"
        exit 1
    fi
    echo "OK: no errors, no warning increases"
fi

exit 0
