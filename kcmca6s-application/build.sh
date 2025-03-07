#!/bin/bash

# git log -n 5 > changelist.txt

export ABSOLUTE_BASE=${PWD}
BASE="./"
QUEC_BASE="quectel"

# 从文件中提取主版本号
# QUEC_VERSION=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | grep "Version" | head -n 1 | cut -d'"' -f2)
# 从文件中提取子版本号
# QUEC_VERION_SUB=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | grep "Subversion" | head -n 1 | cut -d'"' -f2)

QUEC_VER_FILE=${QUEC_BASE}/ql_main/src/ql_version.c

export BUILD_OUT="${PWD}/${QUEC_BASE}/build"

QUEC_LOG_BASE=${BUILD_OUT}/log

# 编译工具路径
export ARM_GCC_DIR_LINUX=${ABSOLUTE_BASE}/quectel/build/utils/build_tools/12.2.rel1_2023.7

# 如果文件夹不存在则进行解压和环境安装
if [ ! -d ${ARM_GCC_DIR_LINUX} ]; then
    chmod +x ./build_tools_check.sh
    ./build_tools_check.sh
fi

# 检查参数个数
case $# in

    0)
        echo "Please Input Parameter"
        QUEC_VERSION=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | grep -w "Version" | head -n 1 | cut -d'"' -f2)
        QUEC_VERION_SUB=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | grep -w "Subversion" | head -n 1 | cut -d'"' -f2)
        
        MARKET_QUEC_VERSION=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | sed -n '11p' | head -n 1 | cut -d'"' -f2)
        MARKET_QUEC_VERION_SUB=$(cat ${QUEC_BASE}/ql_main/src/ql_version.c | sed -n '12p' | head -n 1 | cut -d'"' -f2)

        echo "Example Main Version: ${QUEC_VERSION}, Sub Version: ${QUEC_VERION_SUB}"
        echo "Example Market Main Version: ${MARKET_QUEC_VERSION}, Sub Version: ${MARKET_QUEC_VERION_SUB}"
        
        exit 0
    ;;
    1)
        if [ $1 == "clean" ] ; then
            echo "clean"
            rm -rf ${QUEC_LOG_BASE}
            rm -rf ${BUILD_OUT}/build_firmware
            rm -rf ${BUILD_OUT}/utils/gen_info_tools/gen_info_tools
            make -f kcmca6s-application.Makefile clean
        fi
        exit 0
    ;;
    2)
        # 更新版本信息
        QL_NEW_VERSION=$1
        QL_NEW_SUBVERSION=$2

        if [[ ! ${QL_NEW_VERSION} == *"KCMCA6S"* ]]; then
            echo "Please Check Main Vesrion"
            exit 0
        fi

        if [[ ! ${QL_NEW_SUBVERSION} == *"V"* ]]; then
            echo "Please Check Sub Vesrion"
            exit 0
        fi

        if [[ ${QL_NEW_VERSION} == *"KCMCA6SAAR01A0"* ]]; then
            sed -i "4s/const uint8_t Version\[\] = \".*\";/const uint8_t Version[] = \"${QL_NEW_VERSION}\";/" ${QUEC_VER_FILE}
            sed -i "5s/const uint8_t Subversion\[\] = \".*\";/const uint8_t Subversion[] = \"${QL_NEW_SUBVERSION}\";/" ${QUEC_VER_FILE}
        fi

        if [[ ${QL_NEW_VERSION} == *"KCMCA6SAAR01A1"* ]]; then
            sed -i "11s/const uint8_t Version\[\] = \".*\";/const uint8_t Version[] = \"${QL_NEW_VERSION}\";/" ${QUEC_VER_FILE}
            sed -i "12s/const uint8_t Subversion\[\] = \".*\";/const uint8_t Subversion[] = \"${QL_NEW_SUBVERSION}\";/" ${QUEC_VER_FILE}
            export IS_MAKERT_VERSION_EXP="KCMCA6SAAR01A1"
        fi

        QUEC_VERSION=${QL_NEW_VERSION}
        QUEC_VERION_SUB=${QL_NEW_SUBVERSION}
        
        echo "Main Version Update: ${QL_NEW_VERSION}, Sub Version Update: ${QL_NEW_SUBVERSION}"
    ;;
    3)
        echo "Clean Compile Tools"
        rm -rf ${QUEC_BASE}/build/utils/build_tools/12.2.rel1_2023.7
        exit 0
    ;;
esac

# 检查是否包含BETA
if [[ ${QUEC_VERSION} == *"BETA"* ]]; then
    QUEC_GEN_VERSION=${QUEC_VERSION}
    QUEC_GEN_FW_BASE=${BUILD_OUT}/build_firmware/${QUEC_VERSION}
else
    QUEC_GEN_VERSION=${QUEC_VERSION}${QUEC_VERION_SUB}
    QUEC_GEN_FW_BASE=${BUILD_OUT}/build_firmware/${QUEC_VERSION}${QUEC_VERION_SUB}
fi

QUEC_GEN_FW_DEBUG_BASE=${QUEC_GEN_FW_BASE}/Debug
QUEC_GEN_FW_DEBUG_RELEASE=${QUEC_GEN_FW_BASE}/Release

# 创建文件夹
mkdir -p ${QUEC_LOG_BASE}
mkdir -p ${QUEC_GEN_FW_BASE}
mkdir -p ${QUEC_GEN_FW_DEBUG_BASE}
mkdir -p ${QUEC_GEN_FW_DEBUG_RELEASE}

# 如果是BETA版本复制免责声明
if [[ ${QUEC_VERSION} == *"BETA"* ]]; then
    cp -f ${QUEC_BASE}/build/utils/doc/Quectel_Disclaimer_for_Software_BETA_Version.pdf ${QUEC_GEN_FW_DEBUG_BASE}/Quectel_Disclaimer_for_Software_BETA_Version.pdf
fi

# 编译
if make -B -f kcmca6s-application.Makefile 2>> ${QUEC_LOG_BASE}/error.log; then
    #编译成功
    echo "Build Success"
else
    # 编译失败
    rm -rf ${BASE}/build
    echo "Build Fail, Please Check Program Code, Log Save Location :${QUEC_LOG_BASE}/error.log"
    exit 0
fi

# 复制BOOTLOADER
cp -f ${QUEC_BASE}/build/utils/bootloader/kcmca6s-bootloader.bin ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_BOOT.bin 

# 复制APP
cp -f ${BASE}build/debug/kcmca6s-application.bin ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_APP.bin 

if [ ! -f "${QUEC_BASE}/build/utils/gen_info_tools/gen_info_tools" ]; then
    if gcc ${QUEC_BASE}/build/utils/gen_info_tools/gen_info_tools.c -o ${QUEC_BASE}/build/utils/gen_info_tools/gen_info_tools 2>> ${QUEC_LOG_BASE}/tools_error.log; then
        #编译成功
        echo "Build Success"
    else
        # 编译失败
        echo "Build Fail, Please Check GCC Environment"
        exit 0
    fi
fi

# 调用工具
${QUEC_BASE}/build/utils/gen_info_tools/gen_info_tools ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_APP.bin ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_UPG.bin  ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_FW_INFO.bin 

# 复制UPG文件
cp -f ${QUEC_GEN_FW_DEBUG_BASE}/${QUEC_GEN_VERSION}_UPG.bin ${QUEC_GEN_FW_DEBUG_RELEASE}/${QUEC_GEN_VERSION}_UPG.bin

# 生成HEX文件
GEN_OBJCOPY="${ARM_GCC_DIR_LINUX}/bin/arm-none-eabi-objcopy"

BOOT_ADDR="0x08000000"
APP_ADDR="0x08008000"
APP_INFO_ADDR="0x0803E000"

BOOT_NAME=${QUEC_GEN_VERSION}_BOOT.bin
APP_NAME=${QUEC_GEN_VERSION}_APP.bin
APP_INFO_NAME=${QUEC_GEN_VERSION}_FW_INFO.bin

${GEN_OBJCOPY} \
    -I binary \
    -O ihex \
    --change-addresses "${BOOT_ADDR}" \
    "${QUEC_GEN_FW_DEBUG_BASE}/${BOOT_NAME}" \
    "${QUEC_GEN_FW_DEBUG_RELEASE}/BOOT_TEMP.hex"

${GEN_OBJCOPY} \
    -I binary \
    -O ihex \
    --change-addresses "${APP_ADDR}" \
    "${QUEC_GEN_FW_DEBUG_BASE}/${APP_NAME}" \
    "${QUEC_GEN_FW_DEBUG_RELEASE}/APP_TEMP.hex"

${GEN_OBJCOPY} \
    -I binary \
    -O ihex \
    --change-addresses "${APP_INFO_ADDR}" \
    "${QUEC_GEN_FW_DEBUG_BASE}/${APP_INFO_NAME}" \
    "${QUEC_GEN_FW_DEBUG_RELEASE}/APP_INFO_TEMP.hex"

remove_eof() {
    local file="$1"
    sed -i '$d' "$file"
}

remove_eof ${QUEC_GEN_FW_DEBUG_RELEASE}/BOOT_TEMP.hex
remove_eof ${QUEC_GEN_FW_DEBUG_RELEASE}/APP_TEMP.hex

cat ${QUEC_GEN_FW_DEBUG_RELEASE}/BOOT_TEMP.hex ${QUEC_GEN_FW_DEBUG_RELEASE}/APP_TEMP.hex ${QUEC_GEN_FW_DEBUG_RELEASE}/APP_INFO_TEMP.hex > ${QUEC_GEN_FW_DEBUG_RELEASE}/${QUEC_GEN_VERSION}.hex

# 删除临时文件
rm ${QUEC_GEN_FW_DEBUG_RELEASE}/BOOT_TEMP.hex ${QUEC_GEN_FW_DEBUG_RELEASE}/APP_TEMP.hex ${QUEC_GEN_FW_DEBUG_RELEASE}/APP_INFO_TEMP.hex

chmod +x analyze_output.sh
./analyze_output.sh ${ARM_GCC_DIR_LINUX} build/debug kcmca6s-application

# 生成Jlink烧录文件
cat > ${QUEC_GEN_FW_DEBUG_BASE}/erase_prog_flash_kcmca6s.jlink << EOF
Device EFR32FG23AXXXF256
SelectInterface SWD
Speed 40000
JTAGConf -1,-1
Connect
Sleep 1000
Halt
Sleep 1000
erase 0x08000000,0x0803BFFF
Sleep 1000
erase 0x0803E000,0x0803FFFF
Sleep 1000
loadbin ${QUEC_GEN_VERSION}_BOOT.bin,0x08000000 
Sleep 1000
loadbin ${QUEC_GEN_VERSION}_APP.bin,0x08008000 
Sleep 1000
loadbin ${QUEC_GEN_VERSION}_FW_INFO.bin,0x0803E000 
Sleep 1000
Reset
ClrRESET
Sleep 100
SetRESET
Sleep 100
Exit
EOF
