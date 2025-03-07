#!/bin/bash

# 文件路径
COMPILE_TOOL_DIR=$1
OUTPUT_DIR=$2
PROJECTNAME=$3

# 检查参数
if [ -z "$OUTPUT_DIR" ] || [ -z "$PROJECTNAME" ]; then
    echo "Usage: $0 <output_dir> <project_name>"
    exit 1
fi

OUT_FILE="${OUTPUT_DIR}/${PROJECTNAME}.out"

# 检查 .out 文件是否存在
if [ ! -f "$OUT_FILE" ]; then
    echo "Error: ${OUT_FILE} not found!"
    exit 1
fi

SIZE="${COMPILE_TOOL_DIR}/bin/arm-none-eabi-size"
OBJDUMP="${COMPILE_TOOL_DIR}/bin/arm-none-eabi-objdump"

# 使用 arm-none-eabi-size 查看详细的段信息
SIZE_OUTPUT=$(${SIZE} "$OUT_FILE")
echo "$SIZE_OUTPUT"
echo "$SIZE_OUTPUT" > "${OUTPUT_DIR}/${PROJECTNAME}_size.txt"

# 解析 SIZE_OUTPUT 以计算固件大小
read -r TEXT_SIZE DATA_SIZE BSS_SIZE _ <<< $(echo "$SIZE_OUTPUT" | tail -n 1)
FIRMWARE_SIZE=$((TEXT_SIZE + DATA_SIZE))
USE_RAM_SZIE=$((DATA_SIZE + BSS_SIZE))
echo "Firmware size: $FIRMWARE_SIZE bytes"
echo "Use RAM size: $USE_RAM_SZIE bytes"

${SIZE} --format=SysV "$OUT_FILE" > "${OUTPUT_DIR}/${PROJECTNAME}_detail_size.txt"

# 使用 arm-none-eabi-objdump 生成段信息和反汇编代码
echo "Generating section information and disassembly..."
${OBJDUMP} -h "$OUT_FILE" > "${OUTPUT_DIR}/${PROJECTNAME}_sections.txt"
${OBJDUMP} -S "$OUT_FILE" > "${OUTPUT_DIR}/${PROJECTNAME}_disassembly.txt"

echo "Analysis completed."