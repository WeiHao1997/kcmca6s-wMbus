#!/bin/bash

GCC_PATH=${ABSOLUTE_BASE}/quectel/build/utils/build_tools/12.2.rel1_2023.7/bin
GCC_COMPRESS_PACKAGE=${ABSOLUTE_BASE}/quectel/build/utils/build_tools/12.2.rel1_2023.7.tar.gz
GCC_COMPRESS_PATH=${ABSOLUTE_BASE}/quectel/build/utils/build_tools

# 检查编译工具是否存在
if [ ! -d ${GCC_PATH} ]; then
     tar -xzvf ${GCC_COMPRESS_PACKAGE} -C ${GCC_COMPRESS_PATH}
fi

# 检查GCC是否已安装
if command -v gcc &> /dev/null; then
    echo "GCC 已安装，版本：$(gcc --version | head -n 1)"
else
    echo "GCC 未安装，正在安装..."
    if [ -f /etc/debian_version ]; then
        # Ubuntu/Debian
        sudo apt-get update
        sudo apt-get install -y gcc
    else
        echo "不支持的操作系统，请手动安装 GCC"
        exit 0
    fi

    echo "GCC 安装完成"
fi