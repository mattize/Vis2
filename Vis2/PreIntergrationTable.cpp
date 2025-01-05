#include "PreIntegrationTable.h"

float PreIntegrationTable::volume_positions[4][4] = {
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f}
};

int PreIntegrationTable::volume_values[4][4] = {
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128}
};

float PreIntegrationTable::medium_positions[4][4] = {
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f}
};

int PreIntegrationTable::medium_values[4][4] = {
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128}
};

float PreIntegrationTable::refraction_positions[4][4] = {
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f},
    {0.0f, 0.25f, 0.75f, 1.0f}
};

int PreIntegrationTable::refraction_values[4][4] = {
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128},
    {255, 128, 0, 128}
};

void PreIntegrationTable::defineUI() {

    float currentHeight = 0;
    float fixedWidth = 230.0f; 
    ImGui::SetNextWindowSizeConstraints(ImVec2(fixedWidth, 0), ImVec2(fixedWidth, FLT_MAX));
    ImGui::SetNextWindowSize(ImVec2(fixedWidth, 0));
    ImGui::SetNextWindowPos(ImVec2(0, 0)); 

    if (ImGui::Begin("Transfer Function Editor", NULL, ImGuiWindowFlags_NoResize)) {

        if (ImGui::CollapsingHeader("Volume TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Volume " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("VolumeTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &volume_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &volume_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Medium TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Medium " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("MediumTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &medium_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &medium_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator(); 

        if (ImGui::CollapsingHeader("Refraction TF")) {
            for (int i = 0; i < channels.size(); i++) {
                ImGui::Indent();
                ImGui::PushID(("Refraction " + channels[i] + " Channel").c_str());

                std::string headerLabel = channels[i] + " Channel";

                if (ImGui::CollapsingHeader(headerLabel.c_str())) {
                    for (int point = 0; point < CONTROL_POINTS_COUNT; ++point) {
                        ImGui::PushID(("RefractionTF_" + std::to_string(i) + "_" + std::to_string(point)).c_str());

                        ImGui::PushItemWidth(100.0f);
                        ImGui::SliderFloat(("Position##" + std::to_string(point)).c_str(), &refraction_positions[i][point], 0.0f, 1.0f, "%.2f");
                        ImGui::SliderInt(("Value##" + std::to_string(point)).c_str(), &refraction_values[i][point], 0, 255);

                        ImGui::PopItemWidth();
                        ImGui::PopID();
                    }
                }

                ImGui::PopID();
                ImGui::Unindent();
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Apply TFs")) {
            applyTransportFunctions();
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset TFs")) {
            resetTransportFunctions();
        }

    }
    ImGui::End();
}

void PreIntegrationTable::applyTransportFunctions() {
    /*
    uint32_t width = 512;
    uint32_t height = 512;
    VkFormat imageFormat = VK_FORMAT_R8G8B8A8_UNORM;

    // Create a staging buffer for the data transfer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = width * height * 4; // 4 bytes per pixel (RGBA)
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(device, &bufferCreateInfo, nullptr, &stagingBuffer);

    // Allocate memory for the staging buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(device, &allocInfo, nullptr, &stagingBufferMemory);

    // Bind the staging buffer memory
    vkBindBufferMemory(device, stagingBuffer, stagingBufferMemory, 0);

    // Map the staging buffer to CPU memory to write data
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, VK_WHOLE_SIZE, 0, &data);

    // Write pixel data (let's set a few pixels in the top-left corner)
    uint32_t* pixels = reinterpret_cast<uint32_t*>(data); // Pointer to write pixels

    // Example: Set a few pixels
    pixels[0] = 0xFFFF0000; // Red (R8G8B8A8_UNORM format)
    pixels[1] = 0xFF00FF00; // Green
    pixels[width] = 0xFF0000FF; // Blue (next row)
    pixels[width + 1] = 0xFFFFFFFF; // White (next row)

    // Unmap the staging buffer after writing the data
    vkUnmapMemory(device, stagingBufferMemory);

    // Create an image
    VkImage image;
    VkDeviceMemory imageMemory;

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = imageFormat;
    imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    vkCreateImage(device, &imageCreateInfo, nullptr, &image);

    // Allocate memory for the image
    vkGetImageMemoryRequirements(device, image, &memRequirements);
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);

    // Bind image memory
    vkBindImageMemory(device, image, imageMemory, 0);

    // Start a command buffer to perform the copy operation
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands(device, commandPool);

    // Transition the image to the correct layout for writing
    TransitionImageLayout(commandBuffer, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    // Copy data from the staging buffer to the image
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageOffset = { 0, 0, 0 };
    copyRegion.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    // Transition the image to be used as a texture (if needed)
    TransitionImageLayout(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // End the command buffer and submit it
    EndSingleTimeCommands(device, commandPool, commandBuffer, queue);

    // Clean up the staging buffer
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    */
}

void PreIntegrationTable::resetTransportFunctions() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            volume_positions[i][0] = 0.0f; 
            volume_positions[i][1] = 0.25f;
            volume_positions[i][2] = 0.75f;
            volume_positions[i][3] = 1.0f;
            volume_values[i][0] = 255;
            volume_values[i][1] = 128;
            volume_values[i][2] = 0; 
            volume_values[i][3] = 128;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            medium_positions[i][0] = 0.0f;
            medium_positions[i][1] = 0.25f;
            medium_positions[i][2] = 0.75f;
            medium_positions[i][3] = 1.0f;
            medium_values[i][0] = 255;
            medium_values[i][1] = 128;
            medium_values[i][2] = 0;
            medium_values[i][3] = 128;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            refraction_positions[i][0] = 0.0f;
            refraction_positions[i][1] = 0.25f;
            refraction_positions[i][2] = 0.75f;
            refraction_positions[i][3] = 1.0f;
            refraction_values[i][0] = 255;
            refraction_values[i][1] = 128;
            refraction_values[i][2] = 0;
            refraction_values[i][3] = 128;
        }
    }
}

void PreIntegrationTable::init() {
    for (int i = 0; i < 3; i++) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.flags = 0;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = 256;
        imageInfo.extent.height = 256;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imageInfo, nullptr, &preIntegrationTables[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, preIntegrationTables[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = vulkanDevice.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT /*properties..*/);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &tableMemories[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate table memory!");
        }

        vkBindImageMemory(device, preIntegrationTables[i], tableMemories[i], 0);

        transitionImageLayout(preIntegrationTables[i], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

        VkImageViewCreateInfo view{};
        view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view.format = VK_FORMAT_R8G8B8A8_UNORM;
        view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view.subresourceRange.baseMipLevel = 0;
        view.subresourceRange.baseArrayLayer = 0;
        view.subresourceRange.layerCount = 1;
        view.subresourceRange.levelCount = 1;
        view.image = preIntegrationTables[i];

        if (vkCreateImageView(device, &view, nullptr, &tableViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create table view!");
        }
    }

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = static_cast<float>(1);
    samplerCreateInfo.maxAnisotropy = 4.0;
    samplerCreateInfo.anisotropyEnable = VK_TRUE;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    if (vkCreateSampler(device, &samplerCreateInfo, nullptr, &tableSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create table sampler!");
    }

    applyTransportFunctions();
}

void PreIntegrationTable::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = vulkanDevice.beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vulkanDevice.endSingleTimeCommands(commandBuffer);
}

void PreIntegrationTable::cleanup() {
    vkDestroySampler(device, tableSampler, nullptr);

    for (size_t i = 0; i < 3; i++) {
        vkDestroyImage(device, preIntegrationTables[i], nullptr);
        vkDestroyImageView(device, tableViews[i], nullptr);
        vkFreeMemory(device, tableMemories[i], nullptr);
    }
}