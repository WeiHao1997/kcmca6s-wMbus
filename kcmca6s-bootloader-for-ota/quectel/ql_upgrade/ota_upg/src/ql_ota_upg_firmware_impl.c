/*
 * ql_ota_upg_firmware_impl.c
 *
 *  Created on: 2025年5月28日
 *      Author: kahn.wei
 */

#include <ql_ota_upg_firmware_impl.h>

#if (defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__))

int32_t ql_ota_upg_firmware_init_impl(ql_internal_flash_t *internal_flash, MemoryPool *memory_pool)
{
    // Initialize the firmware implementation here
    // This function should set up the internal flash and other necessary components
    if(internal_flash == NULL)
    {
        return -1; // Return error if internal_flash is NULL
    }

    if (memory_pool == NULL) 
    {
        ql_printf("Failed to initialize memory pool\r\n");
        return -1; // Return error if memory pool initialization fails
    }

    memory_pool->offset = 0; // Reset the memory pool offset
    memset(memory_pool->pool, 0, MEMORY_POOL_SIZE); // Clear the memory pool

    return 0; // Return 0 on success
}

static int32_t ql_ota_upg_firmware_segment_check(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t segment_size, MemoryPool *memory_pool)
{
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if(memory_pool == NULL)
    {
        return -1;
    }

    if((addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr) ||
       (addr + segment_size > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].is_size))
    {
        ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, addr, internal_flash->sector_size);
        return -1; // Return error if address is out of bounds
    }

    uint8_t *buffer = memory_pool_alloc(memory_pool, segment_size);

    if (buffer == NULL) 
    {
        ql_printf("Failed to allocate memory for firmware segment check\r\n");
        memory_pool_reset(memory_pool);
        return -1; // Return error if memory allocation fails
    }

    // Read the segment from internal flash
    int32_t result = internal_flash->vptr->read(internal_flash, addr, buffer, segment_size);
    if (result != 0) 
    {
        ql_printf("Failed to read firmware segment at addr=0x%08X\r\n", addr);
        memory_pool_reset(memory_pool);
        return result; // Return error if read operation fails
    }
    // Perform integrity check on the segment
    uint16_t calc_crc16 = crc16_calculate(buffer, segment_size); 

    uint16_t expected_crc16 = 0;
    // Read the expected CRC16 value from the end of the segment
    if (internal_flash->vptr->read(internal_flash, addr + segment_size, (uint8_t *)&expected_crc16, sizeof(expected_crc16)) != 0) 
    {
        ql_printf("Failed to read expected CRC16 at addr=0x%08X\r\n", addr + segment_size);
        memory_pool_reset(memory_pool);
        return -1; // Return error if read operation fails
    }

    if (calc_crc16 != expected_crc16) 
    {
        ql_printf("CRC16 check failed for firmware segment at addr=0x%08X: expected=0x%04X, calculated=0x%04X\r\n", 
            addr, expected_crc16, calc_crc16);

        memory_pool_reset(memory_pool);
        return -1; // Return error if CRC check fails
    }

    memory_pool_reset(memory_pool);

    return 0; // Return 0 on success
}

int32_t ql_ota_upg_firmware_integrity_check_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len, QL_FLASH_REGION_TYPE i_region, uint32_t segment_size, MemoryPool *memory_pool)
{
    // Check if the specified sector is valid for the given address and length
    // This function should return 0 on success or an error code on failure
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if(memory_pool == NULL)
    {
        return -1;
    }

    if((i_region != QL_FLASH_REGION_APPLICATION_BANK_1) && (i_region != QL_FLASH_REGION_APPLICATION_BANK_2))
    {
        return -1;
    }

    if(i_region == QL_FLASH_REGION_APPLICATION_BANK_1)
    {
        if (len == 0 || addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr ||
            addr + len > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].is_size)
        {
            ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, addr, len);
            return -1; // Return error if address or length is invalid
        }
    }

    if(i_region == QL_FLASH_REGION_APPLICATION_BANK_2)
    {
        if (len == 0 || addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr ||
            addr + len > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].is_size)
        {
            ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, addr, len);
            return -1; // Return error if address or length is invalid
        }
    }

    uint32_t read_cnt = (len % segment_size) ? (len / segment_size + 1) : (len / segment_size);
    uint32_t calc_firmware_crc32 = 0;
    uint32_t read_len = 0;
    uint32_t image_size = len;
    for (uint32_t i = 0; i < read_cnt; i++)
    {
        read_len = segment_size < image_size ? segment_size : image_size;

        uint32_t segment_addr = (i_region == QL_FLASH_REGION_APPLICATION_BANK_1) ? \
            (addr + i * segment_size) : \
            (addr + i * segment_size + i * sizeof(uint16_t));

        uint8_t *buffer = memory_pool_alloc(memory_pool, read_len);
        if (buffer == NULL) 
        {
            ql_printf("Failed to allocate memory for firmware segment read\r\n");
            memory_pool_reset(memory_pool);
            return -1; // Return error if memory allocation fails
        }

        int32_t result = internal_flash->vptr->read(internal_flash, segment_addr, buffer, read_len);
        if (result != 0) 
        {
            ql_printf("Failed to read firmware segment at addr=0x%08X\r\n", segment_addr);
            memory_pool_reset(memory_pool);
            return result; // Return error if read operation fails
        }

        calc_firmware_crc32 = Ql_Check_CRC32(calc_firmware_crc32, buffer, 0, read_len);

        ql_printf("Checking segment: addr=0x%08X, segment_size=%d, read_count=%d, percent=%d\r\n", 
                segment_addr, read_len, read_cnt, (i + 1) * 100 / read_cnt);

        memory_pool_reset(memory_pool);
        
        image_size -= read_len;
    }

    uint8_t *info_buffer = memory_pool_alloc(memory_pool, sizeof(ql_ota_firmware_info_t));

    uint32_t info_addr = (i_region == QL_FLASH_REGION_APPLICATION_BANK_1) ?     \
        internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr : \
        internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_2].start_addr;

    int32_t result = internal_flash->vptr->read(internal_flash, info_addr, info_buffer, sizeof(ql_ota_firmware_info_t));
    if (result != 0) 
    {
        ql_printf("Failed to read firmware info at addr=0x%08X\r\n", addr);
        memory_pool_reset(memory_pool);
        return result; // Return error if read operation fails
    }

    ql_ota_firmware_info_t *firmware_info = (ql_ota_firmware_info_t *)info_buffer;

    if (firmware_info->magic_word != QL_OTA_FIRMWARE_MAGIC_WORD) 
    {
        ql_printf("Firmware magic word mismatch: expected=0x%08X, found=0x%08X\r\n", QL_OTA_FIRMWARE_MAGIC_WORD, firmware_info->magic_word);
        memory_pool_reset(memory_pool);
        return -1; // Return error if magic word does not match
    }

    if (firmware_info->firmware_crc32 != calc_firmware_crc32) 
    {
        ql_printf("Firmware CRC32 mismatch: expected=0x%08X, calculated=0x%08X\r\n", firmware_info->firmware_crc32, calc_firmware_crc32);
        memory_pool_reset(memory_pool);
        return -1; // Return error if CRC32 does not match
    }

    memory_pool_reset(memory_pool);

    ql_printf("Firmware integrity check passed: addr=0x%08X, len=%d, segment_size=%d, firmware_crc32=0x%08X\r\n", 
             addr, len, segment_size, firmware_info->firmware_crc32);
    return 0; // Return 0 on success
}

int32_t ql_ota_upg_firmware_erase_origin_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len)
{
    // Erase the specified region of the internal flash
    // This function should return 0 on success or an error code on failure
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if (len == 0 || addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr ||
        addr + len > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].is_size)
    {
        ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, addr, len);
        return -1; // Return error if address or length is invalid
    }

    uint32_t erase_cnt = (len % internal_flash->sector_size) ? (len / internal_flash->sector_size + 1) : (len / internal_flash->sector_size);

    for (uint32_t i = 0; i < erase_cnt; i++) 
    {
        uint32_t erase_addr = addr + i * internal_flash->sector_size;

        if (internal_flash->vptr->check_region(internal_flash, erase_addr, internal_flash->sector_size) != 0) 
        {
            ql_printf("Failed to check firmware region at addr=0x%08X\r\n", erase_addr);
            return -1; // Return error if region check fails
        }

        if(internal_flash->vptr->erase(internal_flash, erase_addr, internal_flash->sector_size) != 0) // Erase the sector
        {
            ql_printf("Failed to erase firmware region at addr=0x%08X\r\n", erase_addr);
            return -1; // Return error if erase operation fails
        }

        ql_printf("Erasing region: addr=0x%08X, len=%d, sector_size=%d, erase_cnt=%d, percent=%d\r\n", erase_addr, len, internal_flash->sector_size, erase_cnt, (i + 1) * 100 / erase_cnt);
    }

    if (internal_flash->vptr->check_region(internal_flash, internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr, internal_flash->sector_size) != 0) 
    {
        ql_printf("Failed to check config info region\r\n");
        return -1; // Return error if region check fails
    }

    if(internal_flash->vptr->erase(internal_flash, internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr, internal_flash->sector_size) != 0) // Erase the sector
    {
        ql_printf("Failed to erase config info region\r\n");
        return -1; // Return error if erase operation fails
    }

    ql_printf("Erasing config info region: addr=0x%08X, len=%d, sector_size=%d\r\n", 
        internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr, 
        internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].is_size, 
        internal_flash->sector_size);

    ql_printf("Firmware erase completed successfully\r\n");

    return 0; // Call the erase function of the internal flash
}

int32_t ql_ota_upg_firmware_move_impl(ql_internal_flash_t *internal_flash, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t segment_size, MemoryPool *memory_pool)
{
    // Move the firmware from the specified address to the target location
    // This function should return 0 on success or an error code on failure
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if(memory_pool == NULL)
    {
        return -1;
    }

    if (len == 0 || dst_addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr ||
        dst_addr + len > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].is_size)
    {
        ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, src_addr, len);
        return -1; // Return error if address or length is invalid
    }

    uint8_t *buffer = memory_pool_alloc(memory_pool, segment_size);

    uint32_t read_cnt = (len % segment_size) ? (len / segment_size + 1) : (len / segment_size);

    for (size_t i = 0; i < read_cnt; i++)
    {
        uint32_t segment_addr = src_addr + i * segment_size + i * sizeof(uint16_t); // Adjust for CRC16 at the end of each segment

        if (internal_flash->vptr->read(internal_flash, segment_addr, buffer, segment_size) != 0) 
        {
            ql_printf("Failed to read firmware segment at addr=0x%08X\r\n", segment_addr);
            memory_pool_reset(memory_pool);
            return -1; // Return error if read operation fails
        }

        if (internal_flash->vptr->write(internal_flash, dst_addr + i * segment_size, buffer, segment_size) != 0) 
        {
            ql_printf("Failed to write firmware segment to addr=0x%08X\r\n", dst_addr + i * segment_size);
            memory_pool_reset(memory_pool);
            return -1; // Return error if write operation fails
        }

        ql_printf("Moving firmware: src_addr=0x%08X, dst_addr=0x%08X, percent=%d\r\n", 
                 segment_addr, dst_addr + i * segment_size, (i + 1) * 100 / read_cnt);
    }

    memory_pool_reset(memory_pool);

    uint8_t *info_buffer = memory_pool_alloc(memory_pool, sizeof(ql_ota_firmware_info_t));

    uint32_t info_addr = internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_2].start_addr;

    int32_t result = internal_flash->vptr->read(internal_flash, info_addr, info_buffer, sizeof(ql_ota_firmware_info_t));
    if (result != 0) 
    {
        ql_printf("Failed to read firmware info at addr=0x%08X\r\n", info_addr);
        memory_pool_reset(memory_pool);
        return result; // Return error if read operation fails
    }

    if(internal_flash->vptr->write(internal_flash, internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr, info_buffer, sizeof(ql_ota_firmware_info_t))!= 0)
    {
        ql_printf("Failed to write firmware info at addr=0x%08X\r\n", internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1]);
        return -1;
    }

    memory_pool_reset(memory_pool);

    ql_printf("Firmware move successfully: src_addr=0x%08X, dst_addr=0x%08X, len=%d, segment_size=%d\r\n", 
             src_addr, dst_addr, len, segment_size);

    return 0; // Return 0 on success
}

int32_t ql_ota_upg_firmware_check_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len, uint32_t segment_size, MemoryPool *memory_pool)
{
    // Check the integrity of the firmware at the specified address and length
    // This function should return 0 on success or an error code on failure
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if(memory_pool == NULL)
    {
        return -1;
    }

    if (len == 0 || addr < internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr ||
        addr + len > internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr + internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].is_size)
    {
        ql_printf("[LINE:%d]Invalid address or length for firmware erase: addr=0x%08X, len=%d\r\n", __LINE__, addr, len);
        return -1; // Return error if address or length is invalid
    }

    uint32_t read_cnt = (len % segment_size) ? (len / segment_size + 1) : (len / segment_size);

    for (uint32_t i = 0; i < read_cnt; i++) 
    {
        uint32_t segment_addr = addr + i * segment_size + i * sizeof(uint16_t); // Adjust for CRC16 at the end of each segment
        uint32_t read_len = (i == read_cnt - 1) ? (len % segment_size) : segment_size; // Handle last segment size

        if (ql_ota_upg_firmware_segment_check(internal_flash, segment_addr, read_len, memory_pool) != 0) 
        {
            ql_printf("Firmware segment check failed at addr=0x%08X\r\n", segment_addr);
            return -1; // Return error if segment check fails
        }

        ql_printf("Checking firmware: addr=0x%08X, segment_size=%d, read_count=%d, percent=%d\r\n", 
                 segment_addr, read_len, read_cnt, (i + 1) * 100 / read_cnt);
    }

    ql_printf("Firmware check passed: start addr=0x%08X, len=%d, end addr:%08X\r\n", 
             addr, len, addr + len);

    return 0; // Return 0 on success
}

int32_t ql_ota_upg_firmware_get_info_impl(ql_internal_flash_t *internal_flash, ql_ota_firmware_info_t *info, QL_FLASH_REGION_TYPE i_region)
{
    // Retrieve the firmware information such as size and CRC32
    // This function should fill the info structure and return 0 on success
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    if (info == NULL) 
    {
        ql_printf("Firmware info structure is NULL\r\n");
        return -1; // Return error if info is NULL
    }
    
    ql_ota_firmware_info_t firmware_info = { 0 };

    if(i_region == QL_FLASH_REGION_APPLICATION_BANK_1)
    {
        if(internal_flash->vptr->read(internal_flash, 
            internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_1].start_addr, 
            (uint8_t *)&firmware_info, 
            sizeof(ql_ota_firmware_info_t)) != 0)
        {
            ql_printf("Failed to read firmware info from internal flash\r\n");
            return -1; // Return error if read operation fails
        }
    }

    if(i_region == QL_FLASH_REGION_APPLICATION_BANK_2)
    {
        if(internal_flash->vptr->read(internal_flash, 
            internal_flash->region[QL_FLASH_REGION_CONFIG_INFO_BANK_2].start_addr, 
            (uint8_t *)&firmware_info, 
            sizeof(ql_ota_firmware_info_t)) != 0)
        {
            ql_printf("Failed to read firmware info from internal flash\r\n");
            return -1; // Return error if read operation fails
        } 
    }

    info->magic_word = firmware_info.magic_word;
    info->firmware_size = firmware_info.firmware_size;
    info->firmware_crc32 = firmware_info.firmware_crc32;
    info->segment_length = firmware_info.segment_length;

    ql_printf("Firmware Info: config_addr=%08X, magic_word=0x%08X, size=%d, crc32=0x%08X, segment_length=%d\r\n", 
             internal_flash->region[i_region].start_addr, info->magic_word, info->firmware_size, info->firmware_crc32, info->segment_length);

    if(info->magic_word != QL_OTA_FIRMWARE_MAGIC_WORD)
    {
        ql_printf("Magic Info Error=0x%08X\r\n", info->magic_word);
        return -1;
    }

    return 0; // Return 0 on success
}

int32_t ql_ota_upg_firmware_deinit_impl(ql_internal_flash_t *internal_flash)
{
    // Deinitialize the firmware implementation
    // This function should clean up resources and return 0 on success
    if(internal_flash == NULL)
    {
        ql_printf("Internal flash is NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    return 0; // Return 0 on success
}

#endif /* defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__) */
